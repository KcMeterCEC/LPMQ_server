#ifndef __WORKER_H__
#define __WORKER_H__

#include "basic.h"

class Rb;
class Process;
class Io;
class Mem;

#define HEAD_CK (0xA5)

typedef enum
{
    CLASS_INFO,
    CLASS_PS,
    CLASS_MEM,
    CLASS_IO,
}cmd_class;

struct Header
{
    uint8_t        ck;
    uint8_t        cmd;
    int16_t        status;
    uint16_t       payload_len;
}__attribute__((__packed__));

class Worker : public Basic
{
public:
    Worker() = default;
    Worker(int fd);

    ~Worker();

    void exec(void);
private:
    int             client_fd = 0;
    uint8_t    recv_tmp[16384];
    Header          head;
    Rb              *rd_buf = nullptr;

    Process         *ps = nullptr;
    Io              *io = nullptr;
    Mem             *mem = nullptr;

    void close_client(void);
    bool send_result(void);

    uint32_t sys_info(uint8_t *buf, uint32_t maximum_len, int16_t &status);
};
#endif
