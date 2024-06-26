/**
 * @author Ali Mirmohammad
 * @file heap.c
 ** This file is part of AliNix.

**AliNix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**AliNix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with AliNix. If not, see <https://www.gnu.org/licenses/>.
*/
/**
 * @abstraction:
 *  - Define the heap memory section.
*/
#include <alinix/heap.h>
#include <alinix/memory.h>
#include <alinix/module.h>

MODULE_AUTHOR("Ali Mirmohammad")
MODULE_DESCRIPTION("Heap Memory Section")
MODULE_LICENSE("AGPL-3.0")
MODULE_VERSION("1.0")


//? Use Malloc in heap.h or  memory.h ???

bool CheckForErrors()
{
    /*Keep here empty*/
}

/**
 * Rounds up an address to the next page boundary (4096-byte boundary).
 *
 * @param address The address to be rounded up.
 * @return The rounded up address.
 */
uint32_t pageRoundUp(uint32_t address)
{
    if((address & 0xFFFFF000) != address)
    {
        address &= 0xFFFFF000;
        address += 0x1000;
    }
    return address;
}

/**
 * Rounds down an address to the previous page boundary (4096-byte boundary).
 *
 * @param address The address to be rounded down.
 * @return The rounded down address.
 */
uint32_t pageRoundDown(uint32_t address)
{
    return address & 0xFFFFF000;
}


/**
 * Frees memory allocated by alignedAlloc.
 *
 * @param ptr Pointer to the memory block to be freed.
 */
void allignedFree(void* ptr)
{
    if(ptr == 0)
        return;

    uint16_t offset = *((uint16_t*)ptr - 1);

    void* p = (void*)((uint8_t*)ptr - offset);
    // TODO : Implement freeing of memory pages
    Free(p);
}


/**
 * Allocates memory with a specified alignment.
 *
 * @param size  The size of the memory block to allocate.
 * @param align The alignment requirement (must be a power of two).
 * @return A pointer to the allocated memory block, or NULL if allocation fails.
 */
void* alignedMalloc(uint32_t size, uint32_t align)
{
    void* ptr = 0;

    if(!(align & (align - 1)) == 0)
        return 0;

    if(align && size)
    {
        uint32_t hdr_size = sizeof(uint16_t) + (align - 1);

        void* p = malloc(size + hdr_size);

        if(p)
        {
            ptr = (void *) align_up(((uintptr_t)p + sizeof(uint16_t)), align);

            *((uint16_t*)ptr - 1) = (uint16_t)((uintptr_t)ptr - (uintptr_t)p);
        }
    }
    return ptr;
}

/**
 * Frees a memory block previously allocated by the memory allocator.
 *
 * @param ptr Pointer to the memory block to be freed.
 */
void Free(void* ptr)
{
    struct MemoryHeader* chunk = (struct MemoryHeader*)((uint32_t)ptr - sizeof(struct MemoryHeader));
    
    chunk -> allocated = false;
    
    if(chunk->prev != 0 && !chunk->prev->allocated)
    {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(struct MemoryHeader);
        if(chunk->next != 0)
            chunk->next->prev = chunk->prev;
        
        chunk = chunk->prev;
    }
    
    if(chunk->next != 0 && !chunk->next->allocated)
    {
        chunk->size += chunk->next->size + sizeof(struct MemoryHeader);
        chunk->next = chunk->next->next;
        if(chunk->next != 0)
            chunk->next->prev = chunk;
    }
}