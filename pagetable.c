#include "config.h"
#include "pagetable.h"
#include <stdio.h> 
#include <stdint.h>
#include <stdbool.h>
struct PAGE_TABLE_ENTRY{
    bool valid ; 
    int protected ;
    uint64_t page_frame_number ;     
};
struct PAGE_TABLE_DIR{
    bool valid ; 
    uint64_t page_dir_number; 
    struct PAGE_TABLE_ENTRY page_table_entry[MAX_PAGE_TABLE_ENTRY_NUM];
};
struct PAGE_TABLE{
    struct PAGE_TABLE_DIR page_table_dir[MAX_PAGE_TABLE_DIR_NUM];
};

void init_page_table(struct PAGE_TABLE *page_table){
    page_table = (struct PAGE_TABLE*) malloc(sizeof(struct PAGE_TABLE));
    for (int i = 0; i < MAX_PAGE_TABLE_DIR_NUM; i++){
        page_table->page_table_dir->valid = 1 ; 
        page_table->page_table_dir->page_dir_number = i; 
        for (int j = 0 ; j < MAX_PAGE_TABLE_ENTRY_NUM; j++){
            page_table->page_table_dir->page_table_entry[j].valid = 1; 
            page_table->page_table_dir->page_table_entry[j].protected = 0 ; 
            page_table->page_table_dir->page_table_entry[j].page_frame_number = j ;
        } 
    }
}

int translate_dir_num(struct PAGE_TABLE *page_table,uint64_t virtual_address){
    int dir_region = (virtual_address >> 30); 
    return dir_region;
}

int translate_entry_num(struct PAGE_TABLE *page_table,uint64_t virtual_address){
    int entry_region = (virtual_address >> 12) & 0x2ffff;
    return entry_region ; 
}

int64_t acscess_page_frame(struct PAGE_TABLE *page_table, uint64_t virtual_address){
    int page_dir_num = translate_dir_num(page_table, virtual_address);
    int page_entry_num = translate_entry_num(page_table, virtual_address);   
    return (page_dir_num << 18) + page_entry_num ; 
}










