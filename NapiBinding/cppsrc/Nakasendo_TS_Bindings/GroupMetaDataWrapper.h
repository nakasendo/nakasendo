#ifndef __GROUP_META_DATA_WRAPPER__
#define __GROUP_META_DATA_WRAPPER__

#include <napi.h>
#include <memory>
#include <GroupMetaData.h>

class GroupMetaDataWrap : public Napi::ObjectWrap<GroupMetaDataWrap>{
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        GroupMetaDataWrap(const Napi::CallbackInfo&);
        const std::unique_ptr<GroupMetadata>& getInternalInstance() { return GrpMetaDataPtr;}
    private:
        static Napi::FunctionReference constructor; 
        Napi::Value printPlayerGrp(const Napi::CallbackInfo&);
        
        Napi::Value GetGroupID(const Napi::CallbackInfo& );
        Napi::Value SetGroupID(const Napi::CallbackInfo& );
        Napi::Value GetProposer(const Napi::CallbackInfo& );
        Napi::Value SetProposer(const Napi::CallbackInfo&);
        Napi::Value GetM(const Napi::CallbackInfo&);
        Napi::Value SetM(const Napi::CallbackInfo&);
        Napi::Value GetN(const Napi::CallbackInfo&);
        Napi::Value SetN(const Napi::CallbackInfo&);
        Napi::Value GetT(const Napi::CallbackInfo&);
        Napi::Value SetT(const Napi::CallbackInfo&);
        Napi::Value GetGroupSet(const Napi::CallbackInfo&);
        Napi::Value SetGroupSet(const Napi::CallbackInfo&);
        Napi::Value GetGroupInviteReplies(const Napi::CallbackInfo&);
        Napi::Value SetGroupInviteReplies(const Napi::CallbackInfo&);
        Napi::Value IncGroupInviteReplies(const Napi::CallbackInfo&);
        Napi::Value GetGroupSignatureReplies(const Napi::CallbackInfo&);
        Napi::Value SetGroupSignatureReplies(const Napi::CallbackInfo&);
        Napi::Value IncGroupSignatureReplies(const Napi::CallbackInfo&);
        Napi::Value GetParticipantList(const Napi::CallbackInfo&);
        Napi::Value SetCalculationType(const Napi::CallbackInfo&);
        Napi::Value GetCalculationType(const Napi::CallbackInfo&);

        Napi::Value AddUserToGroup(const Napi::CallbackInfo&);
        Napi::Value removeUserFromGroup(const Napi::CallbackInfo&);
        Napi::Value CollectedHiddenPolys(const Napi::CallbackInfo&);
        Napi::Value AddCollatedHiddenPolys(const Napi::CallbackInfo&);
        Napi::Value CollatedHiddenEvals (const Napi::CallbackInfo&);
        Napi::Value AddCollatedHiddenEvals(const Napi::CallbackInfo&);
        Napi::Value CollatedVW(const Napi::CallbackInfo&);
        Napi::Value AddCollatedVW(const Napi::CallbackInfo&);
        Napi::Value CollatedPartialSignatures(const Napi::CallbackInfo&);
        Napi::Value AddCollatedPartialSignature(const Napi::CallbackInfo&);

        Napi::Value ClearSharedDataContainers(const Napi::CallbackInfo&);
        Napi::Value ClearSignatureContainers(const Napi::CallbackInfo&);
        Napi::Value ClearPreSignatureContainers(const Napi::CallbackInfo&);
        Napi::Value isGroupLocked(const Napi::CallbackInfo&);
        Napi::Value LockGroup(const Napi::CallbackInfo&);
        Napi::Value UnLockGroup(const Napi::CallbackInfo&);
        std::unique_ptr<GroupMetadata> GrpMetaDataPtr; 
};

#endif //#ifndef __GROUP_META_DATA_WRAPPER__
