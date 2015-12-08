//
// Created by togarinzmey on 11/25/15.
//

#include "User.h"

User::User(std::string firstName, std::string lastName, int idOfUser)
{
    this->first_name = firstName;
    this->last_name = lastName;
    this->id = idOfUser;
}

User::User(std::string firstName, std::string lastName, int idOfUser, bool isOnline)
{
    this->first_name = firstName;
    this->last_name = lastName;
    this->id = idOfUser;
    this->online = isOnline;
}
std::string User::GetFirstName() const
{
    return this->first_name;
}

int User::GetId() const
{
    return this->id;
}

std::string User::GetLastName() const
{
    return this->last_name;
}

bool User::IsOnline() const
{
    return this->online;
}

bool User::NewMessages() const
{
    return this->new_messages;
}

void User::SetNewMessages(bool tmp)
{
    this->new_messages = tmp;
}

std::string User::GetFullName() const
{
    return this->first_name + " " + this->last_name;
}
