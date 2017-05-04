#include "vm/frame.h"
#include "threads/init.h"
#include "threads/palloc.h"
#include "threads/loader.h"
#include <stdio.h>
// Not sure if we need this or not.
#define FRAME_ARR_SIZE 500

static int frame_ct;
static struct frame* frames[FRAME_ARR_SIZE];
static struct lock scan_lock;
static int hand;
static int used_pages = 0;

// Accepts a page pointer, then allocates a frame for this page.
struct frame* get_free_frame(struct page* page){
    // Get the scan lock.
    lock_acquire(&scan_lock);
    // Set the current thread.
    struct thread* t = thread_current();
    for(int i = 0; i < used_pages; ++i){
        // If this frame is available.
        if(frames[i]->page == NULL){
            // Set the page pointer.
            frames[i]->page = page;
            // Release the scan lock.
            lock_release(&scan_lock); 
            return frames[i];
        }
    }
    // Release the scan lock.
    lock_release(&scan_lock);
    // Panic if there are no free pages.
    PANIC("Please help me.");
    NORETURN();
}

// Function to initialize user memory (called from init.c)
void init_user_mem(){
    // Initialize the scan lock.
    lock_init(&scan_lock);
    // Acquire the scan lock.
    lock_acquire(&scan_lock);
    for(int i = 0; i < FRAME_ARR_SIZE; ++i){
        // Allocate until the max size is met, or palloc_get_page returns NULL.
        void* page = palloc_get_page(PAL_USER | PAL_ZERO);
        if(page == NULL){
            break;
        }else{
            // Allocate a struct frame for this entry.
            frames[i] = (struct frame*) malloc(sizeof(struct frame));
            // Initialize the lock for this entry.
            lock_init(&frames[i]->f_lock);
            // Set the page for this frame to the page we allocated.
            frames[i]->base = page;
            // Set the page pointer to NULL (indicates a free frame).
            frames[i]->page = NULL;
            // Increment the used pages.
            ++used_pages;
        }
    }
    // Release the scan lock.
    lock_release(&scan_lock);
}
