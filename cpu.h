struct TRACE;
void init_trace(struct TRACE* trace);
void read_trace(struct TRACE* trace, char* filename); 
int64_t issue_intruction(struct TRACE* trace); 