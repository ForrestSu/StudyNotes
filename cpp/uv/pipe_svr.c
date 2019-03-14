#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

uv_loop_t *loop;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  const int buffer_len = 32;
  buf->base = (char*) malloc(buffer_len);
  buf->len = buffer_len;
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {

    printf("echo_read() nread =  %d\n", nread);
    if (nread > 0) {
        printf("recv: data = [%s], len =  %d\n", buf->base, buf->len);
        if(strncmp(buf->base, "exit", 4) == 0)
        {
            printf("will stop!\n");
           // uv_stop(loop);
        }
        return;
    }

    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, NULL);
    }

    free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
    if (status == -1) {
        // error!
        return;
    }

    uv_pipe_t *client = (uv_pipe_t*) malloc(sizeof(uv_pipe_t));
    uv_pipe_init(loop, client, 0);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        printf("uv_accept() status %d\n", status);
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*) client, NULL);
    }
}

void on_signal(int sig) {

#ifndef _WIN32
    uv_fs_t req;
    uv_fs_unlink(loop, &req, "echo.sock", NULL);
#endif
    uv_stop(loop);
}

#ifdef _WIN32
#define NAME_PIPE_PREFIX "\\\\.\\pipe\\xunce."
#else
#define NAME_PIPE_PREFIX  "/tmp/xunce."
#endif

#define NAME_PIPE NAME_PIPE_PREFIX ## "echo.sock"
int main() {
    loop = uv_default_loop();

    uv_pipe_t server;
    uv_pipe_init(loop, &server, 0);

    signal(SIGINT, on_signal);

    printf("%s\n", NAME_PIPE);

    int rc = uv_pipe_bind(&server, NAME_PIPE);
    if (rc) {
        printf("UV_EADDRINUSE is %d\n", UV_EADDRINUSE);
        fprintf(stderr, "Bind error: %d:%s\n", rc, uv_strerror(rc));
        return 1;
    }
    rc = uv_listen((uv_stream_t*)&server, 128, on_new_connection);
    if (rc) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(rc));
        return 2;
    }
    rc = uv_run(loop, UV_RUN_DEFAULT);
    printf("exit!\n");
    return rc;
}
