// issue the instruction to the page table and tlb
#include "config.h"
#include <stdio.h>
#include <string.h> 
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "cpu.h"



void read_trace(struct TRACE* trace, char* filename){
    trace = (struct TRACE * )malloc(sizeof(struct TRACE));
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
}

int64_t issue_intruction(struct TRACE* trace){
    if (trace ->total_count <= trace -> read_count){
        return - 1 ; 
    }
  
    char* _this = strtok(trace->inst_trace[trace->read_count], " "); 
    int count = 0  ; 
    while(_this != NULL){
        if (count >= 1){
            break;
        }
        _this = strtok(NULL, " ");
        trace->read_count += 1 ; 
        
        count ++ ; 
    }    
    char this[30];
    int64_t ret = strtoll(_this, NULL, 16);
    
    return ret ; 
}