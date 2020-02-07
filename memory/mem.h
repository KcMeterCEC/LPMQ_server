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

    std::uint32_t exec(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status);
private:    
    const char *path_memstat = "/proc/meminfo";

    std::uint32_t exec_memstat(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status);
};
#endif

