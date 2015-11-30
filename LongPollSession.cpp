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
    std::cout << "Start session" << std::endl;
    action = true;
    int counter = 0;
    while(counter < 4)
    {
        if(GetLongPollServer())  //Trying 4 times to connect to longpoll server
        {
            break;
        }
        else
            counter++;
    }
    if(counter == 4)
    {
        this->error = "Can't connect to longPoll server";
        this->wasError = true;
    }
    else {
        rapidjson::Document document;
        std::string urlToConnect = "https://" + this->Response.server + "?act=a_check"
                                   "&key=" + this->Response.key +
                                   "&ts=" + std::to_string(this->Response.ts) +
                                   "&wait=25"
                                   "&mode=0";
        std::list<Messages> tmpList;
        while (action) {
            std::string response = GetResponseString(urlToConnect);
            if(response == "")
            {
                this->error = "Problems in connection to longpoll";
                wasError = true;
            }
            else
            {
                const char *tmp = response.c_str();
                document.Parse<0>(tmp);
                if (document.HasMember("ts")) {
                    this->Response.ts = document["ts"].GetInt();
                }
                urlToConnect = "https://" + this->Response.server + "?act=a_check"
                               "&key=" + this->Response.key +
                               "&ts=" + std::to_string(this->Response.ts) +
                               "&wait=25"
                               "&mode=0";
                tmpList = Messages::GetMessageList(response);
                for (auto c : tmpList) {
                    queueOfMessages.push(c);
                }
            }
        }
        this->wasError = false;
        std::cout << "Thread was killed" << std::endl;
    }
}

bool LongPollSession::GetLongPollServer()
{
    MakeUrl();
    rapidjson::Document document;
    std::string response = GetResponseString(this->urlForLongPoll);
    if(response == "")
    {
        return false;
    }
    const char* cstr = response.c_str();
    document.Parse<0>(cstr);
    const rapidjson::Value& tmp = document["response"];

    if(tmp.HasMember("key")) {
        this->Response.key = tmp["key"].GetString();
    }
    if(tmp.HasMember("server")) {
        this->Response.server = tmp["server"].GetString();
    }
    if(tmp.HasMember("ts")) {
        this->Response.ts = tmp["ts"].GetInt();
    }
    if(tmp.HasMember("failed"))
    {
        return false;
    }
    return true;
}

void LongPollSession::MakeUrl()  //When we're creating new object, we have to refresh url, i can do it in
{                                //constructor, but i don't want ^_^
    this->urlForLongPoll = "https://api.vk.com/method/messages.getLongPollServer?"
                                 "use_ssl=" + this->use_ssl +
                                 "&need_pts=" + this->need_pts +
                                 "&access_token=" + this->accessToken +
                                 "&v=5.40";
}

void LongPollSession::StartThread()  //new thread to get messageList
{
    std::thread thr(&LongPollSession::Start, this);
    thr.detach();
}

void LongPollSession::KillThread()  //stop thread
{
    this->action = false;
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