#ifndef __TS_PROTO_BUF_HELPER_H__
#define __TS_PROTO_BUF_HELPER_H__

#include <string>
#include <ts_messages.pb.h>

std::string enum2string( const thresholdsignature::CalcType&);
thresholdsignature::CalcType string2enum (const std::string&);

#endif //#ifndef __TS_PROTO_BUF_HELPER_H__


