#include "config.h"
#include "tlb.h"
#include <stdio.h> 
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

extern struct PAGE_TABLE *page_table ; 
extern int tlb_miss ; 
extern int tlb_hit ; 
extern int p_option;

ListNode *head = NULL;


ListNode* insert_first(ListNode* head, int value)
{
	ListNode* p = (ListNode*)malloc(sizeof(ListNode));
	p->data = value;
	
	p->link = head; 
	head = p; 
	return head;
}

int delete_oldest(ListNode* head)
{
	ListNode* removed;
    ListNode* prev_node;
	if (head == NULL) return -1;
    int evict_num;
 
	removed = head->link; 
    prev_node = head;

    while(removed->link != NULL){
        removed = removed->link;
        prev_node = prev_node->link;
    }
    prev_node->link = removed->link;;
    evict_num = removed->data;
	free(removed); 
	return evict_num; 
}

ListNode* delete_value(ListNode* head, int value)
{
	ListNode* removed;
    ListNode* prev_node;
    
    if(head->data == value){
        removed = head;
        head = head->link;
        free(removed);
        return head;
    }
    else{
        removed = head->link;
        prev_node = head;
        while(removed != NULL){
           if(removed->data == value){
                prev_node->link = removed->link;
                free(removed);
                break;
           }
           prev_node = prev_node->link;
           removed = removed->link;
        }
        return head;
    }
}

void print_list(ListNode * head){
    printf("Head=");
    int prev_data;

    for (ListNode * p = head; p != NULL; p = p->link){
        printf(" %d ->", p->data);
        if(prev_data == p->data)
            break;
        prev_data = p->data;
    }

    printf(" NULL\n");
}

int64_t tlb_translate_page_num(uint64_t virtual_address){

    int64_t modified_range = (virtual_address >> 12) & 0xffffffffffff;
    
    return modified_range ;  
}

int tlb_find_frame_num(struct TLB *tlb, int64_t page_num){

    for (int i = 0 ; i < MAX_TLB_ENTRY_NUM ; i++){
        if (tlb->tlb_entry[i].page_num == page_num){
            return i; 
        }  
    }
    return -1; 
}
int tlb_is_space(struct TLB *tlb){
    for (int i = 0 ; i < MAX_TLB_ENTRY_NUM; i++){
        if (tlb->tlb_entry[i].page_num == -1){
            return i;
        }
    }
    return -1 ; 
}
int translate_dir_num(int64_t virtual_address){
    int dir_region = (virtual_address >> 30); 
    return dir_region;
}

int translate_entry_num(int64_t virtual_address){
    int entry_region = (virtual_address >> 12) & 0x3ffff;
    return entry_region ; 
}
void tlb_update_with_space(struct TLB * tlb, int isSpace ,uint64_t virtual_address){
    tlb->tlb_entry[isSpace].page_num = tlb_translate_page_num(virtual_address);
    tlb->tlb_entry[isSpace].frame_num = (translate_dir_num(virtual_address) << 18) + translate_entry_num(virtual_address); 
}


void tlb_update_entry_random(struct TLB * tlb, uint64_t virtual_address){
    //access the matching entry 
    int isSpace = tlb_is_space(tlb);
    if (isSpace == -1){ //tlb_update_with_nospace
        int evict_num = rand() % MAX_TLB_ENTRY_NUM; 
        tlb->tlb_entry[evict_num].page_num = tlb_translate_page_num(virtual_address);
        tlb->tlb_entry[evict_num].frame_num = (translate_dir_num(virtual_address) << 18) + translate_entry_num(virtual_address); 
    }
    else{
        tlb_update_with_space(tlb, isSpace, virtual_address);
    }
}

void tlb_update_entry_lru(struct TLB * tlb, uint64_t virtual_address){
    //access the matching entry 
    int isSpace = tlb_is_space(tlb);
    int idx;
    if (isSpace == -1){
        idx = delete_oldest(head); 
        head = insert_first(head, idx);
        tlb->tlb_entry[idx].page_num= tlb_translate_page_num(virtual_address);
        tlb->tlb_entry[idx].frame_num = (translate_dir_num(virtual_address) << 18) + translate_entry_num(virtual_address);
        if(p_option == 1){
            printf("Miss With Eviction Occurs!!\n"); 
            print_list(head);}
    }
    else{
        tlb_update_with_space(tlb, isSpace, virtual_address);
        idx = isSpace;
        head = insert_first(head, idx);
        if(p_option == 1){
            printf("Cold Miss occurs!!\n");
            print_list(head);}
    }
}

int64_t access_tlb_random(struct TLB *tlb, uint64_t virtual_address){ 
    int64_t page_num = tlb_translate_page_num(virtual_address);
    //search the matching frame num 
    int64_t ret = tlb_find_frame_num(tlb, page_num);
    if (ret == -1){
        tlb_update_entry_random(tlb, virtual_address);
        tlb_miss += 1; 
        return -1;     
    }
    tlb_hit += 1; 
    return ret ; 

}

int64_t access_tlb_lru(struct TLB *tlb, uint64_t virtual_address){
   int64_t page_num = tlb_translate_page_num(virtual_address);
    //search the matching frame num
    if(p_option == 1)
        printf("Virtual address num : %ld, page_num : %ld\n", virtual_address, page_num);
    int ret = tlb_find_frame_num(tlb, page_num);

    if (ret == -1){
        tlb_update_entry_lru(tlb, virtual_address);
        tlb_miss += 1;
        return -1;
    }
    if(p_option == 1)
        printf("Hit occurs, tlb_idx : %d\n", ret);
    head = delete_value(head, ret);
    head = insert_first(head, ret);
    tlb_hit += 1;
    if(p_option == 1){
        print_list(head);}
    return ret;
}

