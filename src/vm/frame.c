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
struct frame* get_free_frame(struct page* page){
    lock_acquire(&scan_lock);
    struct thread* t = thread_current();
    for(int i = 0; i < used_pages; ++i){
        if(frames[i]->page == NULL){
            frames[i]->page = page;
           lock_release(&scan_lock); 
           return frames[i];
        }
    }
    lock_release(&scan_lock);
    PANIC("Please help me.");
    NORETURN();
}

void init_user_mem(){
    lock_init(&scan_lock);
    lock_acquire(&scan_lock);
    for(int i = 0; i < FRAME_ARR_SIZE; ++i){
        void* page = palloc_get_page(PAL_USER | PAL_ZERO);
        if(page == NULL){
            break;
        }else{
            frames[i] = (struct frame*) malloc(sizeof(struct frame));
            lock_init(&frames[i]->f_lock);
            frames[i]->base = page;
            frames[i]->page = NULL;
            ++used_pages;
        }
    }
    lock_release(&scan_lock);
    printf("%d User Pages Allocated.\n",used_pages);
}

void lock_frame(struct frame* frame){

}
