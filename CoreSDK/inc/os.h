#include <sdk_macrosz.h>
#ifndef __OS_H__
#define __OS_H__
#ifdef __cplusplus
extern "C" {
#endif
    #include <stdint.h>
    #include <stddef.h>
    #include <assert.h>
    /** @brief Enumeration of page protection flags . 
     *  @def _OS_PAGE_PROT_NONE No access to the page .
     *  @def _OS_PAGE_PROT_READ Read access to the page .
     *  @def _OS_PAGE_PROT_WRITE Write access to the page .
     *  @def _OS_PAGE_PROT_EXEC Execute access to the page .
     * @note These flags can be combined using bitwise OR to specify multiple protections for a page .
     * 
    */
    enum _os_page_prot{
        _OS_PAGE_PROT_NONE = 0x0 ,
        _OS_PAGE_PROT_READ = 0x1 ,
        _OS_PAGE_PROT_WRITE = 0x2 ,
        _OS_PAGE_PROT_EXEC = 0x4
    };
    typedef enum _os_page_prot _os_page_prot;

    /**
     * @brief This struct is system struct and change it by "hand" can create
     * Undefined behaviors . 
     * @struct _os_layer Contains function pointers to the OS layer functions .  
     */
    struct _os_layer{
        uint32_t (*page_size)(void);

        int (*mmap)(size_t size , int prot , void** addr);

        int (*unmmap)(void* _addr , size_t len);

        int (*map_protect)(void* _addr , size_t len , int prot);

    };
    typedef struct _os_layer _os_layer;


    extern SDK_API _os_layer _current_os_layer;

    /** @brief Map a memory region .
     * 
     * @param[in]  size The size of the memory region to map . Truncated to page size .
     * @param[in]  prot The protection flags for the memory region .
     * @param[out] addr The return address of the mapped region .
     * @return int 0 on success , -1 on failure .
     */
    #define _aoshll_mmap(size , prot , addr) _current_os_layer.mmap(size , prot , addr)


    /** @brief Get the page size .
     * 
     * @return uint32_t The page size .
     */
    #define _aoshll_page_size() _current_os_layer.page_size()

    /**
     * @brief Unmap a already mapped pages
     * @param[in] addr The address of the mapped region to unmap .
     * @param[in] len The length of the mapped region to unmap .
     * @return int 0 on success , -1 on failure .
     * 
     */
    #define _aoshll_unmmap(addr , len) _current_os_layer.unmmap(addr , len)

    /**
     * @brief Change the protection of a mapped region .
     * @param[in] addr The address of the mapped region to change protection .
     * @param[in] len The length of the mapped region.
     * @param[in] prot The new protection flags for the mapped region .
     * 
     */
    #define _aoshll_mprotect(addr , len , prot) _current_os_layer.map_protect(addr , len , prot)

#ifdef __cplusplus
}
#endif
#endif