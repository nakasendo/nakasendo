#include <vector>
#include <sstream>
#include <OrchestratorWrapper.h>
#include <orchestrator.h>


        
        
namespace bindings_TS_Orchestrator{
    Napi::Value AddPlayerToGlobalList (const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if(info.Length() != 4|| !info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsString()){
            Napi::TypeError::New(env,"4 parameters expected of type stirng in AddPlayer").ThrowAsJavaScriptException();
        }
        player p(info[0].As<Napi::String>(),info[1].As<Napi::String>(),info[2].As<Napi::String>(),info[3].As<Napi::String>());
        addPlayer(p);
        return Napi::Boolean::New(env,true);
    }
    Napi::Value GetGlobalPlayerList(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        std::vector<player> players = getPlayerList();
        int index(0);
        Napi::Array playerarray = Napi::Array::New(env,players.size()*4 );
        for(std::vector<player>::const_iterator iter = players.begin(); iter != players.end(); ++ iter){
            playerarray[index++] = Napi::String::New(env, iter->userID());
            playerarray[index++] = Napi::String::New(env,iter->uri());
            playerarray[index++] = Napi::String::New(env,iter->addr());
            playerarray[index++] = Napi::String::New(env,iter->port());
        }
        return playerarray;
    }

    Napi::String PrintGlobalPlayerList(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        std::stringstream output;
        std::vector<player> players = getPlayerList();
        for(std::vector<player>::const_iterator iter = players.begin();iter != players.end(); ++iter){
            output << iter->userID() << " " << iter->uri() << " " << iter->addr() << " " << iter->port() << std::endl;
        }
        return Napi::String::New(env,output.str());
    }

    Napi::Value RemovePlayerFromGlobalList(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if(info.Length() != 4|| !info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsString()){
            Napi::TypeError::New(env,"4 parameters expected of type stirng in AddPlayer").ThrowAsJavaScriptException();
        }
        player p(info[0].As<Napi::String>(),info[1].As<Napi::String>(),info[2].As<Napi::String>(),info[3].As<Napi::String>());
        removePlayer(p);
        return Napi::Boolean::New(env,true);
    }

    Napi::Value GetPlayerDetailsFromGlobalList(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 1 || !info[0].IsString()){
            Napi::TypeError::New(env,"1 parameter of type string expected in call to GetPlayerDetailsFromGlobalList");
        }
        int index(0);
        Napi::Array playerdetails = Napi::Array::New(env,4);
        try{
            const player& p = getPlayer(info[0].As<Napi::String>()); 
            playerdetails[index++] = Napi::String::New(env,p.userID());
            playerdetails[index++] = Napi::String::New(env,p.uri());
            playerdetails[index++] = Napi::String::New(env,p.addr());
            playerdetails[index] = Napi::String::New(env,p.port());
        }catch(std::exception& e){
            Napi::TypeError::New(env,e.what()).ThrowAsJavaScriptException();
        }
        return playerdetails;
    }

    Napi::Value CreateTSGroup(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 3 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()){
            Napi::TypeError::New(env,"3 parameters expected of type string, integer and integer in call CreateGroup").ThrowAsJavaScriptException();
        }
        return Napi::String::New(info.Env(),CreateGroup(info[0].As<Napi::String>(),info[1].As<Napi::Number>(),info[2].As<Napi::Number>()));
    }

    Napi::Value AddUserToTSGroup(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if( info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
            Napi::TypeError::New(env,"2 parameters of type string expected in call to AddUserToTSGroup").ThrowAsJavaScriptException();
        }
        addUserToGroup(info[0].As<Napi::String>(),info[1].As<Napi::String>());
        return Napi::Boolean::New(info.Env(),true);
    }

    Napi::Value RemoveUserFromTSGroup(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
            Napi::TypeError::New(env,"2 parameters of type string expected in call to RemoveUserFromTSGroup").ThrowAsJavaScriptException();
        }
        return Napi::Boolean::New(info.Env(),removeUserFromGroup(info[0].As<Napi::String>(),info[1].As<Napi::String>()));
    }

    Napi::Value DeleteGroup(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 1 || !info[0].IsString()){
            Napi::TypeError::New(env,"A single parameter of type string expected in call to DeleteGroup").ThrowAsJavaScriptException();
        }
        return Napi::Boolean::New(info.Env(),deleteGroup(info[0].As<Napi::String>()));
    }
    Napi::Value DeleteGrpSecretSharingContainers(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 1 || !info[0].IsString()){
            Napi::TypeError::New(env,"A single parameter of type string expected in call to DeleteGrpSecretSharingContainers").ThrowAsJavaScriptException();
        }
        return Napi::Boolean::New(info.Env(),DeleteGrpSharingContainers(info[0].As<Napi::String>()));
    }

    Napi::Value DeleteGrpSignatureContainers(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 1 || !info[0].IsString()){
            Napi::TypeError::New(env,"A single parameter of type string expected in call to DeleteGrpSignatureContainers").ThrowAsJavaScriptException();
        }
        return Napi::Boolean::New(info.Env(),deleteGrpSigSharingContainers(info[0].As<Napi::String>()));
    }

    Napi::Value DeleteGrpPreSignatureContainers(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 1 || !info[0].IsString()){
            Napi::TypeError::New(env,"A single parameter of type string expected in call to deleteGrpPreSigContainers").ThrowAsJavaScriptException();
        }
        return Napi::Boolean::New(info.Env(),deleteGrpPreSigContainers(info[0].As<Napi::String>()));
    }

    Napi::Value lockGroup(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 1 || !info[0].IsString()){
            Napi::TypeError::New(env,"A single parameter of type string expected in call to lockGroup").ThrowAsJavaScriptException();
        }
        return Napi::Boolean::New(info.Env(),lockGrp(info[0].As<Napi::String>()));
    }
    Napi::Value unLockGroup(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 1 || !info[0].IsString()){
            Napi::TypeError::New(env,"A single parameter of type string expected in call to unLockGroup").ThrowAsJavaScriptException();
        }
        return Napi::Boolean::New(info.Env(),unlockGrp(info[0].As<Napi::String>()));
    }
    
    //Generate a UUID
    Napi::Value GenerateUUID(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() == 0){
            return Napi::String::New(env,generateUUID());
        }else{
            if(info.Length() != 1 || !info[0].IsString()){
                Napi::TypeError::New(env,"A single parameter of type integer expected in call to GenerateUUID").ThrowAsJavaScriptException();
            }
            return Napi::String::New(env,generateUUID(info[0].As<Napi::Number>()));
        }
        return Napi::Boolean::New(info.Env(),true);
    }
}

