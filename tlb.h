#include <stdint.h>

struct TLB_ENTRY{
    int64_t page_num ; 
    int64_t frame_num ; 
};

struct TLB{
    struct TLB_ENTRY tlb_entry[MAX_TLB_ENTRY_NUM];
};
void init_tlb(struct TLB *tlb); 
int64_t tlb_translate_page_num(uint64_t virtual_address); 
int64_t tlb_find_frame_num(struct TLB *tlb, int64_t page_num) ;
int tlb_is_space(struct TLB *tlb) ; 
void tlb_update_with_space(struct TLB * tlb, int isSpace ,uint64_t virtual_address);
void tlb_update_with_nospace(struct TLB * tlb, uint64_t virtual_address); 
void tlb_update_entry(struct TLB * tlb, uint64_t virtual_address); 
int64_t access_tlb(struct TLB *tlb, uint64_t virtual_address); 
int translate_entry_num(int64_t virtual_address);
int translate_dir_num(int64_t virtual_address);