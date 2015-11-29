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
                      "&lang=ru";
    const char* tmp = GetResponseString(url).c_str();
    std::string tmpStr = tmp;
    if(tmpStr == "")
    {
        this->error = "connection";
        this->error_description = "no_connection";
        this->haseError = true;
        return false;
    }
    document.Parse<0>(tmp);
    if(document.HasMember("access_token"))
    {
        this->_AccessToken = document["access_token"].GetString();
        return true;
    }
    if (document.HasMember("error"))
    {
        this->_AccessToken = "";
        this->error = document["error"].GetString();
        this->haseError = true;
        return false;
    }
    if (document.HasMember("error_description"))
    {
        this->_AccessToken = "";
        this->error_description = document["error_description"].GetString();
        this->haseError = true;
        return false;
    }
    else
    {
        this->_AccessToken = "";
        this->error = "ERROR";
        this->error_description = "unknown_error";
        this->haseError = true;
        return false;
    }

}

std::string VK_API::Get_AccessToken()
{
    return this->_AccessToken;
}