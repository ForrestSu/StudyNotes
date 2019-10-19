#ifndef UV_TCP_CLIENT_H
#define UV_TCP_CLIENT_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <memory>
#include <uv.h>

namespace EMS{
namespace BP{

class TcpClient{
public:
    TcpClient(uv_loop_t * loop, const std::string& tcp_addr, int port, int id);
    ~TcpClient();
private:
    uv_loop_t * m_loop;
    std::string m_saddr;
    struct sockaddr_in m_sockaddr;
    static void on_tcp_connect(uv_connect_t* req, int status);
public:
    int m_id;
    uv_tcp_t  * m_socket;
    uv_connect_t *m_connect;
    int m_state; // 0 unavailable ;1 is ok
};

}}

#endif


