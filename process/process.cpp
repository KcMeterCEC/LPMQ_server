#include "process.h"
#include "../debug.h"

Process::~Process()
{
    
}
std::uint32_t Process::exec(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status)
{
    LOG_DBG("executing process command...");

    return 0;
}
const char * Process::cpuinfo_path_get(void)
{
    return path_cpuinfo;
}