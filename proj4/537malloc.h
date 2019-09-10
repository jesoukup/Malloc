/* Joseph Soukup
	Steven Mulvey */

#include <stdint.h>
typedef struct tuple {
	uintptr_t addr;
	size_t len;
} tuple;

void *malloc537(size_t size);

void free537(void *ptr);

void *realloc537(void *ptr, size_t size);

void memcheck537(void *ptr, size_t size);

struct Node *search537(uintptr_t *addr);

int main();

