#include <stdio.h>
#include "config.h"
#include "tlb.h"
#include "cpu.h"

struct PAGE_TABLE *page_table ; 
struct TLB *tlb ; 
struct TRACE *trace ; 



void run(struct TLB *tlb, struct TRACE *trace,char* filename){
    
    
    tlb = (struct TLB *) malloc(sizeof(struct TLB));
    for (int i = 0 ; i < MAX_TLB_ENTRY_NUM ; i++){
        tlb->tlb_entry[i].page_num = -1 ;
        tlb->tlb_entry[i].frame_num = -1 ;  
    }    //init_page_table(page_table);
    
    trace = (struct TRACE *)malloc(sizeof(struct TRACE));
    trace->inst_trace = (char**)malloc(sizeof(char*)*MAX_TRACE_LINE);
    for (int i = 0 ; i < MAX_TRACE_LINE ; i++){
        trace->inst_trace[i] = (char*)malloc(sizeof(char)*MAX_CHAR_PER_LINE);   
    } 
    trace->read_count = 0 ;   
    FILE *f = NULL ;
    f = fopen(filename, "r");
    char buffer[MAX_CHAR_PER_LINE] = {0,};
    char* line ;
    if (f == NULL){
        perror("No file exsit!\n");
    }    
    int count = 0 ;
    while(1){
        if (feof(f) != 0){
            printf("end of file.. done reading trace\n");
            trace->total_count = count ; 
            printf("total count is %d\n", trace->total_count);
            break ;
        }
        line = fgets(buffer, MAX_CHAR_PER_LINE, f);
        strcpy(trace->inst_trace[count], line);
        count += 1 ; 
    }  
    fclose(f);
    printf("=== Start issue instruction ===\n");
    while(1){
        int64_t virt_addr = issue_intruction(trace);
        if (virt_addr == -1){
            break ; 
        }
        access_tlb(tlb,virt_addr);
    }
}
int main(){
    char filename[] = "../400_perlbench.out";
   
    run(tlb, trace,filename);
    return 0; 
}