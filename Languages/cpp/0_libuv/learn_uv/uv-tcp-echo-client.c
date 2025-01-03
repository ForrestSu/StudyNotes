/** this program might leak memory; noticed by writer (https://nikhilm.github.io/uvbook) **/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <uv.h>

#define DEFAULT_PORT  13370
#define DEFAULT_BACKLOG 1024

uv_loop_t *loop;
uv_tcp_t* uvsocket;

void on_write(uv_write_t* req, int status) {
  if (status < 0) {
    fprintf(stderr, "on write error %s\n", uv_strerror(status));
    return;
  }
  uv_close((uv_handle_t*)uvsocket, NULL);
}

void on_connect(uv_connect_t* req, int status) {
  if (status < 0) {
    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
    return;
  }
      
    uv_buf_t buf[] = {
      { .base = "O", .len = 1 },
      { .base = "k", .len = 1 }
    };
    uv_write_t req1;
      
    uv_write(&req1, req->handle, buf, 2, on_write);

}

int main() {
    loop = uv_default_loop();
      
    uvsocket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, uvsocket);

    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    struct sockaddr_in dest;
    uv_ip4_addr("127.0.0.1", 13370, &dest);

    uv_tcp_connect(connect, uvsocket, (const struct sockaddr*)&dest, on_connect);
      
  
    return uv_run(loop, UV_RUN_DEFAULT);
}
