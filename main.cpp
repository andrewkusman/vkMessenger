#include "VK_API.h"
#include "LongPollSession.h"
#include <string>
#include <iostream>



int main()
{
    std::string password;
    std::cin >> password;
    VK_API vk_api = VK_API("89998132952", password);
    if(vk_api.Authorize())
    {
        std::cout << "Login successful" << std::endl;
    }
    LongPollSession longPollSession = LongPollSession(vk_api);
    longPollSession.StartThread();
    while(true)
    {
        if(!longPollSession.queueOfMessages.empty())
        {
            std::cout << ">> " + longPollSession.queueOfMessages.front().text << std::endl;
            longPollSession.queueOfMessages.pop();
        }
    }

    return 0;
}