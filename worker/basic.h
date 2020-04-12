#ifndef __BASIC_H__
#define __BASIC_H__

#include <cstdint>

using std::uint8_t;
using std::int8_t;
using std::uint16_t;
using std::int16_t;
using std::uint32_t;
using std::int32_t;
using std::uint64_t;
using std::int64_t;

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

    virtual ~Basic();
protected:
    uint32_t file_read(const char *path, uint8_t *buf, uint32_t maximum_len, int16_t &status);
private:
    
};
#endif