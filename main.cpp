#include <iostream>
#include "VK_API.h"
#include "LongPollSession.h"
#include "Messages.h"
#include <list>

using namespace std;

int main() {
    VK_API* test = new VK_API("89998132952", "K365fmt315z");
    if(test->Authorize())
        std::cout << "Sucsessdul\r\n" << test->Get_AccessToken() <<  std::endl;
    LongPollSession* tmp = new LongPollSession(*(test));
    tmp->Start();
//    Messages msg;
//    std::list<Messages> test =  msg.GetMessageList("{\"ts\":1763976004,\"updates\":[[4,460011,17,13467663,1448718162,\" ... \",\"Ололололо\"],"
//                                "[80,1,0],[7,13467663,460010],[3,460011,1,13467663],[6,13467663,460011],[80,0,0],[4,460018,49,3924756,1448718741,\" ... \",\"Бум\"],[80,1,0],[7,3924756,460017]]}");
//    std::cout <<"Text = " + test.front().text << std::endl;
//    test.pop_front();
//    std::cout <<"Text = " + test.front().text << std::endl;
    return 0;
}