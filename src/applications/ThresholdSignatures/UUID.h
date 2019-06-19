/*
 * A random unique ID.
 */

#ifndef __TS_UUID_H
#define __TS_UUID_H

#include <ThresholdSignatures/ThresholdSignatures.h>

#include <string>
#include <ostream>

// A randomly generated unique ID.
class ThresholdSignatures_API UUID final
{
  public:

    /// Constructor.
    UUID();

    /**
    * Create from a string.
    * @param uuid Value to create ourselves from.
    */
    UUID(const std::string& uuid) : mUuid{uuid} { mUuidLen = uuid.length();}


    /**
    * Get our UUID as a string.
    * @return Our value as a string.
    */
    const std::string& getString() const { return mUuid; }

    /**
    * Generate a unique ID and return it as a string.
    * @return A unique string value.
    */
    static std::string generateUUIDString();

  private:

    /// Our string value.
    std::string mUuid {};

    unsigned int mUuidLen {32};
};

/// Comparision operators
ThresholdSignatures_API bool operator==(const UUID& lhs, const UUID& rhs);
ThresholdSignatures_API bool operator!=(const UUID& lhs, const UUID& rhs);
/// Stream output
ThresholdSignatures_API std::ostream& operator<<(std::ostream& str, const UUID& uuid);

#endif // __TS_UUID_H
