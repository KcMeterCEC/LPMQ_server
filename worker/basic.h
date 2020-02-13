#ifndef __BASIC_H__
#define __BASIC_H__

#include <cstdint>

typedef enum
{
    LPMQ_OK,
    LPMQ_NO_FILE,
    LPMQ_FILE_READ_ERR,
}cmd_status;

class Basic
{
public:
    Basic() = default;

    ~Basic();
protected:
    std::uint32_t file_read(const char *path, std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status);
private:
    
};
#endif