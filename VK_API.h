#ifndef VK_API_H
#define VK_API_H

#include "FunctionsFile.h"
#include <string.h>
#include <sstream>
#include <iostream>

struct Captcha
{
    std::string captcha_sid = "";
    std::string captcha_img = "";
    std::string captcha_key = "";
};

class VK_API {
private:
    std::string _AccessToken;
    std::string _UserName;
    std::string _Password;
public:
    bool needCaptcha = false;
    struct Captcha captcha;
    std::string error = "";
    std::string error_description = "";
    bool has_error = false;
    VK_API(std::string userName, std::string password);
    ~VK_API();
    bool Authorize();
    std::string Get_AccessToken() const;
};


#endif // VK_API_H
