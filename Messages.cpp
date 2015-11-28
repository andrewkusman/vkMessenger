//
// Created by togarinzmey on 11/25/15.
//

#include "Messages.h"


Messages::Messages(int inMessage_id,
                   int inFlags,
                   int inFrom_id,
                   int inTimestamp,
                   std::string inSubject,
                   std::string inText)
{
    this->message_id = inMessage_id;
    this->flags = inFlags;
    this->from_id = inFrom_id;
    this->timestamp = inTimestamp;
    this->subject = inSubject;
    this->text = inText;
}

Messages::Messages()
{
    //ignored
}

std::list<Messages> Messages::GetMessageList(std::string incJson){
    rapidjson::Document document;
    document.Parse<0>(incJson.c_str());
    std::list<Messages> tmpListOfMessages;
    const rapidjson::Value& updates = document["updates"];
    if(updates.IsArray() && !updates.Empty())
    {
        for(rapidjson::SizeType i = 0; i < updates.Size(); i++)
        {
            const rapidjson::Value& element = updates[i];
//            if(element.IsArray())
//            {
//                std::cout << "Ololo" << std::endl;
//            }
            if(element.IsArray())
            {
                for(rapidjson::SizeType j = 0; j < element.Size(); j++)
                {
                    if(j == 0 && element[j].GetInt() == 4)
                    {
                        Messages msg;
                        j++;
                        msg.message_id = element[j].GetInt();
                        j++;
                        msg.flags = element[j].GetInt();
                        j++;
                        msg.from_id = element[j].GetInt();
                        j++;
                        msg.timestamp = element[j].GetInt();
                        j++;
                        msg.subject = element[j].GetString();
                        j++;
                        msg.text = element[j].GetString();
                        tmpListOfMessages.push_front(msg);
                    }
                }
            }
        }
    }
    return tmpListOfMessages;
}

//{"ts":1763976004,"updates":[[4,460011,17,13467663,1448718162," ... ","Ололололо"],
// [80,1,0],[7,13467663,460010],[3,460011,1,13467663],[6,13467663,460011],[80,0,0]]}


//{\"ts\":1763976062,\"updates\":[[4,460018,49,3924756,1448718741,\" ... \",\"Бум\"],[80,1,0],[7,3924756,460017]]}