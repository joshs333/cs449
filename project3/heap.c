#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

struct Block {
  int occ;              // whether block is occupied
  int size;             // size of block (including header)
  struct Block *prev;   // pointer to previous block
  struct Block *next;   // pointer to next block
};

static struct Block *head = NULL;       // head of list

/** @brief Bump up program break by the allocation size
 *
 *  Monotonically increase the heap at every allocation.
 *  In this heap, there are no data structures to mark the
 *  used and unused portions of the heap.
 *
 *  @param size Allocation size in bytes.
 *  @return Pointer to the allocated block of memory.
 */
void *my_malloc1(int size)
{
  return sbrk(size);
}

/** @brief Bump up program break by the allocation size + block header size
 *
 *  Same as malloc1 except that now you also allocate space
 *  for the block header.  The structure of the block header
 *  is given in the struct Block type.  The function should
 *  return the location after the header, which is the usable
 *  part of the block.
 *
 *  @param size Allocation size in bytes.
 *  @return Pointer to the usable part of allocated memory.
 */
void *my_malloc2(int size)
{
  // allocate the requested size and the size of the block
  // add the size of the Block header to give the user the
  // usable space and keep the block ahead of this.
  return sbrk(size + sizeof(struct Block)) + sizeof(struct Block);
}

/** @brief Bump up program break by the allocation size + block header size
 *         and fill in the block header fields
 *
 *  Now blocks are added to the double-linked list which starts
 *  from 'head'.  When a new block is added, the occ bit needs to
 *  be set to 1, size needs to be set to the block size, and the
 *  link pointers need to be updated such that it is double-linked
 *  to the previous block.
 *
 *  @param size Allocation size in bytes.
 *  @return Pointer to the usable part of allocated memory.
 */
void *my_malloc3(int size)
{
    // find free space in the existing heap
    struct Block *new_block = NULL;
    int block_size = size + sizeof(struct Block);
    // if there are no nodes, define an empty node of the perfect size at the begining
    if(head == NULL) {
        head = sbrk(block_size);
        head->size = block_size;
        head->occ = 0;
        head->prev = NULL;
        head->next = NULL;
    }
    struct Block *block_it = head;
    while(block_it) {
        if(block_it->occ == 0) {
            // if we have a currently picked block
            // we need it to be greater than the blcok size and more space efficeint
            // than the currently selected
            if(new_block && block_it->size > block_size && block_it->size < new_block->size) {
                new_block = block_it;
            // if we don't have a currently selected block and this fits
            // we can select it
            } else if(new_block == NULL && block_it->size >= block_size) {
                new_block = block_it;
            }
        }
        // we are at the end of the heap linked list
        // or we have a perfectly sized node selected already
        if(block_it->next == NULL || (new_block && new_block->size == block_size)) {
            // if we don't have a block selected we need to allocate one 
            if(new_block == NULL) {
                new_block = sbrk(block_size);
                new_block->next = NULL;
                new_block->prev = block_it;
                new_block->size = block_size;
                new_block->occ = 1;
                block_it->next = new_block;
                return new_block + 1;
            // if we have the perfect size we are more or less good to go
            } else if(new_block->size == block_size) {
                new_block->occ = 1;
                return new_block + 1;
            // if we have a block greater than the needed size then we need to split it
            } else if(new_block->size > block_size) {
                struct Block* split = (struct Block*)((void*)new_block + block_size);
                split->prev = new_block;
                split->next = new_block->next;
                new_block->next = split;
                // we would normally do a sanity check to see if the tailing block is NULL (end)
                // but that would require new_block to be on the end and empty
                // and any empty nodes on the end get removed so the tailing block is never NULL
                // if the current node is unoccupied
                split->next->prev = split;
                new_block->occ = 1;
                return new_block + 1;
            } else {
                printf("ERROR! We should either have a block of greater or equal size selected or new_block should be NULL!");
            }
        }
        block_it = block_it->next;
    }
    printf("ERROR! End of function reached!");
    return head;
    /*
    // old code
    struct Block* block = sbrk(size + sizeof(struct Block));
  
  // basic block data
  (*block).occ = 1;
  (*block).size = size + sizeof(struct Block);
  
  // linked list handling
  // handle first Node case
  if(head == NULL) {
    head = block;
    (*block).next = NULL;
    (*block).prev = NULL;
  } else {
    // handle append block case
    // find last block
    struct Block* block_it = head;
    while((*block_it).next != NULL) {
      block_it = (*block_it).next;
    }
    // set the next block in the linked list to this
    (*block_it).next = block;
    (*block).prev = block_it;
    // set next block to NULL to be safe
    (*block).next = NULL;
  }
  // return pointer to usable memory
  return block + 1;
  */
}

/** @brief Mark the block free by unsetting the occ bit
 *
 *  In a real heap the freed block would be reused but
 *  this simple heap always allocats from the top of the
 *  heap by increasing heap space.
 *
 *  @param data Pointer to area being freed by user.
 *  @return Void.
 */
void my_free(void *data)
{
    // find the block header
    struct Block* cur = data - sizeof(struct Block);
    cur->occ = 0;

    // coalesce empty blocks above the current
    // only do if there is a next block and it is not occupied
    if(cur->next != NULL && cur->next->occ == 0) {
        struct Block* next_block = cur->next;
        cur->next = next_block->next;
        cur->size += next_block->size;
        // if the following block exists, point it's previous to this
        if(next_block->next)
            next_block->next->prev = cur;
    }
    // coalesce into the previous if it exists and is not occupied
    if(cur->prev != NULL && cur->prev->occ == 0) {
        struct Block* next_block = cur;
        cur = cur->prev;
        cur->next = next_block->next;
        cur->size += next_block->size;
        // if the next block exists point it's previous to the previous
        if(next_block->next)
            next_block->next->prev = cur;
    }
    // if the next block is null, decrement sbrk
    if(cur->next == NULL) {
        if(cur->prev)
            cur->prev->next = NULL;
        else
            head = NULL;
        sbrk(-cur->size);
    }
}

/** @brief Dump the contents of the heap.
 *
 *  Traverse the heap starting from the head of the list and print
 *  each block.  While traversing, check the integrity of the heap
 *  through various assertions.
 *
 *  @return Void.
 */
void dump_heap()
{
  struct Block *cur;
  // printf("brk: %p\n", sbrk(0));
  printf("head->");
  for(cur = head; cur != NULL; cur = cur->next) {
    printf("[%d:%ld:%d]->", cur->occ, (char*)cur - (char*)head, cur->size);
    fflush(stdout);
    assert((char*)cur >= (char*)head && (char*)cur + cur->size <= (char*)sbrk(0)); // check that block is within bounds of the heap
    if(cur->next != NULL) {
      assert(cur->next->prev == cur); // if not last block, check that forward/backward links are consistent
      assert((char*)cur + cur->size == (char*)cur->next); // check that the block size is correctly set
    }
  }
  printf("NULL\n");
}
