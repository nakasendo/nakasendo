
/*
 * Base class for communications messages.
 */

#ifndef _NCHAIN_SDK_MESSAGE_H
#define _NCHAIN_SDK_MESSAGE_H

#include "EnumCast.h"
#include <memory>

/// A message for passing between applications.
class Message 
{
  public:

    /// Enumeration for all message types.
    enum class Type
    {
        // Don't forget to update the enumTableT below as well.
        UNKNOWN,
        GROUP_SETUP,
        GROUP_RESPONSE,
        SHARED_SECRET_SETUP,
        PRE_SIGNATURE_SETUP,
        POOL_BUILDER_SETUP,
        PRE_SIGNATURE_INTERMEDIARY,
        POOL_BUILDER_INTERMEDIARY,
    };

    /**
    * Constructor.
    * @param type Type of this message.
    */
    Message(Type type) : mType{type} {}

    /**
    * Get message type.
    * @return The type of this message.
    */
    Type getType() const { return mType; }

    virtual std::string  to_json();
    virtual void from_json(const std::string& typeJson);
  protected:

    /// Type of this message.
    Type mType { Type::UNKNOWN };

};

/// Pointer type.
using MessageSPtr = std::shared_ptr<Message>;

/// Enable enum_cast for Message::Type
inline const enumTableT<Message::Type>& enumTable(Message::Type)
{
    static enumTableT<Message::Type> table
    {
        {Message::Type::UNKNOWN,                    "Unknown"},
        {Message::Type::GROUP_SETUP,                "GroupSetup"},
        {Message::Type::GROUP_RESPONSE,             "GroupResponse"},
        {Message::Type::SHARED_SECRET_SETUP,        "SharedSecretSetup"},
        {Message::Type::PRE_SIGNATURE_SETUP,        "PreSignatureSetup"},
        {Message::Type::POOL_BUILDER_SETUP,         "PoolBuilderSetup"},
        {Message::Type::PRE_SIGNATURE_INTERMEDIARY, "PreSignatureIntermediary"},
        {Message::Type::POOL_BUILDER_INTERMEDIARY,  "PoolBuilderIntermediary"},
    };
    return table;
}

#endif //_NCHAIN_SDK_MESSAGE_H
