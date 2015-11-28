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
//    std::cout << url;
    const char* tmp = GetResponseString(url).c_str();
    document.Parse<0>(tmp);
    assert(document.HasMember("access_token"));
    this->_AccessToken = document["access_token"].GetString();
    return true;

}

std::string VK_API::Get_AccessToken()
{
    return this->_AccessToken;
}