//
// Created by togarinzmey on 11/25/15.
//

#ifndef VKCLASSTEST_USER_H
#define VKCLASSTEST_USER_H
#include <list>
#include <string>
#include <vector>
#include "Messages.h"
#include <algorithm>

class User
{
protected:
    bool online = false;
    std::string first_name;
    std::string last_name;
    bool new_messages = false;
    int id;
    std::string access_token;
public:
    int last_message_id;
    bool loaded_history = false;
    std::string GetFullName() const;
    void GetMessageHistory(int offset, int count);
    bool IsOnline() const;
    std::string GetFirstName() const;
    std::string GetLastName() const;
    bool NewMessages() const;
    void SetNewMessages(bool tmp);
    int GetId() const;
    User(std::string firstName, std::string lastName, int id, std::string accessToken);
    User(std::string firstName, std::string lastName, int id, bool online, std::string accessToken);
    User();

    std::vector<Messages> list_of_messages;
};


#endif //VKCLASSTEST_USER_H
