// issue the instruction to the page table and tlb
#include "config.h"
#include <stdio.h>
struct TRACE{
    char** inst_trace ; 
};

int init_trace(struct TRACE* trace){
    trace->inst_trace = (char**)malloc(sizeof(char*)*MAX_TRACE_LINE);
    for (int i = 0 ; i < MAX_TRACE_LINE ; i++){
        trace->inst_trace[i] = (char*)malloc(sizeof(char)*30);
    }
}

void read_trace(struct TRACE* trace, char* filename){
    FILE *f = NULL ; 
    f = fopen(filename, "r"); 
    if (f == NULL){
        perror("No file exsit!\n");
    }    
    fclose(f);
}

void issue_intruction(struct TRACE* trace){

}