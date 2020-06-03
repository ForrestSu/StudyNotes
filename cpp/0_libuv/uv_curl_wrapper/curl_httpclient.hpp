#include <cstdio>
#include <cassert>
#include <string>
#include <uv.h>
#include <curl/curl.h>

/**
 * Author: sunquan
 * Date: 2020-06-03 16:54:36 GMT+8:00
 * Description:
 *  Integrate curl into the libuv event loop.
 *  use curl multi-handle api (asynchronous APIs).
 *  allows curl to be used in event driven applications!
 */

namespace uvframe {
namespace baseutils {

// internal struct, don't use it!!!
typedef struct curl_context_s {
    void *data; //save this
    uv_poll_t poll_handle; //data save curl_context_t
    curl_socket_t sockfd;
} curl_context_t;

typedef struct curl_req_s {
    uint64_t reqid;
    // if recv data too long, mark error flag!
    int err_code;
    std::string buffer;
}curl_req_t;
//end internal

template<class T>
class CurlHttpClient {
public:
    typedef void (T::*OnCurlCallBack)(int status, const std::string& msg, uint64_t reqid);

    CurlHttpClient(uv_loop_t *loop, T *thisObj) :
            m_reqid{0}, m_loop(loop), m_thisObj(thisObj), m_on_http_cb(nullptr),
            m_curl_multi_hdl(nullptr), m_timer_hdl(nullptr) {
    }

    ~CurlHttpClient() {
        StopPoll();
        if (m_timer_hdl) {
            uv_close((uv_handle_t*) m_timer_hdl, on_close_uv_hdl);
            m_timer_hdl = NULL;
        }
        if (m_curl_multi_hdl) {
            curl_multi_cleanup(m_curl_multi_hdl);
            m_curl_multi_hdl = nullptr;
        }
    }

    int StartPoll(OnCurlCallBack cb) {
        m_on_http_cb = cb;
        if (curl_global_init(CURL_GLOBAL_DEFAULT)) {
            fprintf(stderr, "Could not init curl\n");
            return -1;
        }
        // init timer
        if (m_timer_hdl == nullptr) {
            m_timer_hdl = (uv_timer_t*) malloc(sizeof(uv_timer_t));
            if (!m_timer_hdl) {
                fprintf(stderr, "fail to malloc() m_timer_hdl! errno = %d.", errno);
                return -1;
            }
            m_timer_hdl->data = this;
            uv_timer_init(m_loop, m_timer_hdl);
        }

        m_curl_multi_hdl = curl_multi_init();
        curl_multi_setopt(m_curl_multi_hdl, CURLMOPT_SOCKETDATA, this);
        curl_multi_setopt(m_curl_multi_hdl, CURLMOPT_SOCKETFUNCTION, handle_socket);
        curl_multi_setopt(m_curl_multi_hdl, CURLMOPT_TIMERDATA, this);
        curl_multi_setopt(m_curl_multi_hdl, CURLMOPT_TIMERFUNCTION, start_timer);
        return 0;
    }

    int StopPoll() {
        if (m_timer_hdl) {
            uv_timer_stop(m_timer_hdl);
        }
        return 0;
    }

    uint64_t SendGet(const std::string &url, int64_t timeout_ms = 12000) {
        CURL *handle = curl_easy_init();
        curl_req_t* pResp = new curl_req_t();
        pResp->reqid = ++m_reqid;
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, timeout_ms);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, pResp); //for cb_get_head
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback_get_head);
        curl_easy_setopt(handle, CURLOPT_PRIVATE, pResp); //user data
        curl_multi_add_handle(m_curl_multi_hdl, handle);
        return m_reqid;
    }
    // TODO SendPost() wait imp...

private:
    static void on_close_uv_hdl(uv_handle_t *handle)
    {
        handle->data = NULL;
        free(handle);
    }

    curl_context_t* create_curl_context(curl_socket_t sockfd) {
        curl_context_t* ctx = (curl_context_t*) malloc(sizeof(curl_context_t));
        ctx->sockfd = sockfd;
        ctx->data = this;
        uv_poll_init_socket(m_loop, &ctx->poll_handle, sockfd);
        ctx->poll_handle.data = ctx;
        return ctx;
    }

    static void on_close_ctx_cb(uv_handle_t *handle) {
        curl_context_t *ctx = static_cast<curl_context_t*>(handle->data);
        free(ctx); //这里handle为ctx的成员变量,只需要释放ctx即可
    }

    void destroy_curl_context(curl_context_t *context) {
        uv_close((uv_handle_t*) &(context->poll_handle), on_close_ctx_cb);
    }

    static size_t callback_get_head(void *buf, size_t size, size_t nmemb, void *userp) {
        size_t len = size * nmemb;
        if (len > 0) {
            assert(userp != NULL);
            auto pResp = static_cast<curl_req_t*>(userp);
            pResp->buffer.append(static_cast<const char*>(buf), len);
        }
        //printf("callback_get_head() len = %ld\n", len);
        return len;
    }

    void check_multi_info() {
        char *done_url;
        CURLMsg *message;
        int pending;
        CURL *easy_handle;
        CURLcode rc;
        curl_req_t *resp_data = NULL;

        while ((message = curl_multi_info_read(m_curl_multi_hdl, &pending))) {
            switch (message->msg) {
            case CURLMSG_DONE: {
                rc = message->data.result;
                easy_handle = message->easy_handle;
                curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &done_url);
                curl_easy_getinfo(easy_handle, CURLINFO_PRIVATE, &resp_data);
                assert(resp_data != NULL);
                //get error info
                if(rc != CURLE_OK){
                    resp_data->buffer = curl_easy_strerror(rc);
                }
                if (m_on_http_cb) {
                    (m_thisObj->*m_on_http_cb)(rc, resp_data->buffer, resp_data->reqid);
                }
                delete resp_data;
                curl_multi_remove_handle(m_curl_multi_hdl, easy_handle);
                curl_easy_cleanup(easy_handle);
            }
            break;

            default:{
                 fprintf(stderr, "CURLMSG default\n");
            }
            break;
            }
        }
    }

    static void curl_perform(uv_poll_t *req, int status, int events) {
        int running_handles;
        int flags = 0;
        curl_context_t *context = (curl_context_t*) req->data;
        auto *pThis = static_cast<CurlHttpClient<T>*>(context->data);

        if (events & UV_READABLE)
            flags |= CURL_CSELECT_IN;
        if (events & UV_WRITABLE)
            flags |= CURL_CSELECT_OUT;

        curl_multi_socket_action(pThis->m_curl_multi_hdl, context->sockfd, flags, &running_handles);
        pThis->check_multi_info();
    }

    static void on_timer_cb(uv_timer_t *req) {
        CurlHttpClient<T> *pThis = static_cast<CurlHttpClient<T>*>(req->data);
        int running_handles;
        curl_multi_socket_action(pThis->m_curl_multi_hdl, CURL_SOCKET_TIMEOUT, 0, &running_handles);
        pThis->check_multi_info();
    }

    static int start_timer(CURLM *multi, long timeout_ms, void *userp) {
        CurlHttpClient<T> *pThis = static_cast<CurlHttpClient<T>*>(userp);
        if (timeout_ms < 0) {
            uv_timer_stop(pThis->m_timer_hdl);
        } else {
            /* 0 means directly call socket_action, but we'll do it in a bit */
            if (timeout_ms == 0) {
                timeout_ms = 1;
            }
            uv_timer_start(pThis->m_timer_hdl, on_timer_cb, timeout_ms, 0);
        }
        return 0;
    }

    static int handle_socket(CURL *easy, curl_socket_t s, int action, void *userp, void *socketp) {
        CurlHttpClient<T> *pThis = static_cast<CurlHttpClient<T>*>(userp);
        curl_context_t *curl_context;
        int events = 0;

        switch (action) {
        case CURL_POLL_IN:
        case CURL_POLL_OUT:
        case CURL_POLL_INOUT:
            curl_context = socketp ? (curl_context_t*) socketp : pThis->create_curl_context(s);

            curl_multi_assign(pThis->m_curl_multi_hdl, s, (void*) curl_context);

            if (action & CURL_POLL_IN)
                events |= UV_READABLE;
            if (action & CURL_POLL_OUT)
                events |= UV_WRITABLE;
            uv_poll_start(&curl_context->poll_handle, events, curl_perform);
            break;
        case CURL_POLL_REMOVE:
            if (socketp) {
                uv_poll_stop(&((curl_context_t*) socketp)->poll_handle);
                pThis->destroy_curl_context((curl_context_t*) socketp);
                curl_multi_assign(pThis->m_curl_multi_hdl, s, NULL);
            }
            break;
        default:
            abort();
        }
        return 0;
    }

private:
    uint64_t m_reqid;
    uv_loop_t *m_loop;
    T *m_thisObj;
    OnCurlCallBack m_on_http_cb;
    CURLM *m_curl_multi_hdl;
    uv_timer_t *m_timer_hdl;
};
}
}
