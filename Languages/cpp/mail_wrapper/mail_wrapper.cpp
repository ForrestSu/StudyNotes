#include <stdio.h>  //popen
#include "mail_wrapper.h"

/**
 * @brief popen 执行 shell发送邮件
 * @param cmd 要执行的 shell命令
 * @param errmsg 失败返回错误信息
 * @return if successful return true;
 */
bool MailWrapper::popen_mailx(const std::string& cmd, std::string& errmsg)
{
    char buffer[256] = { 0 };
    FILE * fp = popen((cmd + " 2>&1").c_str(), "r");
    if (fp == NULL) {
        errmsg = "popen() failed!";
        return false;
    }
    int n_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    int rc = pclose(fp);
    // printf("rc = %d, n_read = %d\n ", rc, n_read);
    for (int k = 0; k < n_read; ++k) {
        if (buffer[k] == '\0') {
            buffer[k] = ' ';
        }
    }
    if (n_read > 0) {
        buffer[n_read] = '\0';
        errmsg = std::string(buffer, n_read);
        return false;
    }
    return true;
}
