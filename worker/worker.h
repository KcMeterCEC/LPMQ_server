#ifndef __WORKER_H__
#define __WORKER_H__
#include <cstdint>
class Rb;
class Process;
class Io;
class Mem;

#define HEAD_CK (0xA5)

typedef enum
{
    CLASS_PS,
    CLASS_MEM,
    CLASS_IO,
}cmd_class;

struct Header
{
    std::uint8_t        ck;
    cmd_class           cmd;
    std::uint16_t       payload_len;
}__attribute__((__packed__));

class Worker
{
public:
    Worker() = default;
    Worker(int fd);

    ~Worker();

    void exec(void);
private:
    int             client_fd = 0;
    std::uint8_t    recv_tmp[1024];
    Header          head;
    Rb              *rd_buf = nullptr;

    Process         *ps = nullptr;
    Io              *io = nullptr;
    Mem             *mem = nullptr;

    void close_client(void);
    bool send_result(void);
};
#endif
