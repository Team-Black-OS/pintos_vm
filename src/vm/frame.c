#include "vm/frame.h"

// Not sure if we need this or not.
#define FRAME_ARR_SIZE 1

static int frame_ct;
static struct frame frames[FRAME_ARR_SIZE];
static struct lock scan_lock;
static int hand;
