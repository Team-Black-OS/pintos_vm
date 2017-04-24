#ifndef _PAGE_H
#define _PAGE_H

#include "vm/frame.h"
#include <hash.h>
#include "threads/thread.h"
#include "filesys/file.h"
#include "devices/block.h"

// Structure to represent a virtual address page.
struct page{

void* addr;
bool read_only;
struct thread *thread;

struct hash_elem hash_elem;

struct frame *frame;

block_sector_t sector;

bool private;

struct file *file;
off_t file_offset;
off_t file_bytes;

};

// Hash function for pages.
unsigned page_hash(const struct hash_elem *e, void* aux);

// Comparison function for pages.
bool page_less(const struct hash_elem *a, const struct hash_elem *b, void* aux);

#endif