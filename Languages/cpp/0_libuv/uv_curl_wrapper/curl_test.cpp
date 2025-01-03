#include <uv.h>
#include <memory>
#include "curl_httpclient.hpp"

using namespace uvframe::baseutils;

class Test{

public:
    Test(uv_loop_t* loop):m_loop(loop){
    }
    ~Test(){}

    void Init() {
        m_httpclient = std::make_shared<CurlHttpClient<Test>>(m_loop, this);
        m_httpclient->StartPoll(&Test::CallBack);

    }

    void DoWork(const std::string& url){
        printf("will Get <%s>...\n", url.c_str());
        m_httpclient->SendGet(url);
    }
private:
    void CallBack(int status, const std::string &msg, uint64_t reqid) {
        printf("status = %d, len = %ld, reqid = %lu\n ", status, msg.length(), reqid);
        std::string emit = msg;
        if (emit.length() > 50) {
            emit = emit.substr(0, 10) + "...";
        }
        puts(emit.c_str());
    }

private:
    uv_loop_t* m_loop;
    std::shared_ptr<CurlHttpClient<Test>> m_httpclient;
};


int main(int argc, char **argv)
{
     if (argc <= 1) {
        printf("Usages:  %s www.baidu.com www.google.com\n", argv[0]);
        return 0;
    }
     std::string url = argv[1];

     uv_loop_t* loop = uv_default_loop();
    {
        auto test = std::make_shared<Test>(loop);
        test->Init();
        while (argc-- > 1) {
            test->DoWork(argv[argc]);
        }
        uv_run(loop, UV_RUN_DEFAULT);
    }

     return 0;
}
