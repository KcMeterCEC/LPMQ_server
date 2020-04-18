#include "mem.h"
#include "../debug.h"

Mem::~Mem()
{
    
}
uint32_t Mem::exec(uint8_t *buf, uint32_t maximum_len, int16_t &status) const
{
    uint32_t len = 0;
    switch(buf[0])
    {
        case MEM_STAT:
        {
            len = exec_memstat(buf + 1, maximum_len - 1, status);
        }break;
        default:
        {
            LOG_ERR("unknown PS command!");
        }break;
    }

    return len ? len + 1 : 0;
}
uint32_t Mem::exec_memstat(uint8_t *buf, uint32_t maximum_len, int16_t &status) const
{
    return file_read(path_memstat, buf, maximum_len, status);
}