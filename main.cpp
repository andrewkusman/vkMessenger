#include "VK_API.h"
#include "LongPollSession.h"
#include "User.h"
#include "Me.h"


int main()
{
    std::string password;
    std::cin >> password;
    VK_API vk_api = VK_API("89998132952", password);
    while(true) {

        if (vk_api.Authorize())
        {
            vk_api.needCaptcha = false;
            vk_api.has_error = false;
            std::cout << "Login successful" << std::endl;
            Me usr = Me(vk_api);
            std::cout << usr.list_of_user.front().GetFirstName() + " >> " +  usr.list_of_user.front().GetLastName()<< std::endl;
            LongPollSession longPollSession = LongPollSession(vk_api);
            longPollSession.StartThread();
            while(true)
            {
                if(!longPollSession.queueOfMessages.empty())
                {
                    usr.IncMessagesSort(longPollSession.queueOfMessages.front());
                    for(int i = 0; i < usr.list_of_user.size(); i++)
                    {
                        if(usr.list_of_user[i].NewMessages())
                        {
                            std::cout <<">> " + usr.list_of_user[i].GetFirstName() + " :: "
                                        << std::boolalpha << longPollSession.queueOfMessages.front().fromMe
                                        << ": " + longPollSession.queueOfMessages.front().text
                                        << std::endl;
                            usr.list_of_user[i].SetNewMessages(false);
                        }
                    }
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

//g++ -std=c++11 FunctionsFile.cpp Me.cpp LongPollSession.cpp main.cpp Messages.cpp User.cpp VK_API.cpp -o main -pthread -lcurl -lcurlpp