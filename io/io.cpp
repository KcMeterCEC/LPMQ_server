#include "io.h"
#include "../debug.h"

Io::~Io()
{
    
}
std::uint32_t Io::exec(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status)
{
    LOG_DBG("executing io command...");

    return 0;
}