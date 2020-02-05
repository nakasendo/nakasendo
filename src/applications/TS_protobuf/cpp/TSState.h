#ifndef __TS_STATE_H__
#define __TS_STATE_H__

#include <iostream>
#include <player.h>
#include <GroupMetaData.h>
#include <ts_messages.pb.h>

class BigNumber;
class ECPoint;


void WriteGroupMetaDataStateToFile(const GroupMetadata&, const std::string& );
void ReadGroupMetaDataStateToFile(GroupMetadata&, const std::string& );

void WriteGroupMetaDataState(const GroupMetadata& , std::ostream&);
void ReadGroupMetaDataState(GroupMetadata&, std::istream&);

void WriteJVRSSStateToFile(const jvrss&, const std::string&);
void ReadJVRSSStateFromFile(jvrss&, const std::string&);

void WriteJVRSSState(const jvrss&, std::ostream&);
void ReadJVRSSState(jvrss&, std::istream&);
void ReadJVRSS(jvrss&, const thresholdsignature::JVRSSState&);

void WritePlayerGroupStateToFile(const playerGroupMetaData&, const std::string&);
void ReadPlayerGroupStateFromFile(playerGroupMetaData&, const std::string&);

void WritePlayerGroupState(const playerGroupMetaData&, std::ostream&);
void ReadPlayerGroupState(playerGroupMetaData&, std::istream&);



#endif //#ifndef __TS_STATE_H__
