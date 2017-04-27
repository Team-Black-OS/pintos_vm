#include "vm/swap.h"
#include "devices/block.h"
#include <bitmap.h>
#include "threads/synch.h"
#include "threads/vaddr.h"

#define PAGE_SECTORS PGSIZE/BLOCK_SECTOR_SIZE
static struct block* global_swap;
static struct bitmap global_bitmap;
static struct lock swap_lock;



