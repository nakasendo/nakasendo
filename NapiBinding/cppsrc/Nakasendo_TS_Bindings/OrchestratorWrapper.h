#ifndef __ORCHESTRATOR_WRAPPER_H__
#define __ORCHESTRATOR_WRAPPER_H__

#include <napi.h>
#include <memory>


namespace bindings_TS_Orchestrator{
    // Global User list interface
    Napi::Value AddPlayerToGlobalList (const Napi::CallbackInfo&);
    Napi::Value GetGlobalPlayerList(const Napi::CallbackInfo&);
    Napi::Value RemovePlayerFromGlobalList(const Napi::CallbackInfo&);
    Napi::Value GetPlayerDetailsFromGlobalList(const Napi::CallbackInfo&);
    Napi::String PrintGlobalPlayerList(const Napi::CallbackInfo&);
    // Group management interface
    Napi::Value CreateTSGroup(const Napi::CallbackInfo&);
    Napi::Value AddUserToTSGroup(const Napi::CallbackInfo&);
    Napi::Value RemoveUserFromTSGroup(const Napi::CallbackInfo&);
    Napi::Value DeleteGroup(const Napi::CallbackInfo&);
    Napi::Value DeleteGrpSecretSharingContainers(const Napi::CallbackInfo&);
    Napi::Value DeleteGrpSignatureContainers(const Napi::CallbackInfo&);
    Napi::Value DeleteGrpPreSignatureContainers(const Napi::CallbackInfo&);
    Napi::Value lockGroup(const Napi::CallbackInfo&);
    Napi::Value unLockGroup(const Napi::CallbackInfo&);
    
    //Generate a UUID
    Napi::Value GenerateUUID(const Napi::CallbackInfo&);
};
#endif //#ifndef __ORCHESTRATOR_H__