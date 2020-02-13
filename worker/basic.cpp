#include <fstream>
#include "basic.h"
#include "../debug.h"

Basic::~Basic()
{

}

std::uint32_t Basic::file_read(const char *path, std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status)
{
    std::uint32_t r_size = 0;

    std::ifstream file(path);
    if(file)
    {
        if(file.read(reinterpret_cast<char*>(buf), maximum_len) || file.eof())
        {
            status = LPMQ_OK;
            r_size = file.gcount();
        }
        else
        {
            LOG_ERR("read file failed " << path);
            status = -LPMQ_FILE_READ_ERR;
        }
        
    }
    else
    {
        LOG_ERR("can't open file " << path);
        status = -LPMQ_NO_FILE;
    }

    return r_size;
}