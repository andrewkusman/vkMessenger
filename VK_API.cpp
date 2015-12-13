#include "VK_API.h"

VK_API::~VK_API()
{
    //ignored
}


VK_API :: VK_API(std::string userName, std::string password)
{
    this->_UserName = userName;
    this->_Password = password;
}

bool VK_API::Authorize()
{
    rapidjson::Document document;
    std::string url = "https://oauth.vk.com/token"
                              "?grant_type=password"
                              "&client_id=3697615"
                              "&client_secret=AlVXZFMUqyrnABp8ncuU"
                              "&username=" + this->_UserName +
                      "&password=" + this->_Password +
                      "&v=5.40"
                              "&lang=ru";;
    if(needCaptcha)
    {
        url += "&captcha_sid=" + this->captcha.captcha_sid +
               "&captcha_key=" + this->captcha.captcha_key;
    }
    std::string tmpStr = GetResponseString(url);
    if(tmpStr == "")
    {
        this->error = "connection";
        this->error_description = "no_connection";
        this->has_error = true;
        return false;
    }
    const char* tmp = tmpStr.c_str();
    document.Parse<0>(tmp);
    if(document.HasMember("access_token"))
    {
        this->_AccessToken = document["access_token"].GetString();
        return true;
    }
    if (document.HasMember("error"))
    {
        if(document.HasMember("captcha_sid") && document.HasMember("captcha_img"))
        {
            this->error = document["error"].GetString();
            this->captcha.captcha_sid = document["captcha_sid"].GetString();
            this->captcha.captcha_img = document["captcha_img"].GetString();
            this->needCaptcha = true;
            this->has_error = true;
            return false;
        }
        this->_AccessToken = "";
        this->error = document["error"].GetString();
        this->has_error = true;
        return false;
    }
    if (document.HasMember("error_description"))
    {
        this->_AccessToken = "";
        this->error_description = document["error_description"].GetString();
        this->has_error = true;
        return false;
    }
    else
    {
        this->_AccessToken = "";
        this->error = "ERROR";
        this->error_description = "unknown_error";
        this->has_error = true;
        return false;
    }

}

std::string VK_API::Get_AccessToken() const
{
    return this->_AccessToken;
}