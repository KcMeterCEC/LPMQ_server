#ifndef __IO_H__
#define __IO_H__
#include "../worker/basic.h"

class Io : public Basic
{
public:    
    Io() = default;

    ~Io();

    uint32_t exec(uint8_t *buf, uint32_t maximum_len, int16_t &status) const;
private:    
    typedef enum
    {
        IO_STAT,
    }io_cmd;

    const char *path_iostat = "/proc/diskstats";

    uint32_t exec_iostat(uint8_t *buf, uint32_t maximum_len, int16_t &status) const;
};
#endif

