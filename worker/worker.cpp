#include <sys/types.h>
#include <sys/socket.h>
#include <cassert>
#include <errno.h>
#include <unistd.h>
#include <cstdio>

#include "worker.h"
#include "rb.h"

#include "process.h"
#include "io.h"
#include "mem.h"

#include "../debug.h"

Worker::Worker(int fd)
: client_fd(fd)
{
    rd_buf = new Rb(1024);
    ps = new Process();
    io = new Io();
    mem = new Mem();
}
Worker::~Worker()
{
    delete rd_buf;
    delete ps;
    delete io;
    delete mem;
}
void Worker::close_client(void)
{
    if(client_fd)
    {
        close(client_fd);
        client_fd = 0;
    }
}
bool Worker::send_result(void)
{
    bool ret = true;
    if(send(client_fd, (const void *)&head, sizeof(Header), MSG_NOSIGNAL) == -1)
    {
        if((errno == EPIPE) || (errno == ENOTCONN))
        {
            LOG_INFO("connect has been closed by client!");
            close_client();

            return false;
        }
        else
        {
            LOG_ERR("send data to client failed!");
            assert(0);
        }
    }
    if(head.payload_len)
    {
        if(send(client_fd, (const void *)recv_tmp, head.payload_len, MSG_NOSIGNAL) == -1)
        {
            if((errno == EPIPE) || (errno == ENOTCONN))
            {
                LOG_INFO("connect has been closed by client!");
                close_client();

                return false;
            }
            else
            {
                LOG_ERR("send data to client failed!");
                assert(0);
            }
        }
    }

    return ret;
}
void Worker::exec(void)
{
    int ret = 0;

    enum
    {
        GET_HEAD,
        GET_CONTENTS,
    }status;

    status = GET_HEAD;
    while(1)
    {
        ret = recv(client_fd, recv_tmp, sizeof(recv_tmp), 0);
        if(ret > 0)
        {
            rd_buf->write(recv_tmp, ret);
            while(1)
            {
                if(status == GET_HEAD)
                {
                    if(rd_buf->read((uint8_t *)&head, sizeof(Header)))
                    {
                        if(head.ck == HEAD_CK)
                        {
                            status = GET_CONTENTS;
                        }
                        else
                        {
                            LOG_ERR("net data fault!");
                            assert(0);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if(rd_buf->read(recv_tmp, head.payload_len))
                    {
                        status = GET_HEAD;
                        switch(head.cmd)
                        {
                            case CLASS_INFO:
                            {
                                LOG_DBG("dealwith sysinfo command.");
                                head.payload_len = sys_info(recv_tmp, sizeof(recv_tmp), head.status);
                            }break;
                            case CLASS_PS:
                            {
                                head.payload_len = ps->exec(recv_tmp, sizeof(recv_tmp), head.status);
                            }break;
                            case CLASS_MEM:
                            {
                                head.payload_len = mem->exec(recv_tmp, sizeof(recv_tmp), head.status);
                            }break;
                            case CLASS_IO:
                            {
                                head.payload_len = io->exec(recv_tmp, sizeof(recv_tmp), head.status);
                            }break;
                            default:
                            {
                                LOG_ERR("unknown command!");
                                assert(0);
                            }break;
                        }

                        if(!send_result())
                        {
                            return;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        else if(ret == 0)
        {
            LOG_INFO("client is exited");
            break;
        }
        else
        {
            if(errno != ECONNRESET)
            {
                perror("socket fault:");
                assert(0);
            }
        }
    }

    close_client();
}
std::uint32_t Worker::sys_info(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status)
{
    FILE *fp = std::fopen(ps->cpuinfo_path_get(), "r");

    if(!fp)
    {
        LOG_ERR("can't open file " << ps->cpuinfo_path_get());
        status = -LPMQ_NO_FILE;

        return 0;
    }
    std::uint32_t r_size = std::fread(buf, 1, maximum_len, fp);
    std::fclose(fp);

    status = LPMQ_OK;

    return r_size;
}