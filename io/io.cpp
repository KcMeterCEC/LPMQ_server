#include "io.h"
#include "../debug.h"

Io::~Io()
{
    
}
uint32_t Io::exec(uint8_t *buf, uint32_t maximum_len, int16_t &status) const
{
    uint32_t len = 0;
    switch(buf[0])
    {
        case IO_STAT:
        {
            len = exec_iostat(buf + 1, maximum_len - 1, status);
        }break;
        default:
        {
            LOG_ERR("unknown IO command!");
        }break;
    }

    return len ? len + 1 : 0;
}
uint32_t Io::exec_iostat(uint8_t *buf, uint32_t maximum_len, int16_t &status) const
{
    return file_read(path_iostat, buf, maximum_len, status);
}