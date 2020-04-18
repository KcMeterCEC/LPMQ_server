#ifndef __IO_H__
#define __IO_H__
#include "../worker/basic.h"

class Io
{
public:    
    Io() = default;

    ~Io();

    uint32_t exec(uint8_t *buf, uint32_t maximum_len, int16_t &status) const;
private:    

};
#endif

