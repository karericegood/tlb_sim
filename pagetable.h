struct PAGE_TABLE_ENTRY;
struct PAGE_TABLE_DIR;
struct PAGE_TABLE;

void init_page_table(struct PAGE_TABLE *page_table);
int translate_dir_num(struct PAGE_TABLE *page_table,uint64_t virtual_address);
int translate_entry_num(struct PAGE_TABLE *page_table,uint64_t virtual_address);
int64_t acscess_page_frame(struct PAGE_TABLE *page_table, uint64_t virtual_address);