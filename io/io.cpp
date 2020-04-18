#include "io.h"
#include "../debug.h"

Io::~Io()
{
    
}
uint32_t Io::exec(uint8_t *buf, uint32_t maximum_len, int16_t &status) const
{
    LOG_DBG("executing io command...");

    return 0;
}