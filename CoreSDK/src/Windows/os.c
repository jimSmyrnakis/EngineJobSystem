#include <os.h>
#include <windows.h>

static uint32_t _os_page_size(void) {
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    return (uint32_t)system_info.dwPageSize;
}

static int _os_mmap(size_t size, _os_page_prot prot, void** addr) {
    assert(addr != NULL);
    DWORD protection = 0;

    if (prot == _OS_PAGE_PROT_NONE) {
        protection = PAGE_NOACCESS;
    } else if (prot == _OS_PAGE_PROT_READ) {
        protection = PAGE_READONLY;
    } else if (prot == (_OS_PAGE_PROT_READ | _OS_PAGE_PROT_WRITE)) {
        protection = PAGE_READWRITE;
    } else if (prot == _OS_PAGE_PROT_EXEC) {
        protection = PAGE_EXECUTE;
    } else if (prot == (_OS_PAGE_PROT_READ | _OS_PAGE_PROT_EXEC)) {
        protection = PAGE_EXECUTE_READ;
    } else if (prot == (_OS_PAGE_PROT_READ | _OS_PAGE_PROT_WRITE | _OS_PAGE_PROT_EXEC)) {
        protection = PAGE_EXECUTE_READWRITE;
    } else {
        protection = PAGE_READWRITE;
    }

    (*addr) = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, protection);
    return ((*addr) == NULL) ? -1 : 0;
}

_os_layer _current_os_layer = {
    .page_size = _os_page_size,
    .mmap = _os_mmap
};
