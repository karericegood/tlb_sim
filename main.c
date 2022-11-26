#include <stdio.h>
#include "config.h"
#include "pagetable.h"
#include "tlb.h"
#include "cpu.h"

struct PAGE_TABLE *page_table ; 
struct TLB *tlb ; 
struct TRACE *trace ; 

void init(struct TRACE *trace,char* filename){ 
    init_tlb(tlb); 
    init_page_table(page_table);
    read_trace(trace, filename);
}

void run(struct PAGE_TABLE *page_table, struct TLB *tlb, struct TRACE *trace){
    while(1){
        int64_t virt_addr = issue_intruction(trace);
        if (virt_addr == -1){
            break ; 
        }
        access_tlb(tlb,virt_addr, page_table);
    }
}
int main(){
    char filename[] = "400_perlbench.out";
    init(trace, filename);
    return 0 ;

    run(page_table, tlb, trace);
    return 0; 
}