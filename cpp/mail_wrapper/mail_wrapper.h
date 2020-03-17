#pragma once
#include <vector>
#include <string>

class MailWrapper{
public:
    MailWrapper(){
    }
    ~MailWrapper(){}
    // check field info is complete
    bool is_vaild()
    {
        if (from.size() <= 0) {
            return false;
        }
        if (rcpts.size() <= 0) {
            return false;
        }
        return true;
    }
    std::string to_echo_fmt(const std::string& text)
    {
        std::string shell = "echo '" + text + "' | " + format_auth();
        return std::move(shell);
    }
    std::string to_file_fmt(const std::string& filename)
    {
        std::string shell = format_auth() + " < " + filename;
        return std::move(shell);
    }
    //call mailx shell
    static bool popen_mailx(const std::string& cmd, std::string& errmsg);
private:
    std::string format_auth()
    {
         std::string shell = "mail -s '" + subject + "'"
                + " -S sendwait"
                + " -S 'from=" + from + "'"
                + " -S smtp='"+ smtp + "'"
                + " -S smtp-auth='" + smtp_auth +"'"
                + " -S smtp-auth-user='" + smtp_auth_user + "'"
                + " -S smtp-auth-password='" + smtp_auth_pass + "'";
        for (size_t k = 0; k < rcpts.size(); ++k) {
            if (rcpts[k].size() > 0) {
                shell += (" \"" + rcpts[k] + "\"");
            }
        }
        return std::move(shell);
    }
public:
    std::string smtp;  //smtp_server: smtp.qiye.163.com
    std::string smtp_auth; // login
    std::string smtp_auth_user; // test@xuncetech.com
    std::string smtp_auth_pass; // pass
    std::string subject;
    std::string from;  // nick<test@xuncetech.com>
    std::vector<std::string> rcpts; // 收件人
};

