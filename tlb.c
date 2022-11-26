#include "config.h"
#include "pagetable.h"
#include "tlb.h"
#include <stdio.h> 
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

extern struct PAGE_TABLE *page_table ; 

struct TLB_ENTRY{
    int64_t page_num ; 
    int64_t frame_num ; 
};

struct TLB{
    struct TLB_ENTRY tlb_entry[MAX_TLB_ENTRY_NUM];
};

void init_tlb(struct TLB *tlb){
    tlb = (struct TLB *) malloc(sizeof(struct TLB));
    for (int i = 0 ; i < MAX_TLB_ENTRY_NUM ; i++){
        tlb->tlb_entry[i].page_num = -1 ;
        tlb->tlb_entry[i].frame_num = -1 ;  
    }
}

int64_t tlb_translate_page_num(uint64_t virtual_address){
    uint64_t modified_range = (virtual_address >> 12) & 0x2ffff;
    return modified_range ;  
}

int64_t tlb_find_frame_num(struct TLB *tlb, int64_t page_num){
    for (int i = 0 ; i < MAX_TLB_ENTRY_NUM ; i++){
        if (tlb->tlb_entry[i].frame_num == page_num){
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

void tlb_update_with_space(struct TLB * tlb, int isSpace ,uint64_t virtual_address, struct PAGE_TABLE *page_table){
    tlb->tlb_entry[isSpace].page_num = tlb_translate_page_num(virtual_address);
    tlb->tlb_entry[isSpace].frame_num = acscess_page_frame(page_table, virtual_address); 
}

void tlb_update_with_nospace(struct TLB * tlb, uint64_t virtual_address, struct PAGE_TABLE *page_table){
    int evict_num = rand() % 100 ; 
    tlb->tlb_entry[evict_num].page_num = tlb_translate_page_num(virtual_address);
    tlb->tlb_entry[evict_num].frame_num = acscess_page_frame(page_table, virtual_address);
}

void tlb_update_entry(struct TLB * tlb, uint64_t virtual_address, struct PAGE_TABLE *page_table){
    //access the matching entry 
    int isSpace = tlb_is_space(tlb);
    if (isSpace == -1){
        tlb_update_with_nospace(tlb, virtual_address, page_table);
    }
    else{
        tlb_update_with_space(tlb, isSpace, virtual_address, page_table);
    }
}

int64_t access_tlb(struct TLB *tlb, uint64_t virtual_address, struct PAGE_TABLE *page_table){
    int64_t page_num = tlb_translate_page_num(virtual_address);
    //search the matching frame num 
    int64_t ret = tlb_find_frame_num(tlb, page_num);
    if (!ret){
        tlb_update_entry(tlb, virtual_address, page_table);
        return -1;     
    }
    return ret ; 

}
