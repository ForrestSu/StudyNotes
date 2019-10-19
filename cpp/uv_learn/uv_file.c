#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <uv.h>
#include <iostream>

using namespace std;
void on_read(uv_fs_t *req);

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;

static char buffer[1024];

static uv_buf_t iov;

void on_write(uv_fs_t *req) {
   std::cout<<"write\n"<<std::endl;
}

void on_read(uv_fs_t *req) {
   std::cout<<"read\n"<<std::endl;
}

void on_open(uv_fs_t *req) {
    if (req->result != -1) {
        iov = uv_buf_init(buffer, sizeof(buffer));
        uv_fs_read(uv_default_loop(), &read_req, req->result,
                   &iov, 1, -1, on_read);
    }
    else {
        fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
    }
    uv_fs_req_cleanup(req);
}

int main(int argc, char **argv) {
	
	  uv_loop_t m_uv_t;
	   uv_loop_init(&m_uv_t);
   // uv_fs_open(uv_default_loop(), &open_req, argv[1], O_RDONLY, 0, on_open);
    uv_file m_file=uv_fs_open(&m_uv_t, &open_req, argv[1], O_CREAT | O_APPEND | O_WRONLY, 0644,nullptr);
    
    
    char data[64];
    for(int i=0;i<63;i++)
    	data[i]='1';
    data[63]='\0';
    int size=64;
    
    uv_buf_t buf = uv_buf_init(data, (unsigned int) size);
   // uv_fs_t *req = (uv_fs_t*) malloc(sizeof(uv_fs_t));
    open_req.data = buf.base;
    uv_fs_write(&m_uv_t, &open_req,m_file, &buf, 1, -1,on_write);
   // uv_loop_close(&m_uv_t);
    uv_run(&m_uv_t, UV_RUN_DEFAULT);
   // uv_fs_close(uv_default_loop(),&open_req,m_file,on_read);
    
    return 0;
}
