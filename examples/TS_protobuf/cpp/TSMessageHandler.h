#ifndef __TSMESSAGE_HANDLER_H__
#define __TSMESSAGE_HANDLER_H__

#include <iostream>
#include <string>
#include <TSMessageFactory.h>

struct MessageDescriptor
{
    MessageDescriptor() : m_EnumMSGID(TSMessageDefs::TS_UNKNOWN_TYPE),m_grpID(),m_userID(), m_SSType(),m_Message(){ return ; }
    ~MessageDescriptor() = default;
    MessageDescriptor(const MessageDescriptor& obj) 
    : m_EnumMSGID(obj.m_EnumMSGID),m_grpID(obj.m_grpID), m_userID (obj.m_userID), m_SSType(obj.m_SSType),m_Message(obj.m_Message) { return ; }
    MessageDescriptor& operator=(const MessageDescriptor& obj){
        if(this != &obj){
            m_EnumMSGID = obj.m_EnumMSGID;
            m_grpID = obj.m_grpID;
            m_userID = obj.m_userID; 
            m_SSType = obj.m_SSType;
            m_Message = obj.m_Message;
        }
        return *this; 
    }
    TSMessageDefs m_EnumMSGID; 
    std::string m_grpID;
    std::string m_userID;
    std::string m_SSType;
    std::string m_Message; 
    int m_eKeyIndex;
};

bool HandleMessage(MessageDescriptor&, std::istream&);
bool CreateResponseMessage(MessageDescriptor&,std::ostream&);


// 
bool SendGroupInvites(const std::string&, const std::string&);
bool SendGroupDetails(const std::string&); 
bool SendGroupDelete(const std::string&, const std::string&);

bool ShareEvalsPlayers(const std::string&, const std::string&, const std::string&);
bool SharePrivateEvalsRequest(const std::string&, const std::string&, const std::string&);
bool SecretSharingPlayerDataRequest(const std::string&, const std::string&,const std::string&);
bool SecretSharingCollatedDataRequest(const std::string&, const std::string&, const std::string&);

bool EphemeralKeyPlayerDataRequest(const std::string&, const std::string&);
bool EphemeralKeyCollatedPlayerDataRequest(const std::string&, const std::string&);

bool SignaturePlayerDataRequest(const std::string&, const std::string&, const std::string&, const int&);
bool deleteGrpSignatureSharingContainsers(const std::string&);
bool deleteGrpPreSignatureContainers(const std::string&);

bool InitPrivateKeyShareExchange(const std::string&,const std::string&,const std::vector<std::string>&);
bool SharePrivateKeyShare(const std::string&,const std::string&);

#endif //#ifndef __TSMESSAGE_HANDLER_H__

