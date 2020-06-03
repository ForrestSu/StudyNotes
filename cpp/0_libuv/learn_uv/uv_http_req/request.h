#ifndef request_h_
#define request_h_
#include <stdlib.h>
#include <unordered_map>
#include <string>
#include <assert.h>


class Method{
public :
    Method(const std::string& type, const std::string& uri) {
        m_get_post = type;
        m_fulluri = uri;
        m_host = "";
        m_port = 80;
        m_uri = "";
       assert(ParseUri(uri)==true);
    }
    ~Method(){};
private:
    // http://github.com:80/master/src/xyhttp.cpp
    bool ParseUri(const std::string& uri)
    {
        auto ibegin = uri.find("://");
        if(ibegin!= std::string::npos)
        {
            size_t nlen = uri.find('/', ibegin+3);
            if(nlen != std::string::npos )
            {
                m_uri = uri.substr(nlen);
                nlen = nlen-ibegin-3;
            }
            auto hostport = uri.substr(ibegin+3,nlen);

            int find_pos = -1; //npos;
            int tmp_port = 0;
            for(size_t i=0;i<hostport.size();++i)
            {
                if (find_pos > 0)
                {
                    if (hostport[i] < '0' || hostport[i] > '9')
                        break;
                    tmp_port = tmp_port * 10 + (hostport[i] - '0');
                }
                if(hostport[i]==':')
                {
                    find_pos = i;
                    tmp_port = 0;
                }
            }
            if(tmp_port > 0) m_port = tmp_port;
            m_host = hostport.substr(0, find_pos);
            return true;
        }
        return false;
    }
public:
    std::string m_get_post;//POST, GET
    std::string m_fulluri;
    //detail
    std::string m_host; // "github.com"
    int m_port;         //80
    std::string m_uri;  // "/master/src/xyhttp.cpp"
};

class Request {
public:
    Request(Method* mth) {
        method = mth;
        // default prop
        headers.insert(std::make_pair("User-Agent", "UvHttpClient/1.0"));
        headers.insert(std::make_pair("Accept", "text/html,application/json,*/*"));
        headers.insert(std::make_pair("Accept-Encoding", "gzip, deflate, br"));
        headers.insert(std::make_pair("Accept-Language", "zh-CN,zh;q=0.9,en;q=0.8"));
        headers.insert(std::make_pair("Content-Type", "text/plain;charset=UTF-8"));
        // headers.insert(std::make_pair("Cache-Control", "max-age=0"));
        headers.insert(std::make_pair("Connection", "Keep-Alive"));

        //Fill default arguments if needed
        if (headers.find("User-Agent") == headers.end()) {
           headers.insert(std::make_pair("User-Agent", "libuv_http_client"));
        }
        if ( headers.find("Accept") == headers.end()) {
           headers.insert(std::make_pair("Accept", "text/html,application/json,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"));
        }
    }
    ~Request(){};
    bool setPostData(std::string& post_str)
    {
        m_post_data = post_str;
        headers.insert(std::make_pair("Content-Length", std::to_string(post_str.size())));
        return true;
    }
    std::string to_string()
    {
        std::string req_header = method->m_get_post;
        req_header.append(" " + method->m_uri + " HTTP/1.1\r\n");
        req_header.append("Host: "+method->m_host +"\r\n");

        for(auto & it: headers)
        {
            req_header.append(it.first + ": " + it.second +"\r\n");
        }
        req_header.append("\r\n");
        req_header.append(m_post_data);
        return std::move(req_header);
    }
public:
    Method* method;
    std::string m_post_data;
    std::unordered_map<std::string, std::string> headers;
};

#endif

