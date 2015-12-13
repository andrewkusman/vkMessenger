//
// Created by togarinzmey on 11/25/15.
//

#ifndef VKCLASSTEST_MESSAGES_H
#define VKCLASSTEST_MESSAGES_H

#include <string>
#include <iostream>
#include "FunctionsFile.h"
#include <list>

class Messages
{
public:
    int message_id;
    int flags;
    int from_id;
    int timestamp;
    std::string subject;
    std::string text;
    bool fromMe = false;
    bool readstate = false;
    bool is_new = true;
    bool from_histroy;
    Messages(int message_id,
             int flags,
             int from_id,
             int timestamp,
             std::string subject,
             std::string text);
    Messages();
    static std::list<Messages> GetMessageList(std::string);
};


#endif //VKCLASSTEST_MESSAGES_H
