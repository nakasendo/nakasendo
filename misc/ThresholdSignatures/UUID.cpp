/*
 * A random unique ID.
 */

#include <ThresholdSignatures/UUID.h>
#include <BigNumbers/BigNumbers.h>
#include <MessageHash/MessageHash.h>

// Constructor
UUID::UUID()
{
    // Generate random value
    BigNumber ranVal;
    std::string uuidVal = ranVal.generateRandHex(mUuidLen * 8);
    MessageHash hashMess;
    hashMess.HashSha256 (uuidVal);
    mUuid = hashMess.HashHex();
}

// Generate a unique ID and return it as a string.
std::string UUID::generateUUIDString()
{
    UUID uuid {};
    return uuid.getString();
}

/// Comparision operators
bool operator==(const UUID& lhs, const UUID& rhs) {
    return lhs.getString() == rhs.getString();
}

bool operator!=(const UUID& lhs, const UUID& rhs) {
    return lhs.getString() != rhs.getString();
}
/// Stream output
std::ostream& operator<<(std::ostream& str, const UUID& uuid)
{
    str << uuid.getString();
    return str;
}


