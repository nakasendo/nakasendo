#include <iostream>
#include <single_include/nlohmann/json.hpp>
#include <BigNumbers/BigNumbers.h>
#include <SecretSplit/KeyShare.h>


KeyShare::KeyShare (const KeyShare& obj) 
    : m_k (obj.m_k)
    , m_n (obj.m_n)
    , m_publicID (obj.m_publicID)
    , m_Index (obj.m_Index)
    , m_Share (obj.m_Share)
 { return ; }
         
 KeyShare& KeyShare::operator= (const KeyShare& obj){
    if (this != &obj){
        m_k = obj.m_k;
        m_n = obj.m_n;
        m_publicID = obj.m_publicID;
        m_Index = obj.m_Index;
        m_Share = obj.m_Share;
    }
    return *this ; 
 }
         
std::string keyshare_to_json (const KeyShare& obj) {
    nlohmann::json j;
    j["threshold"] = obj.k();
    j["sharecount"] = obj.n(); 
    j["pubid"] = obj.publicID();
    j["index"] = obj.Index().ToHex(); 
    j["share"] = obj.Share().ToHex(); 
    return j.dump();
}

KeyShare keyshare_from_json (const std::string& keyshareJson){

    KeyShare share; 
    nlohmann::json j;
    j = nlohmann::json::parse(keyshareJson); 

    share.k() = j.at("threshold").get<int>(); 
    share.n() = j.at("sharecount").get<int>();
    share.publicID() = j.at("pubid").get<std::string>();
    share.Index().FromHex(j.at("index").get<std::string>());
    share.Share().FromHex(j.at("share").get<std::string>());
 
    return share; 
}
    
