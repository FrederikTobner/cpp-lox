# Memory Management

This class will provide a garbage collector and memory mutator.
To make sure the garbage collector can move objects around, a mutex is used to lock the memory manager.
Every Value that is stored on the stack needs to be updated when the garbage collector moves an object. To make sure
does not introduce any issues all accesses to the objects should go through the memory manager or we use a shared
lock, that is used by the memory manager and the vm. Then we add a access object method in the vm that checks the
lock but does not lock it. Another possiblity to solve this problem would be to use a pointer to a pointer. This
would require more memory and would be slower.
