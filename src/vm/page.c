#include "vm/page.h"
#include <hash.h>
#include "threads/vaddr.h"
#include "vm/frame.h"
#include "threads/malloc.h"
#include "threads/palloc.h"

unsigned page_hash(const struct hash_elem *e, void* aux) {
    struct page* pg = hash_entry(e,struct page,hash_elem);
    return ((int) pg->addr << PGBITS);
}

bool page_less(const struct hash_elem *a, const struct hash_elem *b, void* aux){
    struct page* first = hash_entry(a,struct page,hash_elem);
    struct page* second = hash_entry(b,struct page,hash_elem);

    return (first->addr < second->addr);
}

// Function to get a user page of memory.
struct page* get_user_page(){
    struct page* p = malloc(sizeof(struct page));
    struct thread* t = thread_current();

    struct hash_elem* h_elem = hash_insert(t->page_table,&p->hash_elem);
    // This page already exists in the table.
    if(h_elem != NULL){
        free(p);
        return hash_entry(h_elem,struct page,hash_elem);
    }else{
        
    }
}
