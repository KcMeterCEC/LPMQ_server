#include <stdio.h>
#include "basic.h"
#include "../debug.h"

Basic::~Basic()
{

}

std::uint32_t Basic::file_read(const char *path, std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status)
{
    FILE *fp = std::fopen(path, "r");

    if(!fp)
    {
        LOG_ERR("can't open file " << path);
        status = -LPMQ_NO_FILE;

        return 0;
    }
    std::uint32_t r_size = std::fread(buf, 1, maximum_len, fp);
    std::fclose(fp);

    status = LPMQ_OK;

    return r_size;
}