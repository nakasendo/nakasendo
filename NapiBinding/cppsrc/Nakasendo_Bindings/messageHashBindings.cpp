#include <napi.h>
#include <iostream>
#include <MessageHash/MessageHash.h>
#include <MessageHash/Base64EncDec.h>
#include <MessageHash/Base58EncDec.h>



namespace bindings_MessageHash{
    // have to be chainable
    Napi::String SHA256 (const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if(info.Length() < 1 || !info[0].IsString()){
            Napi::TypeError::New(env, "1 parameter expected of type string").ThrowAsJavaScriptException(); 
        }
        Napi::String arg = info[0].As<Napi::String>();
        std::string hashedVal = HashMsgSHA256 (arg);
        return Napi::String::New(env, hashedVal);
    }
    
    Napi::String RIPEMD160 (const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if(info.Length() < 1 || !info[0].IsString()){
            Napi::TypeError::New(env, "1 parameter expected of type string").ThrowAsJavaScriptException(); 
        }
        Napi::String crMsg = info[0].As<Napi::String>();
        std::string hashfunc ("ripemd160");
        MessageHash hashMess;
        
        hashMess.Hash (crMsg, hashfunc);
        std::string hashedVal =  hashMess.HashHex();
        
        return Napi::String::New(env, hashedVal);
    }
    
    Napi::String Hash (const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 2 || !info[0].IsString() || !info[1].IsString()){
            Napi::TypeError::New(env, "2 parameters expected of type string").ThrowAsJavaScriptException(); 
        }
        Napi::String crMsg = info[0].As<Napi::String>();
        Napi::String hashfunc = info[1].As<Napi::String>();
        
        MessageHash hashMess;
        
        hashMess.Hash (crMsg, hashfunc);
        std::string hashedVal =  hashMess.HashHex();
        
        return Napi::String::New(env, hashedVal);
    }
    
    Napi::String EncB64(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 1 || !info[0].IsString()){
            Napi::TypeError::New(env, "1 parameter expected of type string").ThrowAsJavaScriptException(); 
        }
        Napi::String crMsg = info[0].As<Napi::String>();
        
        std::string crMsgStr (crMsg); 
        
        std::unique_ptr<unsigned char[]> msgValptr = std::make_unique<unsigned char[]>(crMsgStr.size() + 1);

        for (unsigned int i = 0;i < crMsgStr.size(); ++i){
            msgValptr.get()[i] = crMsgStr[i]; 
        }
        msgValptr.get()[crMsgStr.size()]='\0';

        std::string encoded = EncodeBase64(msgValptr,crMsgStr.size());
        
        return Napi::String::New(env,encoded);
    }
    
    Napi::String decB64(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 1 || !info[0].IsString()){
            Napi::TypeError::New(env, "1 parameter expected of type string").ThrowAsJavaScriptException(); 
        }
        Napi::String crMsg = info[0].As<Napi::String>();
        
        std::string crMsgStr (crMsg); 
        
        
        int msgLen(0);
        std::string result;
        std::unique_ptr<unsigned char[]> resultAsBytes = DecodeBase64(crMsgStr,msgLen);
        for(int i=0;i<msgLen;++i){
            result.push_back(resultAsBytes.get()[i]);
        }
        
        return Napi::String::New(env,result);
    }
    
    Napi::String EncB58(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 1 || !info[0].IsString()){
            Napi::TypeError::New(env, "1 parameter expected of type string").ThrowAsJavaScriptException(); 
        }
        Napi::String crMsg = info[0].As<Napi::String>();
        
        

        std::string encoded = EncodeBase58(crMsg);
        
        return Napi::String::New(env,encoded);
    }
    
    Napi::String DecB58(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
         if(info.Length() < 1 || !info[0].IsString()){
            Napi::TypeError::New(env, "1 parameter expected of type string").ThrowAsJavaScriptException(); 
        }
        Napi::String crMsg = info[0].As<Napi::String>();        
        std::string decoded = DecodeBase58(crMsg);
        
        return Napi::String::New(env,decoded);
    }
    
    
    Napi::String EncB58Checked(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 1 || !info[0].IsString()){
            Napi::TypeError::New(env, "1 parameter expected of type string").ThrowAsJavaScriptException(); 
        }
        Napi::String crMsg = info[0].As<Napi::String>();
        
        

        std::string encoded = EncodeBase58Checked(crMsg);
        
        return Napi::String::New(env,encoded);
    }
    
    Napi::String DecB58Checked(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 1 || !info[0].IsString()){
            Napi::TypeError::New(env, "1 parameter expected of type string").ThrowAsJavaScriptException(); 
        }
        Napi::String crMsg = info[0].As<Napi::String>();
        
        

        std::string decoded = DecodeBase58Checked(crMsg);
        
        return Napi::String::New(env,decoded);
    }
    
}
