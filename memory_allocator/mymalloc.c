// Katie Pucci (kev19)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mymalloc.h"

// Don't change or remove these constants.
#define MINIMUM_ALLOCATION  16
#define SIZE_MULTIPLE       8

typedef struct Block {
    unsigned int data_size;     // size of data portion
    unsigned int status;        // 1 = used, 0 = free
    struct Block* next;         // Link to next block
    struct Block* previous;     // Link to previous block
} Block;

Block* head;
Block* tail;

unsigned int round_up_size(unsigned int data_size)
{
	if(data_size == 0)
		return 0;
	else if(data_size < MINIMUM_ALLOCATION)
		return MINIMUM_ALLOCATION;
	else
		return (data_size + (SIZE_MULTIPLE - 1)) & ~(SIZE_MULTIPLE - 1);
}

Block* create_block(unsigned int size)
{
    Block* block = sbrk(size + sizeof(Block));  // block will point to memory location
    block->data_size = size;
    block->status = 1;
    block->next = NULL;
    block->previous = NULL;
    return block;
}

void* expand_heap(unsigned int size)
{
    Block* new_block = create_block(size);
    if(head == NULL)    // If first allocation
    {
        head = new_block;
        tail = head;
    }
    else    // Extend list
    {
        new_block->previous = tail;
        tail->next = new_block;
        tail = new_block;
        new_block = NULL;
    }

    return (void*)tail;
}

void contract_heap()
{
    // Get size of tail
    unsigned int tail_size = tail->data_size;
    if(tail->previous == NULL)  // If only one block allocated
    {
        head = NULL;
        tail = NULL;
    }
    else
    {
        tail->previous->next = NULL;    // Break first link between second to last block and the tail
        tail = tail->previous;          // Set tail to second to last block
        tail->next = NULL;              // Break final link between new tail and deleted block
    }

    sbrk(-(tail_size + sizeof(Block)));     // Move break
}

Block* best_fit(unsigned int size)
{
    Block* block = NULL;
    Block* best_fit = NULL;

    for(block = head; block != NULL; block = block->next)   // For each block in list
    {
        if(best_fit == NULL)
        {
            if(block->status == 0)
            {
                best_fit = block;
            }
        }
        else
        {
            if((block->status == 0) && (block->data_size >= size) && (block->data_size < best_fit->data_size))
            {
                best_fit = block;
            }
        }
    }

    return best_fit;
}

Block* coalesce(Block* block)
{
    Block* next_block = block->next;
    block->next->previous = NULL;
    block->next = NULL;

    block->data_size += next_block->data_size + sizeof(Block);

    if(next_block->next != NULL)
    {
        next_block->next->previous = block;
        block->next = next_block->next;
    }
    else if(block == head && head->next == NULL)
    {
        block = NULL;
        return head;
    }

    return block;
}

void split(Block* block, unsigned int size)
{
    unsigned int leftover_size = block->data_size - (size + sizeof(Block));
    block->data_size -= (leftover_size + sizeof(Block));
    Block* leftover = block + (sizeof(Block)) + (block->data_size);
    leftover->data_size = leftover_size;
    leftover->status = 0;
    leftover->next = NULL;
    leftover->previous = NULL;

    if((block->next != NULL))
    {
        block->next->previous = leftover;
        leftover->next = block->next;
    }
    block->next = leftover;
    leftover->previous = block;

    if(leftover->next != NULL && leftover->next->status == 0)
    {
        leftover = coalesce(leftover);
    }

}

void* my_malloc(unsigned int size)
{
	if(size == 0)
		return NULL;

	size = round_up_size(size);

	// ------- Don't remove anything above this line. -------
	// Here's where you'll put your code!

	// -Round up the size of the requested allocation to a certain size (I gave you this)
    // -Try to find a block to reuse, using best-fit allocation

    void* free_space = NULL;

    Block* best_fit_block = best_fit(size); // Find best-fit position
    if(best_fit_block != NULL)  // Block was found
    {
        best_fit_block->status = 1;

        if((best_fit_block->data_size - size) >= (MINIMUM_ALLOCATION + sizeof(Block)))
        {
            split(best_fit_block, size);
        }

        free_space = (void*)best_fit_block;
    }
    else    // Block wasn't found
    {
        free_space = expand_heap(size);
    }

	return free_space + sizeof(Block);
}

void coalescePrevious(Block* block)
{
    Block* prev_block = block->previous;
    block->previous->next = NULL;
    block->previous = NULL;

    block->data_size += prev_block->data_size + sizeof(Block);

    if(prev_block->previous != NULL)
    {
        prev_block->previous->next = block;
        block->previous = prev_block;
    }
}



void my_free(void* ptr)
{
	if(ptr == NULL)
		return;

	// -Mark the block as being free
	Block* block_to_free = (Block*)(ptr-sizeof(Block));

    block_to_free->status = 0;

    if((block_to_free != NULL) && (block_to_free->next != NULL))
    {
        if(block_to_free->next->status == 0)
        {
            block_to_free = coalesce(block_to_free);
        }
    }

    if((block_to_free != NULL) && (block_to_free->previous != NULL))
    {
        if(block_to_free->previous->status == 0)
        {
            block_to_free = coalesce(block_to_free->previous);
        }
    }

    if(block_to_free == tail)
    {
        contract_heap();
    }
    else if(block_to_free == head && head->next == NULL)    // If only one block allocated, and it's free
    {
        unsigned int head_size = head->data_size;
        block_to_free = NULL;
        head = NULL;
        sbrk(-(head_size + sizeof(Block)));
    }
}
