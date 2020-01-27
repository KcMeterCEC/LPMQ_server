#ifndef __MEM_H__
#define __MEM_H__
#include <cstdint>

typedef enum
{
    MEM_STAT,
}mem_cmd;

class Mem
{
public:    
    Mem() = default;

    ~Mem();

    std::uint32_t exec(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status);
private:    

};
#endif

