//
// Created by togarinzmey on 11/28/15.
//

#include "FunctionsFile.h"

std::string GetResponseString(std::string incUrl) // Get response from server for current URL(incUrl)
{
    try
    {
        rapidjson::Document document;
        std::stringstream ss;

        curlpp::Cleanup cleanup;
        curlpp::Easy request;

        request.setOpt(new curlpp::options::Url(incUrl));
        request.setOpt(new curlpp::options::WriteStream(&ss));

        request.perform();

//        std::cout << "GetResponsestring: = " + ss.str() << std::endl;
        return ss.str();
    }
    catch( cURLpp::RuntimeError &e )
    {
        std::cout << e.what() << std::endl;
        return "";
    }
    catch( cURLpp::LogicError &e )
    {
        std::cout << e.what() << std::endl;
        return "";
    }
}

