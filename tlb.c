#include "config.h"
#include "tlb.h"
#include <stdio.h> 
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

extern struct PAGE_TABLE *page_table ; 
extern int tlb_miss ; 
extern int tlb_hit ; 



void init_tlb(struct TLB *tlb){
    
}

int64_t tlb_translate_page_num(uint64_t virtual_address){

    int64_t modified_range = (virtual_address >> 12) & 0xffffffffffff;
    
    return modified_range ;  
}

int64_t tlb_find_frame_num(struct TLB *tlb, int64_t page_num){

    for (int i = 0 ; i < MAX_TLB_ENTRY_NUM ; i++){
        if (tlb->tlb_entry[i].page_num == page_num){
            return tlb->tlb_entry[i].frame_num ; 
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

void tlb_update_with_nospace(struct TLB * tlb, uint64_t virtual_address){
    //replacement policy... -> need to specify.. 
    int evict_num = rand() % MAX_TLB_ENTRY_NUM; 
    tlb->tlb_entry[evict_num].page_num = tlb_translate_page_num(virtual_address);
    tlb->tlb_entry[evict_num].frame_num = (translate_dir_num(virtual_address) << 18) + translate_entry_num(virtual_address); 
}

void tlb_update_entry(struct TLB * tlb, uint64_t virtual_address){
    //access the matching entry 
    int isSpace = tlb_is_space(tlb);
    if (isSpace == -1){
        tlb_update_with_nospace(tlb, virtual_address);
    }
    else{
        tlb_update_with_space(tlb, isSpace, virtual_address);
    }
}

int64_t access_tlb(struct TLB *tlb, uint64_t virtual_address){ 
    int64_t page_num = tlb_translate_page_num(virtual_address);
    //search the matching frame num 
    int64_t ret = tlb_find_frame_num(tlb, page_num);
    if (ret == -1){
        tlb_update_entry(tlb, virtual_address);
        tlb_miss += 1; 
        return -1;     
    }
    tlb_hit += 1; 
    return ret ; 

}
