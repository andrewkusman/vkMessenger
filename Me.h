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

class Me : public User
{
private:
    int friends_count;
    bool new_messages = false;
public:
    Me(VK_API);
    std::string error_message;
    std::string GetNameById(int id);
    int GetFriendsNumb() const;
    User GetUserByFullName(std::string fullName) const;
    bool MessageSent(int id, std::string message);
    bool IncMessagesSort(Messages newMessage);

    std::vector<User> list_of_user;
};


#endif //VKCLASSTEST_ME_H
