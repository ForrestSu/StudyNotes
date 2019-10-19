#include "uv_tcp_client.h"

using namespace EMS::BP;

void on_close_handle_cb(uv_handle_t* handle){
    free(handle);
}

TcpClient::TcpClient(uv_loop_t* loop, const std::string& tcp_addr, int port, int id){
    m_loop = loop;
    m_id = id;
    m_saddr = tcp_addr;
    uv_ip4_addr(tcp_addr.c_str(), port, &m_sockaddr);

    m_socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(m_loop, m_socket);
    uv_tcp_keepalive(m_socket, 1, 600); // 60 seconds
    uv_tcp_nodelay(m_socket, 1);

    m_state = 0;
    m_connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
    m_connect->data = this;
    uv_tcp_connect(m_connect, m_socket, (const struct sockaddr*)(&m_sockaddr), TcpClient::on_tcp_connect);
}

TcpClient::~TcpClient() {
    m_state = 0;
    if (m_connect)
        free(m_connect);
    if (m_socket) {
        uv_close((uv_handle_t*) m_socket, on_close_handle_cb);
    }
}

void TcpClient::on_tcp_connect(uv_connect_t* req, int status) {
    TcpClient* self = (TcpClient*) (req->data);
    if (status != 0) {
        self->m_state = 0;
        fprintf(stderr, "error: %s\n", uv_strerror(status));
    } else {
        self->m_state = 1; //
        printf("tcp-connected! m_id:%d.\n", self->m_id);
    }
}



