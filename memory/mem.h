#ifndef __MEM_H__
#define __MEM_H__
#include <cstdint>

class Mem
{
public:    
    Mem() = default;

    ~Mem();

    std::uint32_t exec(std::uint8_t *buf, std::uint32_t maximum_len);
private:    

};
#endif

