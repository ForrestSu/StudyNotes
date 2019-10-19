#include "http_request_call.h"
//#include "../json11/json11.hpp"
#include "request.h"
#include <string>
#include <iostream>
#include <stdio.h>

using namespace lasote;

using namespace std;

class HttpJSONRequest : public HttpRequestCall{
	public:
		HttpJSONRequest(int chunk_size=0): HttpRequestCall(chunk_size){}
    //You can also override the following methods
    /*
      virtual void on_body(const char *p, size_t len);
      virtual void on_message_begin();
      virtual void on_url(const char* url, size_t length);
      virtual void on_header_field(const char* name, size_t length);
      virtual void on_header_value(const char* value, size_t length);
      virtual void on_headers_complete(void);
    */
		void on_message_complete(int status){
		    std::cout<<"on_message_complete: " <<  status <<std::endl;
       //By default, method on_body will append to response_buffer received response body
	 		/* auto json = Json::parse(this->response_buffer, err);
	 		 cout << json.dump();*/
		};
};


int main() {

  Method method("POST",  "http://192.168.4.61:8086/write?u=toptrade&p=toptrade&db=sunquan");
  Request request( &method);

  string post_data = "cpu,host=serverCode,region=china_code value=0.111 1520320877369812809";
  request.setPostData(post_data);

  HttpJSONRequest request_call;
  request_call.send(request);

  /*
  HttpJSONRequest request_call2;
  request_call2.send(request);
  */

  //Run libuv loop
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
