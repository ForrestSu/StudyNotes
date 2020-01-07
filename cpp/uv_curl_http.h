#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <curl/curl.h>

namespace uvframe {
	namespace baseutils {

		typedef struct http_info_s {
			void* data;
			char buff[1024*1024];
		} http_info_t;

		typedef struct curl_context_s {
			void* data;
			uv_poll_t poll_handle;
			curl_socket_t sockfd;
		} curl_context_t;

		template<class T>
		class FrameAsyncHttp {
		public:
			typedef void (T::*FrameAsyncHttpCallBack)(std::string);

			FrameAsyncHttp(uv_loop_t *loop, T* _thisObj, FrameAsyncHttpCallBack asyncHttpCallBack)
				:m_loop(loop), m_thisObj(_thisObj), m_asyncHttpCallBack(asyncHttpCallBack) {
			}

			~FrameAsyncHttp() {
				curl_multi_cleanup(m_curl_handle);
			}

			int InitHttp() {
				if (curl_global_init(CURL_GLOBAL_ALL)) {
					fprintf(stderr, "Could not init curl\n");
					return -1;
				}

				m_uv_timeout = (uv_timer_t*)malloc(sizeof(uv_timer_t));
				m_uv_timeout->data = this;
				uv_timer_init(m_loop, m_uv_timeout);

				m_curl_handle = curl_multi_init();
				curl_multi_setopt(m_curl_handle, CURLMOPT_SOCKETDATA, this);
				curl_multi_setopt(m_curl_handle, CURLMOPT_SOCKETFUNCTION, handle_socket);
				curl_multi_setopt(m_curl_handle, CURLMOPT_TIMERDATA, this);
				curl_multi_setopt(m_curl_handle, CURLMOPT_TIMERFUNCTION, start_timeout);
			}

			void SendHttpReq(const char* url)
			{
				m_http_info = (http_info_t*)malloc(sizeof(http_info_t));
				m_http_info->data = this;
				CURL *handle = curl_easy_init();
				curl_easy_setopt(handle, CURLOPT_WRITEDATA, m_http_info);
				curl_easy_setopt(handle, CURLOPT_URL, url);
				curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback_get_head);
				curl_multi_add_handle(m_curl_handle, handle);
			}

		private:
		    curl_context_t* create_curl_context(curl_socket_t sockfd)
			{
				curl_context_t *context;

				context = (curl_context_t *)malloc(sizeof(*context));

				context->sockfd = sockfd;
				context->data = this;

				uv_poll_init_socket(m_loop, &context->poll_handle, sockfd);
				context->poll_handle.data = context;

				return context;
			}

			static void curl_close_cb(uv_handle_t *handle)
			{
				curl_context_t *context = (curl_context_t *)handle->data;
				free(context);
			}

			void destroy_curl_context(curl_context_t *context)
			{
				uv_close((uv_handle_t *)&context->poll_handle, curl_close_cb);
			}

			static size_t callback_get_head(void *buf, size_t size, size_t nmemb, void *userp)
			{
				FrameAsyncHttp<T>* pThis = static_cast<FrameAsyncHttp<T>*>(((http_info_t*)userp)->data);
				char* response_body = (char*)pThis->m_http_info->buff;
				size_t response_body_len = strlen(response_body);
				size_t len = size * nmemb;
				memcpy(response_body + response_body_len, buf, len);
				if (pThis) {
					T* thisObj = pThis->m_thisObj;
					FrameAsyncHttpCallBack asyncHttpCallBack = pThis->m_asyncHttpCallBack;
					if (asyncHttpCallBack) {
						(thisObj->*asyncHttpCallBack)(std::string(response_body + response_body_len, len));
					}
				}
				free(userp);
				return size * nmemb;
			}

			void check_multi_info(void)
			{
				char *done_url;
				CURLMsg *message;
				int pending;
				CURL *easy_handle;

				while ((message = curl_multi_info_read(m_curl_handle, &pending))) {
					switch (message->msg) {
					case CURLMSG_DONE:
						easy_handle = message->easy_handle;
						curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &done_url);

						curl_multi_remove_handle(m_curl_handle, easy_handle);
						curl_easy_cleanup(easy_handle);
						break;

					default:
						fprintf(stderr, "CURLMSG default\n");
						break;
					}
				}
			}

			static void curl_perform(uv_poll_t *req, int status, int events)
			{
				int running_handles;
				int flags = 0;
				curl_context_t* context = (curl_context_t *)req->data;
				FrameAsyncHttp<T>* pThis = static_cast<FrameAsyncHttp<T>*>(context->data);

				if (events & UV_READABLE)
					flags |= CURL_CSELECT_IN;
				if (events & UV_WRITABLE)
					flags |= CURL_CSELECT_OUT;

				curl_multi_socket_action(pThis->m_curl_handle, context->sockfd, flags, &running_handles);

				pThis->check_multi_info();
			}

			static void on_timeout(uv_timer_t *req)
			{
				FrameAsyncHttp<T>* pThis = static_cast<FrameAsyncHttp<T>*>(req->data);
				int running_handles;
				curl_multi_socket_action(pThis->m_curl_handle, CURL_SOCKET_TIMEOUT, 0, &running_handles);
				pThis->check_multi_info();
			}

			static int start_timeout(CURLM *multi, long timeout_ms, void *userp)
			{
				FrameAsyncHttp<T>* pThis = static_cast<FrameAsyncHttp<T>*>(userp);
				if (timeout_ms < 0) {
					uv_timer_stop(pThis->m_uv_timeout);
				}
				else {
					if (timeout_ms == 0) {
						timeout_ms = 1;
					}

					uv_timer_start(pThis->m_uv_timeout, on_timeout, timeout_ms, 0);
				}
				return 0;
			}

			static int handle_socket(CURL *easy, curl_socket_t s, int action, void *userp, void *socketp)
			{
				FrameAsyncHttp<T>* pThis = static_cast<FrameAsyncHttp<T>*>(userp);
				curl_context_t *curl_context;
				int events = 0;

				switch (action) {
				case CURL_POLL_IN:
				case CURL_POLL_OUT:
				case CURL_POLL_INOUT:
					curl_context = socketp ? (curl_context_t *)socketp : pThis->create_curl_context(s);

					curl_multi_assign(pThis->m_curl_handle, s, (void *)curl_context);

					if (action != CURL_POLL_IN)
						events |= UV_WRITABLE;
					if (action != CURL_POLL_OUT)
						events |= UV_READABLE;
					uv_poll_start(&curl_context->poll_handle, events, curl_perform);
					break;
				case CURL_POLL_REMOVE:
					if (socketp) {
						uv_poll_stop(&((curl_context_t*)socketp)->poll_handle);
						pThis->destroy_curl_context((curl_context_t*)socketp);
						curl_multi_assign(pThis->m_curl_handle, s, NULL);
					}
					break;
				default:
					abort();
				}

				return 0;
			}

		private:
			uv_loop_t *m_loop;
			T* m_thisObj;
			CURLM *m_curl_handle;
			uv_timer_t* m_uv_timeout;
			http_info_t* m_http_info;
			FrameAsyncHttpCallBack m_asyncHttpCallBack;
		};
	}
}
