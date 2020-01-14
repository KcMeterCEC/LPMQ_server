#ifndef __SERVER_H__
#define __SERVER_H__
#include <cstdint>

class Server
{
public:
    Server(std::uint16_t p);

    ~Server();

    bool exec(void);
private:
    std::uint16_t   port = 0;
    int             server_fd = 0;
};
#endif