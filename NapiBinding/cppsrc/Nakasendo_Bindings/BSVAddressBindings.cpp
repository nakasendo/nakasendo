#include <napi.h>
#include <sstream>
#include <iostream>
#include <BSVAddress/BSVAddress.h>
#include <BSVAddress/BSVAddressInfo.h>


namespace bindings_BSVAddress{


    Napi::Array createBSVAddress(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
            Napi::TypeError::New(env,"2 parameters expected of type string and integer");
        }
        
        Napi::String key = info[0].As<Napi::String>();
        Napi::Number version = info[1].As<Napi::Number>();
        
        int ver = version.Int32Value();
  
        
        VersionPrefix versionPrefix = static_cast<VersionPrefix> (ver) ; 
        

        std::string keyStr(key);
        // construct using key and version prefix    
        BSVAddress address( key, versionPrefix ) ;
        
        Napi::Array address_array = Napi::Array::New(env, 5);
        std::pair<std::string, std::string> versionPair = address.getVersion();
        
        int index(0);
        address_array[index++] = Napi::String::New(env, address.getAddress());
        address_array[index++] = Napi::Boolean::New(env, address.valid());
        address_array[index++] = Napi::String::New(env, address.decode());
        address_array[index++] = Napi::String::New(env, address.getVersion().first);
        address_array[index++] = Napi::String::New(env, address.getVersion().second);
        return address_array;
    }
    
    
    Napi::Array importBSVAddress(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() != 1){
            Napi::TypeError::New(env,"1 parameter expected of type string");
        }
        
        Napi::String key = info[0].As<Napi::String>();
        


        // construct using key and version prefix    
        BSVAddress address( key ) ;
        Napi::Array address_array = Napi::Array::New(env, 5);
        std::pair<std::string, std::string> versionPair = address.getVersion();
        
        int index(0);
        address_array[index++] = Napi::String::New(env, address.getAddress());
        address_array[index++] = Napi::Boolean::New(env, address.valid());
        address_array[index++] = Napi::String::New(env, address.decode());
        address_array[index++] = Napi::String::New(env, address.getVersion().first);
        address_array[index++] = Napi::String::New(env, address.getVersion().second);
        return address_array;
    }
    
    Napi::String printBSVAddress(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length () != 1 || !info[0].IsString()){
            Napi::TypeError::New(env,"1 parameter expected of type string");
        }
        
        Napi::String addressStr = info[0].As<Napi::String>();
        BSVAddress addr(addressStr);
        std::stringstream ss; 
        ss << addr << std::endl;
        return Napi::String::New(env, ss.str()); 
    }
}
