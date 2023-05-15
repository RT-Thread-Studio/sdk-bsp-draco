#include <rtthread.h>
#include <stdlib.h>

size_t get_free_heap_size(void)
{
    rt_uint32_t total, used, max;

    rt_memory_info(&total, &used, &max);

    return total - used;
}

#if (RTTHREAD_VERSION >= RT_VERSION_CHECK(4, 1, 0))
extern int cmd_free(int argc, char **argv);
void list_mem(void)
{
    cmd_free(0, 0);
}
#endif
