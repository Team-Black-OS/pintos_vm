#include "vm/frame.h"
#include "threads/init.h"
#include "threads/palloc.h"
#include "threads/loader.h"
#include <stdio.h>
// Not sure if we need this or not.
#define FRAME_ARR_SIZE 1

static int frame_ct;
static struct frame frames[FRAME_ARR_SIZE];
static struct lock scan_lock;
static int hand;

struct frame get_free_frame(){
    struct thread* t = thread_current();
    for(int i = 0; i < FRAME_ARR_SIZE; ++i){
        if(frames[i].page == NULL){
           return frames[i];
        }
    }
    PANIC("Please help me.");
    NORETURN();
}

void init_user_mem(){
    for(int i = 0; i < FRAME_ARR_SIZE; ++i){
        void* page = palloc_get_page(PAL_USER);
        if(page == NULL){
            break;
        }else{
            frames[i].base = page;
        }
    }
}
