#include "VK_API.h"
#include "LongPollSession.h"
#include "User.h"
#include "Me.h"

void Send(Me* usr)
{
    int id = usr->GetId();
    std::string str = " ";
    while(true)
    {
        std::cout << usr->GetNameById(id) + " >> ";
        std::getline(std::cin, str);
        if(str.compare("изменить адресата") == 0)
        {
            std::cout << "Укажите нового адресата: " ;
            std::string tmp = " ";
            std::getline(std::cin, tmp);
            for(int i = 0; i < usr->list_of_user.size(); i++)
            {
                if (usr->list_of_user[i].GetFullName().compare(tmp) == 0)
                {
                    id = usr->list_of_user[i].GetId();
                }
            }
        }
        else
        {
//            std::cout << "Sent " << std::endl;
            if(usr->MessageSent(id, str))
            {
//                std::cout << "fine " << std::endl;
            }
            else
                std::cout << "Pizdec" << std::endl;
        }
    }
}

int main()
{
    std::string password;
    std::cin >> password;
    VK_API vk_api = VK_API("89998132952", password);
    while(true)
    {

        if (vk_api.Authorize())
        {
            vk_api.needCaptcha = false;
            vk_api.has_error = false;
            std::cout << "Login successful" << std::endl;
            Me usr = Me(vk_api);
            std::cout << usr.list_of_user.front().GetFirstName() + " >> " +  usr.list_of_user.front().GetLastName()<< std::endl;
            LongPollSession longPollSession = LongPollSession(vk_api);
            if(longPollSession.StartThread())
            {
                std::thread thr(Send, &usr);
                thr.detach();
            }
            while(true)
            {
                if(!longPollSession.queueOfMessages.empty())
                {
//                    std::cout << "Crush before IncMessagesSort" << std::endl;
                    usr.IncMessagesSort(longPollSession.queueOfMessages.front());
                    longPollSession.queueOfMessages.pop();
                    for(int i = 0; i < usr.list_of_user.size(); i++)
                    {
                        if(usr.list_of_user[i].NewMessages() == true)
                        {
                            for(int j = 0; j < usr.list_of_user[i].list_of_messages.size(); j++)
                            {
                                if(usr.list_of_user[i].list_of_messages[j].is_new == true)
                                {
                                    std::string tmp;
                                    if(usr.list_of_user[i].list_of_messages[j].fromMe)
                                    {
//                                        std::cout << "\r\n" + usr.list_of_user[i].GetFirstName()
//                                        << " >> " + usr.list_of_user[i].list_of_messages[j].text
//                                        << std::endl;
                                    }
                                    else
                                    {
                                        std::cout << "\r\n" + usr.list_of_user[i].GetFirstName()
                                        << " << " + usr.list_of_user[i].list_of_messages[j].text
                                        << std::endl;
                                    }
                                    usr.list_of_user[i].list_of_messages[j].is_new = false;
                                }
                            }
                            usr.list_of_user[i].SetNewMessages(false);
                        }
                    }
//                    std::cout << "Crush before pop" << std::endl;
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

//#include "VK_API.h"
//#include "LongPollSession.h"
//#include "User.h"
//#include "Me.h"
//
////void Send(Me* usr)
////{
////    int id = 35220086;
//////    char *msg;
////    std::string str;
//////    std::cin >> id;
////    while(true)
////    {
////        std::cin >> str;
////        usr->MessageSent(id, str);
////    }
////}
//
//int main()
//{
//    std::string password;
//    std::cin >> password;
//    VK_API vk_api = VK_API("89998132952", password);
//    while(true)
//    {
//        if (vk_api.Authorize())
//        {
//            vk_api.needCaptcha = false;
//            vk_api.has_error = false;
//            std::cout << "Login successful" << std::endl;
//            Me usr = Me(vk_api);
//            std::cout << usr.list_of_user.front().GetFirstName() + " >> " +  usr.list_of_user.front().GetLastName()<< std::endl;
//            LongPollSession longPollSession = LongPollSession(vk_api);
//            longPollSession.StartThread();
////            std::thread thr(Send, &usr);
////            thr.detach();
//            while(true)
//            {
//                if(!longPollSession.queueOfMessages.empty())
//                {
//                    usr.IncMessagesSort(longPollSession.queueOfMessages.front());
//                    for(int i = 0; i < usr.list_of_user.size(); i++)
//                    {
//                        if(usr.list_of_user[i].NewMessages())
//                        {
//                            for(int j = usr.list_of_user[i].list_of_messages.size(); j >= 0; j--)
//                            {
//                                if(usr.list_of_user[i].list_of_messages[j].is_new)
//                                {
//                                    std::cout << ">> " + usr.list_of_user[i].GetFirstName() + " :: "
//                                    << std::boolalpha << usr.list_of_user[i].list_of_messages[j].fromMe
//                                    << ": " + usr.list_of_user[i].list_of_messages[j].text
//                                    << std::endl;
//                                    usr.list_of_user[i].list_of_messages[j].is_new = false;
//                                }
//                            }
//                            usr.list_of_user[i].SetNewMessages(false);
//                        }
//                    }
//                    longPollSession.queueOfMessages.pop();
//                }
//            }
//        }
//        else {
//            std::cout << "ERROR: " + vk_api.error + " :: " + vk_api.error_description << std::endl;
//            if (vk_api.needCaptcha) {
//                std::cout << "Captcha sid :: " + vk_api.captcha.captcha_sid << std::endl
//                << "Captcha img :: " + vk_api.captcha.captcha_img << std::endl
//                << "Input Captcha key: ";
//                std::cin >> vk_api.captcha.captcha_key;
//            }
//        }
//    }
//    return 0;
//}

//g++ -std=c++11 FunctionsFile.cpp Me.cpp LongPollSession.cpp main.cpp Messages.cpp User.cpp VK_API.cpp -o main -pthread -lcurl -lcurlpp