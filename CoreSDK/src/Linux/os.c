#include <os.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

static int _get_prot_flags(int prot){
    int flags = 0;
    if (prot & _OS_PAGE_PROT_READ) 
        flags |= PROT_READ;
    
    if (prot & _OS_PAGE_PROT_WRITE) 
        flags |= PROT_WRITE;
    
    if (prot & _OS_PAGE_PROT_EXEC) 
        flags |= PROT_EXEC;
        
    if (prot & _OS_PAGE_PROT_NONE)
        flags = PROT_NONE;
    return flags;
}

static uint32_t _linux_page_size(void) {
    return (uint32_t)sysconf(_SC_PAGESIZE);
}

static int _linux_mmap(size_t size, int prot, void** addr) {
    assert(addr != NULL);
    int flags = _get_prot_flags(prot);
    
    

    (*addr) = mmap(NULL, size, flags, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return ((*addr) == MAP_FAILED) ? -1 : 0;
}

static int _linux_unmmap(void* _addr, size_t len) {
    
    return munmap(_addr, len);
}

static int _linux_map_protect(void* _addr , size_t len , int prot){
    

    return mprotect(_addr , len , _get_prot_flags(prot));
}


_os_layer _current_os_layer = {
    .page_size = _linux_page_size,
    .mmap = _linux_mmap,
    .unmmap = _linux_unmmap , 
    .map_protect = _linux_map_protect
};


