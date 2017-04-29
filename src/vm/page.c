#include "vm/page.h"
#include <hash.h>
#include "threads/vaddr.h"
#include "vm/frame.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include <string.h>

unsigned page_hash(const struct hash_elem *e, void* aux) {
    struct page* pg = hash_entry(e,struct page,hash_elem);
    return ((int) pg->addr >> PGBITS);
}

bool page_less(const struct hash_elem *a, const struct hash_elem *b, void* aux){
    struct page* first = hash_entry(a,struct page,hash_elem);
    struct page* second = hash_entry(b,struct page,hash_elem);

    return (first->addr < second->addr);
}

// Allocates a page of memory that contains the address void* addr.
// Note: Doesn't allocate the struct frame yet. This is handled in page_in().
struct page* page_allocate(void* addr){
    struct page* p = malloc(sizeof(struct page));
    struct thread* t = thread_current();
    p->addr = pg_round_down(addr);
    struct hash_elem* h_elem = hash_insert(t->page_table,&p->hash_elem);
    // This page already exists in the table.
    if(h_elem != NULL){
        // free the existing structure.
        free(p);
        // Return the structure that already exists in the table.
        return hash_entry(h_elem,struct page,hash_elem);
    }else{
        // Return the newly allocated page.
        p->file = NULL;
        p->frame = NULL;
        p->sector = NULL;
        return p;
    }
}

bool page_in(void* addr){
    struct thread* t = thread_current();
    // Temporary page structure.
    struct page p;
    // Set p to the address we were given above.
    p.addr = pg_round_down(addr);
    struct hash_elem* h_elem = hash_find(t->page_table,&p.hash_elem);
    if(!h_elem){
        // Pointer to the new page.
        struct page* pg = hash_entry(h_elem,struct page,hash_elem);
        // Create a new zeroed page.
        if(pg->file == NULL && pg->sector == NULL && pg->frame == NULL){
            pg->frame = get_free_frame();
            lock_acquire(&pg->frame->f_lock);
            memset(pg->frame->base,0,PGSIZE);
            lock_release(&pg->frame->f_lock);
        }
    }

}

bool page_in_core(struct page* in_page){
    
}
