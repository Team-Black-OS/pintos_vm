#ifndef _FRAME_H
#define _FRAME_H

#include "threads/synch.h"
#include "vm/page.h"

// Structure to represent a frame in memory.
struct frame{
    struct lock f_lock;
    void* base;
    struct page *page;
};

#endif