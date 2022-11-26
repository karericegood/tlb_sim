// issue the instruction to the page table and tlb
#include "config.h"
#include <stdio.h>
#include <string.h> 
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

struct TRACE{
    char** inst_trace ; 
    int read_count ; 
    int total_count ;
};

int init_trace(struct TRACE* trace){
    trace = malloc(sizeof(struct TRACE));
    trace->inst_trace = (char**)malloc(sizeof(char*)*MAX_TRACE_LINE);
    for (int i = 0 ; i < MAX_TRACE_LINE ; i++){
        trace->inst_trace[i] = (char*)malloc(sizeof(char)*MAX_CHAR_PER_LINE);
    }
    trace->read_count = 0 ; 
}

void read_trace(struct TRACE* trace, char* filename){
    FILE *f = NULL ; 
    f = fopen(filename, "r");
    char buffer[MAX_CHAR_PER_LINE] = {0,};
    char* line ;
     if (f == NULL){
        perror("No file exsit!\n");
    }    
    int count = 0 ;
    while(!feof(f)){
        line = fgets(buffer, MAX_CHAR_PER_LINE, f);
        strcpy(trace->inst_trace[count++], line);
        printf("%s", line);
    } 
    trace->total_count = count ; 
    fclose(f);
}

int64_t issue_intruction(struct TRACE* trace){
    if (trace -> total_count <= trace -> read_count){
        return - 1 ; 
    }
    char *temp_virt_addr = strtok(trace->inst_trace[trace->read_count], " ");
    trace->read_count += 1; 
    int64_t ret = strtoll(temp_virt_addr, NULL, 16);
    return ret ; 
}