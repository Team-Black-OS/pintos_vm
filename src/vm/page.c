#include "vm/page.h"
#include <hash.h>
#include "threads/vaddr.h"
#include "vm/frame.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "userprog/pagedir.h"
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
    p->thread = thread_current();
    p->addr = pg_round_down(addr);
    struct hash_elem* h_elem = hash_insert(p->thread->page_table,&p->hash_elem);
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
    if(h_elem != NULL){
        // Pointer to the new page.
        struct page* pg = hash_entry(h_elem,struct page,hash_elem);
        // Create a new zeroed page.
        lock_page(pg);
        if(page_in_core(pg)){
            pagedir_set_page(pg->thread->pagedir,pg->addr,pg->frame->base,true);
        }
        unlock_page(pg);

    }
    return true;
}

bool page_in_core(struct page* page){
    if(page->file == NULL && page->sector == NULL && page->frame == NULL){
        page->frame = get_free_frame();
        lock_acquire(&page->frame->f_lock);

        page->frame->page = page;
        lock_release(&page->frame->f_lock);
        }
    else if(page->file != NULL){
            //file_reopen(page->file);
            page->frame = get_free_frame();
            page->frame->page = page;
            if(page->frame != NULL){
                file_seek(page->file,page->file_offset);
                file_read(page->file,page->frame->base,page->file_bytes);
                memset(page->frame->base + page->file_bytes, 0, PGSIZE - page->file_bytes);
            }
            //file_close(page->file);
        }
    else if(page->sector != NULL){

        }
    else if (page->frame != NULL){
            
        }
    return true;
}
void lock_page(struct page* page){
    if(page->frame != NULL){
        lock_acquire(&page->frame->f_lock);
    }
}
void unlock_page(struct page* page){
    if(page->frame != NULL && lock_held_by_current_thread(&page->frame->f_lock)){
        lock_release(&page->frame->f_lock);
    }
}
