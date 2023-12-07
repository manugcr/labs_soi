/*  Memory management
 *  - brk() is used to set the end of a data segment of a specified value.
 *      It takes a single argument, the address of the new end of the data segment.
 *      On success, brk() returns zero. On error, -1 is returned, the global errno is set to indicate the error.
 *  - sbrk() increments the programs data space (heap) by increment bytes.
 *      Depending on the system, it returns either the previous break or the new break. But we wont use the returned value.
 *      A special case is when increment is 0, which returns the current break without changing it.
 *      We will use sbrk() as our main tool to implement malloc(). All we have to do is acquire more space to fullfil the query.
 * 
 *  Some applications of malloc() use mmap() to allocate memory. mmap() is a system call that maps files or devices into memory.
 *  Its more used for large allocations (more than one page). 
 */

#include "memory.h"

void    *base   = NULL;
int     method  = 0;

/*  
 *  Funciones pedidas por la consigna
 *      1- malloc 
 *      2- free
 *      3- calloc
 *      4- realloc
 *      5- check_heap
 *      6- malloc_control
 * 
 */

void    *malloc(size_t size)
{
    t_block b;
    t_block last;
    size_t s;

    s = align(size);

    if (!s)
        return NULL;

    if (base)
    {
        /* First find a block */
        last = base;
        b = find_block(&last, s);
        if (b)
        {
            /* Can we split */
            if ((b->size - s) >= (BLOCK_SIZE + 4))
                split_block(b, s);

            b->free = 0;
        }
        else
        {
            /* No fitting block, extend the heap */
            b = extend_heap(last, s);
            if (!b)
                return NULL;
        }
    }
    else
    {
        /* First time */
        b = extend_heap(NULL, s);
        if (!b)
            return NULL;
        base = b;
    }

    return b->data;
}

void free(void *p)
{
    t_block b;
    if (valid_addr(p))
    {
        b = get_block(p);
        b->free = 1;
        /* fusion with previous if possible */
        if (b->prev && b->prev->free)
            b = fusion(b->prev);
        /* then fusion with next */
        if (b->next)
            fusion(b);
        else
        {
            /* free the end of the heap */
            if (b->prev)
                b->prev->next = NULL;
            else
                /* No more block !*/
                base = NULL;
            brk(b);
        }
    }
}

/*  calloc implementation
 *      1- Allocate memory for number elements of size bytes each, and returns a pointer to the allocated memory.
 *      2- The memory is set to zero. If number or size is zero, then calloc() returns either NULL, or a unique pointer value that can later be successfully passed to free().
 * 
 */
void    *calloc(size_t number, size_t size)
{
	size_t *new;

	if(!(number || size))
		return NULL;

	new = malloc(number *size);
	if (new)
	{
		size_t s4 = align(number *size) >> 2;
		for (size_t i = 0; i < s4; i++)
			new[i] = 0;
		
	}

	return (new);
}

/*  realloc implementation 
 *      1- If the pointer is NULL, then we have to allocate a new memory (malloc).
 *      2- If the size is equal to zero, then we have to free the memory (free).
 *      3- If the pointer is not NULL, then we have to reallocate the memory (realloc).
 * 
 */
void    *realloc(void *p, size_t size)
{
    size_t      s;
    t_block     b, new;
    void        *newp;

    if (!p)
        return malloc(size);
    
    if (valid_addr(p))
    {
        s = align(size);
        b = get_block(p);
        if (b->size >= s)
        {
            if (b->size - s >= (BLOCK_SIZE + 4))
                split_block(b, s);
        }
        else
        {
            /* Try fusion with next if possible */
            if (b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
            {
                fusion(b);
                if (b->size - s >= (BLOCK_SIZE + 4))
                    split_block(b, s);
            }
            else
            {
                /* no free block of sufficient space, we malloc then free */
                newp = malloc(s);

                if (!newp)
                    return NULL;
                
                new = get_block(newp);
                /* Copy data */
                copy_block(b, new);
                /* free the old one */
                free(p);
                return (newp);
            }
        }
        return (p);
    }
    return NULL;
}

void check_heap(void *data)
{
	t_block block = get_block(data);
	printf("-----------------------------------------------------------------\n");
    printf("HEAP CHECK.\n");
    printf("Size: %zu\n", block->size);
    printf("Next block: %p\n", (void *)(block->next));
    printf("Prev block: %p\n", (void *)(block->prev));
    printf("Free: %d\n", block->free);
    printf("Beginning data address: %p\n", block->ptr);
    printf("Last data address: %p\n", (void *)((char *)(block->ptr) + block->size));
    printf("Heap address: %p\n", sbrk(0));
	printf("-----------------------------------------------------------------\n\n");	
}

/* Choose method between FIRST_FIT (default) or BEST_FIT. */
void    malloc_control(int OPTION)
{
    if (OPTION == FIRST_FIT)
        method = OPTION;
    else if (OPTION == BEST_FIT)  
        method = OPTION;
    else
        printf("INVALID OPTION: try BEST_FIT or FIRST_FIT. \n");
}




/*  
 *  Funciones agregadas para el funcionamiento del programa
 * 
 */

t_block get_block(void *p)
{
    char *tmp   = NULL;
    tmp         = p;

    return (t_block)(tmp -= BLOCK_SIZE);
}

int valid_addr(void *p)
{
	if (base)
	{
		if (p > base && p < sbrk(0))
			return (p == &((get_block(p))->data));
	}

	return (0);
}

t_block find_block(t_block *last, size_t size){
	t_block b = base;
	
    /* FIRS_FIT method */
	if (method == FIRST_FIT)
	{
		while (b && !(b->free && b->size >= size)){
			*last = b;
			b = b->next;
		}
		return b;
	}
	/* BEST_FIT method */ 
	else
	{
		size_t diference = PAGESIZE;
		t_block best = NULL;
		while (b)
		{
			if (b->free)
			{
				if (b->size == size)
					return (b);

				if (b->size > size)
				{
					if ((b->size - size) < diference)
					{
						diference = b->size - size;
						best = b;
					}
				}
			}
			*last = b;
			b = b->next;
		}
		return best;
	}
}

t_block extend_heap(t_block last, size_t s)
{
	t_block b;

	b = sbrk(0);

	if (sbrk(BLOCK_SIZE + (long int)s) == (void*)-1)
		return (NULL);

	b->ptr  = b->data; 
	b->size = s;
	b->prev = last;
	b->next = NULL;
	
    if (last)
		last->next = b;

	b->free = 0;
	return (b);
}

void split_block(t_block b, size_t s)
{
	t_block     new;
	
    new         = (t_block)(b->data + s);
	new->size   = b->size - s - BLOCK_SIZE;
	new->next   = b->next;
	new->prev   = b;
	new->free   = 1;
	new->ptr    = new->data;
	b->size     = s;
	b->next     = new;

	if (new->next)
		new->next->prev = new;

}

t_block fusion(t_block b)
{
    if (b->next && b->next->free)
    {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if (b->next)
            b->next->prev = b;
    }

    return b;
}


void copy_block(t_block src, t_block dst)
{
	int *sdata, *ddata;
	size_t i;

	sdata = src->ptr;
	ddata = dst->ptr;
	
    for (i = 0; i*4 < src->size && i*4 < dst->size; i++)
		ddata[i] = sdata[i];
}