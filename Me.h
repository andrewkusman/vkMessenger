//
// Created by togarinzmey on 11/30/15.
//

#ifndef VKCLASSTEST_ME_H
#define VKCLASSTEST_ME_H

#include "VK_API.h"
#include <vector>
#include "Messages.h"
#include "User.h"
#include <map>
#include <ctime>
#include <algorithm>

class Me : User
{
private:
    bool new_messages = false;
    int friends_count;
    std::string access_token;
public:
    Me(VK_API);
    std::string error_message;
    std::string GetNameById(int id);
    int GetFriendsNumb() const;
    bool IsOnline() const;
    std::string GetFirstName() const;
    std::string GetLastName() const;
    User GetUserByFullName(std::string fullName) const;
    bool NewMessages() const;
    int GetId() const;
    std::vector<User> list_of_user;
    bool MessageSent(int id, std::string message);
    bool IncMessagesSort(Messages newMessage);
};


#endif //VKCLASSTEST_ME_H
