#include "cma_api.h"

#include "..\rpmalloc\rpmalloc.h"

size_t __stdcall MemTotalCommitted(void)
{
    return 0;
}

size_t __stdcall MemTotalReserved(void)
{
    return 0;
}

size_t __stdcall MemFlushCache(size_t size)
{
    return size;
}

void __stdcall MemFlushCacheAll(void)
{
    //rpmalloc_thread_collect(); // This function has not been implemented yet
}

size_t __stdcall MemSize(void* mem)
{
    return rpmalloc_usable_size(mem);
}

size_t __stdcall MemSizeA(void* mem, size_t /*align*/)
{
    return rpmalloc_usable_size(mem);
}

void* __stdcall MemAlloc(size_t size)
{
    return rpmalloc(size);
}

void* __stdcall MemAllocA(size_t size, size_t align)
{
    return rpaligned_alloc(align, size);
}

void __stdcall MemFree(void* mem)
{
    rpfree(mem);
}

void __stdcall MemFreeA(void* mem)
{
    rpfree(mem);
}

void __stdcall EnableHugePages(void)
{
}

BOOL WINAPI DllMain(HINSTANCE /*hInst*/, DWORD callReason, LPVOID /*lpvReserved*/)
{
    switch (callReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        rpmalloc_config_t config{};
        config.enable_huge_pages = 1; // Windows huge pages privilege has been handled by rpmalloc internally
        rpmalloc_initialize_config(&config);
        break;
    }
    case DLL_PROCESS_DETACH:
    {
        rpmalloc_finalize();
        break;
    }
    case DLL_THREAD_ATTACH:
    {
        rpmalloc_thread_initialize();
        break;
    }
    case DLL_THREAD_DETACH:
    {
        rpmalloc_thread_finalize(0);
        break;
    }
    default:
        break;
    }

    return TRUE;
}
