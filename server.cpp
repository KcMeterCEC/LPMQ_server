#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cassert>
#include <thread>

#include "server.h"
#include "worker.h"
#include "debug.h"

using std::thread;

static void worker(int fd)
{
    Worker handler(fd);

    handler.exec();
}

Server::Server(std::uint16_t p = 60123)
: port(p), server_fd(0)
{
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    server_fd = socket(server_addr.sin_family, SOCK_STREAM, 0);
    assert(server_fd > 0);

    int option = true;
    socklen_t optlen = sizeof(option);

    int ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, static_cast<void *>(&option), optlen);
    assert(ret == 0);

    ret = bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    assert(ret == 0);

    ret = listen(server_fd, 5);
    assert(ret == 0);
}
Server::~Server()
{
    close(server_fd);
}
bool Server::exec(void) const
{
    int client_fd = 0;

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    LOG_INFO("waiting for client ...");
    while((client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
    &addr_len)))
    {
        if(client_fd < 0)
        {
            LOG_ERR("accept failed!");
            break;
        }
        LOG_INFO("client connected : " << inet_ntoa(client_addr.sin_addr)
             << " " << ntohs(client_addr.sin_port));

        thread *wt = new thread(worker, client_fd);
        wt->detach();
    }

    return false;
}