#include "mem.h"
#include "../debug.h"

Mem::~Mem()
{
    
}
std::uint32_t Mem::exec(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status)
{
    std::uint32_t len = 0;
    switch(buf[0])
    {
        case MEM_STAT:
        {
            len = exec_memstat(buf, maximum_len, status);
        }break;
        default:
        {
            LOG_ERR("unknown PS command!");
        }break;
    }

    return len;
}
std::uint32_t Mem::exec_memstat(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status)
{
    std::uint32_t len = file_read(path_memstat, buf + 1, maximum_len, status);
    return len ? len + 1 : 0;
}