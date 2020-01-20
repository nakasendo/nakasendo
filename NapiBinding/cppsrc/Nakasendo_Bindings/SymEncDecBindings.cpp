#include <napi.h>
#include <iostream>
#include <SymEncDec/SymEncDec.h>


namespace bindings_SymEnc{
    Napi::String encAES (const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if(info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString()){
            Napi::TypeError::New(env, "3 parameters expected of type string").ThrowAsJavaScriptException(); 
        }
        
        Napi::String msg = info[0].As<Napi::String>();
        Napi::String key = info[1].As<Napi::String>();
        Napi::String iv = info[2].As<Napi::String>();
        

        const std::string encMsgHex = Encode(msg, key, iv);
        return Napi::String::New(env, encMsgHex); 
        
    }
    
    Napi::String decAES (const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if(info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString()){
            Napi::TypeError::New(env, "3 parameters expected of type string").ThrowAsJavaScriptException(); 
        }
        
        Napi::String msg = info[0].As<Napi::String>();
        Napi::String key = info[1].As<Napi::String>();
        Napi::String iv = info[2].As<Napi::String>();


        const std::string decMsg = Decode (msg, key, iv);
        return Napi::String::New(env, decMsg);    
    }
    
    Napi::String GenNounce ( const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if (info.Length() != 0){
            Napi::TypeError::New(env,"No parameters expected").ThrowAsJavaScriptException();
        }
        const std::string nounceAsHex = GenerateNounce (); 
        return Napi::String::New(env, nounceAsHex);
    }
    
    Napi::String GenKey (const Napi::CallbackInfo& info){
    
        Napi::Env env = info.Env();
        
        if(info.Length() < 2 || !info[0].IsString() || !info[1].IsString()){
            Napi::TypeError::New(env, "2 parameters expected of type string").ThrowAsJavaScriptException(); 
        }
        
        Napi::String argKey = info[0].As<Napi::String>();
        Napi::String argIV = info[1].As<Napi::String>();
     
        std::string key(argKey);
        std::string iv(argIV);
        

        std::string keyHex = GenerateKey256(key, iv);
        return Napi::String::New(env, keyHex);
    
    }
}
