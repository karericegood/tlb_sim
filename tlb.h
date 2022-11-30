#include <stdint.h>

struct TLB_ENTRY{
    int64_t page_num ; 
    int64_t frame_num ; 
};

struct TLB{
    struct TLB_ENTRY tlb_entry[MAX_TLB_ENTRY_NUM];
};

typedef struct ListNode {
    int data;
    struct ListNode* link;
} ListNode;

ListNode* insert_first(ListNode* head, int value);
int delete_oldest(ListNode* head);
ListNode* delete_value(ListNode* head, int value);
void print_list(ListNode* head);

int64_t tlb_translate_page_num(uint64_t virtual_address); 
int tlb_find_frame_num(struct TLB *tlb, int64_t page_num) ;
int tlb_is_space(struct TLB *tlb) ; 
void tlb_update_with_space(struct TLB * tlb, int isSpace ,uint64_t virtual_address);

void tlb_update_entry_random(struct TLB * tlb, uint64_t virtual_address); 
void tlb_update_entry_lru(struct TLB * tlb, uint64_t virtual_address); 

int64_t access_tlb_random(struct TLB *tlb, uint64_t virtual_address); 
int64_t access_tlb_lru(struct TLB *tlb, uint64_t virtual_address);

int translate_entry_num(int64_t virtual_address);
int translate_dir_num(int64_t virtual_address);
