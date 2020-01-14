#ifndef __RB_H__
#define __RB_H__
#include <cstdint>
#include <vector>

class Rb
{
public:    
    Rb(std::uint32_t size);

    ~Rb();

    bool write(const std::uint8_t *wbuf, std::uint32_t size);
    bool read(std::uint8_t *rbuf, std::uint32_t size);
private:
    std::uint32_t write_index = 0;
    std::uint32_t read_index = 0;
    std::uint32_t empty_size = 0;
    std::vector<std::uint8_t> buf;
};
#endif