//
// Created by togarinzmey on 11/27/15.
//

#ifndef VKCLASSTEST_LONGPOLLSESSION_H
#define VKCLASSTEST_LONGPOLLSESSION_H
#include "VK_API.h"
#include "Messages.h"
#include <queue>
#include "FunctionsFile.h"
#include <thread>

struct LongPollServerResponse
{
    std::string key = "";
    std::string server = "";
    int ts = 0;
};

class LongPollSession
{
private:
    struct LongPollServerResponse Response;
    std::string urlForLongPoll;
    std::string use_ssl = "0";
    std::string need_pts = "0";
    std::string accessToken;
    bool GetLongPollServer();
    void MakeUrl();
    bool action;

public:
//    static void StartSession(LongPollSession&);
    std::queue<Messages> queueOfMessages;
    LongPollSession(VK_API&);
    LongPollSession(VK_API&, bool, bool);
    ~LongPollSession();
    void Start();
    std::string Output();
//    static void StartThread(LongPollSession&);
//    void Stop();
};


#endif //VKCLASSTEST_LONGPOLLSESSION_H
