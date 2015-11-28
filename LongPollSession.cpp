//
// Created by togarinzmey on 11/27/15.
//

#include "LongPollSession.h"

LongPollSession::LongPollSession(VK_API& tmp)
{
    this->accessToken = tmp.Get_AccessToken();
}

LongPollSession::LongPollSession(VK_API& tmp, bool need_pts, bool need_ssl)
{
    this->accessToken = tmp.Get_AccessToken();
    this->need_pts = need_pts;
    this->use_ssl = need_ssl;
}

LongPollSession::~LongPollSession()
{
    //ignore
}

void LongPollSession::Start()
{
    action = true;
    GetLongPollServer();
    rapidjson::Document document;
    std::string urlToConnect = "https://"+ this->Response.server +"?act=a_check"
            "&key=" + this->Response.key +
            "&ts=" + std::to_string(this->Response.ts) +
            "&wait=25"
            "&mode=0";
    std::list<Messages> tmpList;
    while(action)
    {
        const char *tmp = GetResponseString(urlToConnect).c_str();
        std::string response = tmp;
        document.Parse<0>(tmp);
        if (document.HasMember("ts")) {
            this->Response.ts = document["ts"].GetInt();
//            std::cout << Response.ts << std::endl;
        }
        urlToConnect = "https://"+ this->Response.server +"?act=a_check"
                       "&key=" + this->Response.key +
                       "&ts=" + std::to_string(this->Response.ts) +
                       "&wait=25"
                       "&mode=0";

        tmpList = Messages::GetMessageList(response);
        for(auto c : tmpList)
        {
            queueOfMessages.push(c);
            std::cout << c.from_id << " :: " + c.text << std::endl;

        }
    }
}

bool LongPollSession::GetLongPollServer()
{
    MakeUrl();
    rapidjson::Document document;
    std::string response = GetResponseString(this->urlForLongPoll);
    const char* cstr = response.c_str();
    document.Parse<0>(cstr);
    const rapidjson::Value& tmp = document["response"];

    if(tmp.HasMember("key")) {
        this->Response.key = tmp["key"].GetString();
//        std::cout << "Key: >>  " + this->Response.key << std::endl;
    }
    if(tmp.HasMember("server")) {
        this->Response.server = tmp["server"].GetString();
    }
    if(tmp.HasMember("ts")) {
        this->Response.ts = tmp["ts"].GetInt();
    }
    return true;
}

std::string LongPollSession::Output()
{
    return this->Response.server + std::to_string(this->Response.ts);
}

void LongPollSession::MakeUrl()
{
    this->urlForLongPoll = "https://api.vk.com/method/messages.getLongPollServer?"
                                 "use_ssl=" + this->use_ssl +
                                 "&need_pts=" + this->need_pts +
                                 "&access_token=" + this->accessToken +
                                 "&v=5.40";
}

//void LongPollSession::StartSession(LongPollSession& tmp)
//{
//    tmp.Start();
//}
//
//void LongPollSession::StartThread(LongPollSession& tmp)
//{
//    std::thread threadFunc(StartSession, std::cref(tmp));
//    threadFunc.join();
//}