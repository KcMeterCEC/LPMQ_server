#ifndef __IO_H__
#define __IO_H__
#include <cstdint>

class Io
{
public:    
    Io() = default;

    ~Io();

    std::uint32_t exec(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status);
private:    

};
#endif

