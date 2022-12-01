#include <stdio.h>
#include "config.h"
#include "tlb.h"
//#include "cpu.h"
#include <string.h>
#include <stdlib.h>
//struct PAGE_TABLE *page_table ; 
struct TLB *tlb ; 
int tlb_miss = 0  ; 
int tlb_hit = 0 ; 
int p_option = 0; //print_option

void run(struct TLB *tlb, char* filename){ 
    tlb = (struct TLB *) malloc(sizeof(struct TLB));
    for (int i = 0 ; i < MAX_TLB_ENTRY_NUM ; i++){
        tlb->tlb_entry[i].page_num = -1 ;
        tlb->tlb_entry[i].frame_num = -1 ;  
    }    
    
    FILE *f = NULL ;
    f = fopen(filename, "r");
    char buffer[MAX_CHAR_PER_LINE] = {0,};
    char* line ;
    char* v_addr_str;
    char* op;
    int64_t v_addr_int ;
    if (f == NULL){
        perror("No file exsit!\n");
    }    
    int count = 0 ;
    while(1){
        if (feof(f) != 0){
            printf("end of file.. done reading trace\n");
            break ;
        }
        line = fgets(buffer, MAX_CHAR_PER_LINE, f);
        op=strtok(line, " ");
        v_addr_str = strtok(NULL, " ");
        if(v_addr_str == NULL)
            break;
        v_addr_int = strtoll(v_addr_str, NULL,16);
        //access_tlb_random(tlb,v_addr_int);
        access_tlb_lru(tlb,v_addr_int);
        if(p_option == 1){
            for(int i = 0; i < MAX_TLB_ENTRY_NUM; i++){
                printf("%d, %ld, %ld\n", i, tlb->tlb_entry[i].page_num, tlb->tlb_entry[i].frame_num);
            }
        }
        //if(count == 100000000)
        //    break;
        //count++;
    }  
    fclose(f);
}

int main(int argc, char* argv[]){
    char filename[] = "./400_perlbench.out";

    if(argv[1] !=  NULL){
        if(atoi(argv[1]) == 1){
            p_option = 1;
        }
    }
    run(tlb, filename);
    printf("=== hit count : %d ===\n", tlb_hit);
    printf("=== miss count : %d ===\n", tlb_miss);
    return 0; 
}
