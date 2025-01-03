/*
 * code_convert.h
 *
 *  Created on: 2018-05-16 15:33:20
 *      Author: sunquan
 *       Email: sunquana@gmail.com
 */
#ifndef CODE_CONVERT_H
#define CODE_CONVERT_H

int code_convert(const char *from_charset,const char *to_charset, char *inbuf, size_t* inlen, char *outbuf, size_t* outlen);

#endif
