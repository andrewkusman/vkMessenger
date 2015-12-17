//
// Created by togarinzmey on 11/30/15.
//

#include "Me.h"

Me::Me(VK_API tmp)
{
    this->access_token = tmp.Get_AccessToken();
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
                  "&order=hints"
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
//            std::cout << this->friends_count << std::endl;
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
                                items[i]["online"].GetInt() != 0,
                                this->access_token)));
                    }
                }
            }
        }

    }
    list_of_user.push_back(*(new User(
            this->first_name,
            this->last_name,
            this->id,
            this->online,
            this->access_token)));;
}

bool Me::IncMessagesSort(Messages newMessage)
{
    for(int i = 0; i < this->list_of_user.size(); i++)
    {
        if(this->list_of_user[i].GetId() == newMessage.from_id)
        {
            this->new_messages = true;
            this->list_of_user[i].list_of_messages.push_back(newMessage);
            if(!newMessage.fromMe)
            {
                this->list_of_user[i].SetNewMessages(true);
            }
            auto it = this->list_of_user.begin() + i;
            std::rotate(it, this->list_of_user.begin(), it-1);
            return true;
        }
    }
}

bool Me::MessageSent(int id, std::string message)
{
    int msgId;
    for (size_t pos = message.find(' '); pos != std::string::npos; pos = message.find(' ', pos))
    {
        message.replace(pos, 1, "%20");
    }
    std::time_t time = std::time(0);
    rapidjson::Document document;
    std::string url = "https://api.vk.com/method/messages.send?v=5.40"
                              "&user_id=" + std::to_string(id) +
                              "&message=" + message +
                              "&access_token=" + this->access_token;
    std::string response = GetResponseString(url);
    if(response == "")
    {
        return false;
    }
    else
    {
        const char* chr = response.c_str();
        document.Parse<0>(chr);
        if(document.HasMember("response"))
        {
//            std::cout << "Norm" << std::endl;
            msgId = document["response"].GetInt();
//            std::cout << "Norm2" << std::endl;
            return true;
        }
        else if(document.HasMember("error_msg"))
        {
            this->error_message = document["error_msg"].GetString();
            return false;
        }
    }
}

std::string Me::GetNameById(int id)
{
    for(int i = 0; i < this->list_of_user.size(); i++)
    {
        if(this->list_of_user[i].GetId() == id)
            return this->list_of_user[i].GetFullName();
    }
    return "";
}


User Me::GetUserByFullName(std::string fullName) const
{
    for(int i = 0; i < this->list_of_user.size(); i++)
    {
        if(this->list_of_user[i].GetFullName().compare(fullName) == 0)
            return this->list_of_user[i];
    }
}

