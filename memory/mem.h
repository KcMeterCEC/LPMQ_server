#ifndef __MEM_H__
#define __MEM_H__
#include <cstdint>

#include "../worker/basic.h"

typedef enum
{
    MEM_STAT,
}mem_cmd;

class Mem : public Basic
{
public:    
    Mem() = default;

    ~Mem();

    uint32_t exec(uint8_t *buf, uint32_t maximum_len, int16_t &status);
private:    
    const char *path_memstat = "/proc/meminfo";

    uint32_t exec_memstat(uint8_t *buf, uint32_t maximum_len, int16_t &status);
};
#endif

