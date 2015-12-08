//
// Created by togarinzmey on 11/30/15.
//

#include "Me.h"

Me::Me(VK_API tmp)
{
    rapidjson::Document document;
    std::string url = "https://api.vk.com/method/users.get?v=5.40&access_token=" + tmp.Get_AccessToken();
    std::string response = GetResponseString(url);
    if(response == "")
    {

    }
    else
    {
        const char* chr = response.c_str();
        document.Parse<0>(chr);
        const rapidjson::Value& responseJson = document["response"];
        if(responseJson.IsArray())
        {
            for(rapidjson::SizeType i = 0; i < responseJson.Size(); i++)
            {
                if(responseJson[i].HasMember("id"))
                {
                    this->id = responseJson[i]["id"].GetInt();
                }
                if(responseJson[i].HasMember("first_name"))
                {
                    this->first_name = responseJson[i]["first_name"].GetString();
                }
                if(responseJson[i].HasMember("last_name"))
                {
                    this->last_name = responseJson[i]["last_name"].GetString();
                }
                this->online = true;
            }
        }
    }
    url = "https://api.vk.com/method/friends.get?v=5.40"
                  "&fields=name"
                  "&access_token=" + tmp.Get_AccessToken();
    response = GetResponseString(url);
    if(response == "")
    {

    }
    else
    {
        const char* chr = response.c_str();
        document.Parse<0>(chr);
        const rapidjson::Value& resp = document["response"];
        if(resp.HasMember("count") && resp.HasMember("items"))
        {
            this->friends_count = resp["count"].GetInt();
            std::cout << this->friends_count << std::endl;
            const rapidjson::Value& items = resp["items"];
            if(items.IsArray())
            {
                for(rapidjson::SizeType i = 0; i < items.Size(); i++)
                {
                    if(items[i].HasMember("id"))
                    {
//                        std::cout << items[i]["first_name"].GetString() << std::endl;
                        list_of_user.push_back(*(new User(
                                items[i]["first_name"].GetString(),
                                items[i]["last_name"].GetString(),
                                items[i]["id"].GetInt(),
                                items[i]["online"].GetInt() != 0)));
                    }
                }
            }
        }

    }
}

bool Me::IncMessagesSort(Messages newMessage)
{
    for(int i = 0; i < list_of_user.size(); i++)
    {
        if(list_of_user[i].GetId() == newMessage.from_id)
        {
            list_of_user[i].list_of_messages.push_back(newMessage);
            list_of_user[i].SetNewMessages(true);
            return true;
        }
    }
    return false;
}

std::string Me::GetFirstName() const
{
    return this->first_name;
}

int Me::GetId() const
{
    return this->id;
}

std::string Me::GetLastName() const
{
    return this->last_name;
}

bool Me::IsOnline() const
{
    return this->online;
}

bool Me::NewMessages() const
{
    return this->new_messages;
}

int Me::GetFriendsNumb() const
{
    return this->friends_count;
}
