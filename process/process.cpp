#include <cstdio>

#include "process.h"
#include "../debug.h"

Process::~Process()
{
    
}
std::uint32_t Process::exec(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status)
{
    LOG_DBG("executing process command...");
    std::uint32_t len = 0;
    switch(buf[0])
    {
        case CPU_STAT:
        {
            len = exec_cpustat(buf, maximum_len, status);
            LOG_DBG("read cpu stat from proc, return len is " << len);
        }break;
        default:
        {
            LOG_ERR("unknown PS command!");
        }break;
    }

    return len;
}
const char * Process::cpuinfo_path_get(void)
{
    return path_cpuinfo;
}
std::uint32_t Process::exec_cpustat(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status)
{
    std::uint32_t len = file_read(path_cpustat, buf + 1, maximum_len, status);
    return len ? len + 1 : 0;
}