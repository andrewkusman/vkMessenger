//
// Created by togarinzmey on 11/25/15.
//

#include "User.h"

User::User()
{
    //ignored
}

User::User(std::string firstName, std::string lastName, int idOfUser, std::string accessToken)
{
    this->first_name = firstName;
    this->last_name = lastName;
    this->id = idOfUser;
    this->access_token = accessToken;
    this->list_of_messages.clear();
}

User::User(std::string firstName, std::string lastName, int idOfUser, bool isOnline, std::string accessToken)
{
    this->first_name = firstName;
    this->last_name = lastName;
    this->id = idOfUser;
    this->online = isOnline;
    this->access_token = accessToken;
    this->list_of_messages.clear();
}

bool wayToSort(Messages i, Messages j) { return i.timestamp > j.timestamp; }

void User::GetMessageHistory(int offset, int count)
{
    offset = 0;
    std::string url;
    if(!this->list_of_messages.empty())
    {
        url = "https://api.vk.com/method/messages.getHistory?"
              "user_id=" + std::to_string(this->id) +
              "&count=" + std::to_string(count) +
              "&rev=0"
              "&start_message_id=" + std::to_string(this->list_of_messages.front().message_id);
              "&access_token=" + this->access_token +
              "&v=5.40";
    }
    else
    {
        url = "https://api.vk.com/method/messages.getHistory?"
              "user_id=" + std::to_string(this->id) +
              "&count=" + std::to_string(count) +
              "&rev=0"
              "&access_token=" + this->access_token +
              "&v=5.40";
    }
    rapidjson::Document document;
        std::string response = GetResponseString(url);
        document.Parse<0>(response.c_str());
        int tmp = 0;
        if (response == "") {

        }
        else {
            const rapidjson::Value &responseJson = document["response"];
            if (responseJson.HasMember("items") && responseJson["items"].IsArray() && !responseJson["items"].Empty()) {
                const rapidjson::Value &items = responseJson["items"];
                for (rapidjson::SizeType i = 0; i < items.Size(); i++) {
                    list_of_messages.push_back(*(new Messages(items[i]["id"].GetInt(),
                                                              items[i]["from_id"].GetInt() == this->id ? 51 : 30,
                                                              items[i]["from_id"].GetInt(),
                                                              items[i]["date"].GetInt(),
                                                              "",
                                                              items[i]["body"].GetString())));
                }
            }
        }
    std::sort(this->list_of_messages.begin(), this->list_of_messages.end(), wayToSort);
    this->loaded_history = true;
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
