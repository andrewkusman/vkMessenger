#include "VK_API.h"
#include "LongPollSession.h"
#include <string>
#include <iostream>



int main()
{
    std::string password;
    std::cin >> password;
    VK_API vk_api = VK_API("89998132952", password);
    while(true) {

        if (vk_api.Authorize())
        {
            vk_api.needCaptcha = false;
            vk_api.haseError = false;
            std::cout << "Login successful" << std::endl;
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
        }
        else {
            std::cout << "ERROR: " + vk_api.error + " :: " + vk_api.error_description << std::endl;
            if (vk_api.needCaptcha) {
                std::cout << "Captcha sid :: " + vk_api.captcha.captcha_sid << std::endl
                << "Captcha img :: " + vk_api.captcha.captcha_img << std::endl
                << "Input Captcha key: ";
                std::cin >> vk_api.captcha.captcha_key;
            }
        }
    }
    return 0;
}

//g++ -std=c++11 FunctionsFile.cpp LongPollSession.cpp main.cpp Messages.cpp User.cpp VK_API.cpp -o main -pthread -lcurl -lcurlpp