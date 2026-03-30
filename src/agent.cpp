#include<iostream>
#include<string>
#include"sqlite3.h"
#include"httplib.h"
#include"json.hpp"
using json=nlohmann::json;

bool agentprocess(sqlite3* db,const std::string& studentID,const std::string& query,std::string& final_message)
{
    httplib::Client cli("localhost",5000);
    cli.set_connection_timeout(5);
    cli.set_read_timeout(30);
    cli.set_write_timeout(30);

    json req_body;
    req_body["query"]=query;

    auto res=cli.Post("/llm_agent",req_body.dump(),"application/json");

    if(!res)
    {
        final_message="Failed to connect to LLM service.";
        return false;
    }

    json llm_result;
    try
    {
        llm_result=json::parse(res->body);
    }
    catch(const std::exception&)
    {
        final_message="Failed to parse LLM response.";
        return false;
    }

    if(llm_result.contains("final_message")&&llm_result["final_message"].is_string())
    {
        final_message=llm_result["final_message"].get<std::string>();
    }
    else
    {
        final_message="LLM service returned no final message.";
    }

    if(res->status!=200)
    {
        return false;
    }

    if(!llm_result.contains("success")||!llm_result["success"].is_boolean())
    {
        final_message="LLM response does not contain valid success field.";
        return false;
    }

    return llm_result["success"].get<bool>();
}