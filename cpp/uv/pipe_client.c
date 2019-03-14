#include <uv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SAFE_FREE(x)  if (x){free(x); x=NULL;}

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
    char cmd[16];
} write_req_t;
const char* uv_cmd = "exit";

void echo_write(uv_write_t *req, int status) {
    printf("echo_write() status =  %d\n", status);
    if (status < 0) {
        fprintf(stderr, "Write error: %s\n", uv_strerror(status));
    }
    SAFE_FREE(req);
}

void on_connect_cb(uv_connect_t* req, int status)
{
    printf("on_connect_cb()  status = %d. \n ", status);
    if (status == 0)
    {
       /* write_req_t * wt_req = (write_req_t *) malloc(sizeof(write_req_t));
        size_t cmd_len = strlen(uv_cmd);
        memcpy(wt_req->cmd, uv_cmd, cmd_len);
        wt_req->buf = uv_buf_init(wt_req->cmd, cmd_len);
        uv_write((uv_write_t*) wt_req, req->handle, &wt_req->buf, 1, echo_write);
   */
   } else {
        printf("errinfo : %s\n", uv_strerror(status));
    }
}

#ifdef _WIN32
#define NAME_PIPE_PREFIX "\\\\.\\pipe\\xunce."
#else
#define NAME_PIPE_PREFIX  "/tmp/xunce."
#endif
#define NAME_PIPE NAME_PIPE_PREFIX ## "echo.sock"

int main(int argc, char* argv[])
{
    uv_loop_t * uvloop = uv_default_loop();

    uv_pipe_t handle;
    uv_connect_t req_conn;

    uv_pipe_init(uvloop, &handle, 0);
    uv_pipe_connect(&req_conn, &handle, NAME_PIPE, on_connect_cb);

    uv_run(uvloop, UV_RUN_DEFAULT);
    printf("exit\n");
    return 0;
}
