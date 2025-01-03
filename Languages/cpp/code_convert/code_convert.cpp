#include <string.h>
#include <iconv.h>

#include "code_convert.h"

/**
 * @return 0 successful; -1 fail
 * usage:
 *    int iRet == code_convert("gb2312", "utf-8", inbuf, &inlen, outbuf, &outlen);
 *    printf("convert %ld bytes, buffer[%s]!\n", outlen, outbuf);
 *    std::string stext = std::string(outbuf, outlen); //to string
 */
int code_convert(const char *from_charset,const char *to_charset, char *inbuf, size_t* inlen, char *outbuf, size_t* outlen)
{
    int iRet = 0;
    //1 iconv_open(2) 函数说明将要进行哪两种编码的转换,tocode是目标编码,fromcode是原编码,该函数返回一个转换句柄
    iconv_t cd = iconv_open(to_charset, from_charset);
    if (cd == (iconv_t)-1)
        return -1;
    size_t outbytesleft = *outlen;
    //2 iconv(5)函数从inbuf中读取字符,转换后输出到outbuf中, inbytesleft 用以记录还未转换的字符数, outbytesleft用以记录输出缓冲的剩余空间
    size_t rc = iconv(cd, &inbuf, inlen, &outbuf, &outbytesleft);
    if (rc == (size_t) -1) {
        iRet = -1; //fail
    } else {//success
        *outlen = ((*outlen) - outbytesleft); //get converted bytes
        outbuf[*outlen] = '\0';
    }
    //3 iconv_close(1)函数用于关闭转换句柄,释放资源
    iconv_close(cd);
    return iRet;
}
