#include <napi.h>
#include <iostream>
#include <BigNumbers/BigNumbers.h>



namespace BigNumberHelper{

    std::string generate_random(const int size, const bool decimal){
        BigNumber bn = GenerateRand(size); 
        if(decimal){
            return bn.ToDec();
        }else{
            return bn.ToHex(); 
        }
    }
}

namespace bindings_BigNumber{


    Napi::String generate_random_wrapped(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if(info.Length() < 2 || !info[0].IsNumber() || !info[1].IsBoolean()){
            Napi::TypeError::New(env, 
                                 "2 parameters expected: a size of type integer & a boolean indicating hex or dec representation").ThrowAsJavaScriptException(); 
        }
        
        Napi::Number sizeVal = info[0].As<Napi::Number>(); 
        Napi::Boolean dec = info[1].As<Napi::Boolean>();
        std::string retVal = BigNumberHelper::generate_random(sizeVal.Int32Value(),dec); 
        return Napi::String::New(env,retVal); 
    }



    Napi::String bn_add(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() <  3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()){
            Napi::TypeError::New(env, "3 parametrs expected: std::string, std::string, boollean").ThrowAsJavaScriptException();
        }
        
        Napi::String bnAStr = info[0].As<Napi::String>();
        Napi::String bnBStr = info[1].As<Napi::String>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        BigNumber bnA, bnB;
        if(dec){
            bnA.FromDec(bnAStr);
            bnB.FromDec(bnBStr);
        }else{
            bnB.FromHex(bnAStr);
            bnB.FromHex(bnBStr);
        }
        
        const BigNumber res = bnA + bnB;
        std::cout << "RESULT IN NAPI " << res.ToHex() << std::endl; 
        if(dec){
            return Napi::String::New(env, res.ToDec());
        }else{
            return Napi::String::New(env, res.ToHex());
        }
    
    }
    
    Napi::String bn_sub(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() <  3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()){
            Napi::TypeError::New(env, "3 parametrs expected: std::string, std::string, boollean").ThrowAsJavaScriptException();
        }
        
        Napi::String bnAStr = info[0].As<Napi::String>();
        Napi::String bnBStr = info[1].As<Napi::String>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        BigNumber bnA, bnB;
        if(dec){
            bnA.FromDec(bnAStr);
            bnB.FromDec(bnBStr);
        }else{
            bnB.FromHex(bnAStr);
            bnB.FromHex(bnBStr);
        }
        
        const BigNumber res = bnA - bnB;
        if(dec){
            return Napi::String::New(env, res.ToDec());
        }else{
            return Napi::String::New(env, res.ToHex());
        }
    }
    
    Napi::String bn_mul(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() <  3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()){
            Napi::TypeError::New(env, "3 parametrs expected: std::string, std::string, boollean").ThrowAsJavaScriptException();
        }
        
        Napi::String bnAStr = info[0].As<Napi::String>();
        Napi::String bnBStr = info[1].As<Napi::String>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        BigNumber bnA, bnB;
        if(dec){
            bnA.FromDec(bnAStr);
            bnB.FromDec(bnBStr);
        }else{
            bnB.FromHex(bnAStr);
            bnB.FromHex(bnBStr);
        }
        
        const BigNumber res = bnA * bnB;
        if(dec){
            return Napi::String::New(env, res.ToDec());
        }else{
            return Napi::String::New(env, res.ToHex());
        }
    }
    
    Napi::String bn_div(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() <  3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()){
            Napi::TypeError::New(env, "3 parametrs expected: std::string, std::string, boollean").ThrowAsJavaScriptException();
        }
        
        Napi::String bnAStr = info[0].As<Napi::String>();
        Napi::String bnBStr = info[1].As<Napi::String>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        BigNumber bnA, bnB;
        if(dec){
            bnA.FromDec(bnAStr);
            bnB.FromDec(bnBStr);
        }else{
            bnB.FromHex(bnAStr);
            bnB.FromHex(bnBStr);
        }
        
        try{
            const BigNumber res = bnA / bnB;
            if(dec){
                return Napi::String::New(env, res.ToDec());
            }else{
                return Napi::String::New(env, res.ToHex());
            }
        }
        catch(std::exception& re){
            throw Napi::Error::New(env, re.what());
        }
    }
    

    Napi::String bn_mod (const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()){
             Napi::TypeError::New(env, "3 parametrs expected: std::string, std::string, boollean").ThrowAsJavaScriptException();
        }
        Napi::String bnArgStr = info[0].As<Napi::String>();
        Napi::String bnModStr = info[1].As<Napi::String>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        
        BigNumber bnArg, bnMod;
        if(dec){
            bnArg.FromDec(bnArgStr);
            bnMod.FromDec(bnModStr);
            const BigNumber res = bnArg % bnMod;
            return Napi::String::New(env, res.ToDec());
        }else{
            bnArg.FromHex(bnArgStr);
            bnMod.FromHex(bnModStr);
            const BigNumber res = bnArg % bnMod;
            return Napi::String::New(env,res.ToHex());
        }
    }
    
    Napi::String bn_inv_mod (const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()){
             Napi::TypeError::New(env, "3 parametrs expected: std::string, std::string, boollean").ThrowAsJavaScriptException();
        }
        Napi::String bnArgStr = info[0].As<Napi::String>();
        Napi::String bnModStr = info[1].As<Napi::String>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        
        BigNumber bnArg, bnMod;
        if(dec){
            bnArg.FromDec(bnArgStr);
            bnMod.FromDec(bnModStr);
            const BigNumber res = Inv_mod(bnArg,bnMod);
            return Napi::String::New(env, res.ToDec());
        }else{
            bnArg.FromHex(bnArgStr);
            bnMod.FromHex(bnModStr);
            const BigNumber res = Inv_mod(bnArg,bnMod);
            return Napi::String::New(env,res.ToHex());
        }
    }
    
    Napi::String bn_add_mod(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 4 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsBoolean()){
             Napi::TypeError::New(env, "4 parametrs expected: std::string, std::string, std::string , boollean").ThrowAsJavaScriptException();
        }
        Napi::String bnArgAStr = info[0].As<Napi::String>();
        Napi::String bnArgBStr = info[1].As<Napi::String>();
        Napi::String bnModStr = info[2].As<Napi::String>();        
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        
        BigNumber bnA, bnB, bnMod;
        if(dec){
            bnA.FromDec(bnArgAStr);
            bnB.FromDec(bnArgBStr);
            bnMod.FromDec(bnModStr);
            const BigNumber res = Add_mod(bnA, bnB, bnMod);
            return Napi::String::New(env, res.ToDec());
        }else{
            bnA.FromHex(bnArgAStr);
            bnB.FromHex(bnArgBStr);
            bnMod.FromHex(bnModStr);
            const BigNumber res = Add_mod(bnA,bnB,bnMod);
            return Napi::String::New(env, res.ToHex());
        }
        
    }
    Napi::String bn_sub_mod(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 4 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsBoolean()){
             Napi::TypeError::New(env, "4 parametrs expected: std::string, std::string, std::string , boollean").ThrowAsJavaScriptException();
        }
        Napi::String bnArgAStr = info[0].As<Napi::String>();
        Napi::String bnArgBStr = info[1].As<Napi::String>();
        Napi::String bnModStr = info[2].As<Napi::String>();        
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        
        BigNumber bnA, bnB, bnMod;
        if(dec){
            bnA.FromDec(bnArgAStr);
            bnB.FromDec(bnArgBStr);
            bnMod.FromDec(bnModStr);
            const BigNumber res = Sub_mod(bnA, bnB, bnMod);
            return Napi::String::New(env, res.ToDec());
        }else{
            bnA.FromHex(bnArgAStr);
            bnB.FromHex(bnArgBStr);
            bnMod.FromHex(bnModStr);
            const BigNumber res = Sub_mod(bnA,bnB,bnMod);
            return Napi::String::New(env, res.ToHex());
        }
    }
    
    Napi::String bn_mul_mod(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 4 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsBoolean()){
             Napi::TypeError::New(env, "4 parametrs expected: std::string, std::string, std::string , boollean").ThrowAsJavaScriptException();
        }
        Napi::String bnArgAStr = info[0].As<Napi::String>();
        Napi::String bnArgBStr = info[1].As<Napi::String>();
        Napi::String bnModStr = info[2].As<Napi::String>();        
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        
        BigNumber bnA, bnB, bnMod;
        if(dec){
            bnA.FromDec(bnArgAStr);
            bnB.FromDec(bnArgBStr);
            bnMod.FromDec(bnModStr);
            const BigNumber res = Mul_mod(bnA, bnB, bnMod);
            return Napi::String::New(env, res.ToDec());
        }else{
            bnA.FromHex(bnArgAStr);
            bnB.FromHex(bnArgBStr);
            bnMod.FromHex(bnModStr);
            const BigNumber res = Mul_mod(bnA,bnB,bnMod);
            return Napi::String::New(env, res.ToHex());
        }
    }
    
    Napi::String bn_div_mod(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 4 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsBoolean()){
             Napi::TypeError::New(env, "4 parametrs expected: std::string, std::string, std::string , boollean").ThrowAsJavaScriptException();
        }
        Napi::String bnArgAStr = info[0].As<Napi::String>();
        Napi::String bnArgBStr = info[1].As<Napi::String>();
        Napi::String bnModStr = info[2].As<Napi::String>();        
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        
        BigNumber bnA, bnB, bnMod;
        if(dec){
            bnA.FromDec(bnArgAStr);
            bnB.FromDec(bnArgBStr);
            bnMod.FromDec(bnModStr);
            const BigNumber res = Div_mod(bnA, bnB, bnMod);
            return Napi::String::New(env, res.ToDec());
        }else{
            bnA.FromHex(bnArgAStr);
            bnB.FromHex(bnArgBStr);
            bnMod.FromHex(bnModStr);
            const BigNumber res = Div_mod(bnA,bnB,bnMod);
            return Napi::String::New(env, res.ToHex());
        }
    }
    
    Napi::Boolean bn_greater(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()){
            Napi::TypeError::New(env, "3 parameters expected: string, string, boolean").ThrowAsJavaScriptException();
        }
        Napi::String bnAStr = info[0].As<Napi::String>();
        Napi::String bnBStr = info[1].As<Napi::String>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        BigNumber bnA,bnB;
        if(dec){
            bnA.FromDec(bnAStr);
            bnB.FromDec(bnBStr);
        }else{
            bnA.FromHex(bnAStr);
            bnB.FromHex(bnBStr);
        }
        return Napi::Boolean::New(env,bnA > bnB);
    }
    
    Napi::Boolean bn_equal(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()){
            Napi::TypeError::New(env, "3 parameters expected: string, string, boolean").ThrowAsJavaScriptException();
        }
        Napi::String bnAStr = info[0].As<Napi::String>();
        Napi::String bnBStr = info[1].As<Napi::String>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        BigNumber bnA,bnB;
        if(dec){
            bnA.FromDec(bnAStr);
            bnB.FromDec(bnBStr);
        }else{
            bnA.FromHex(bnAStr);
            bnB.FromHex(bnBStr);
        }
        return Napi::Boolean::New(env,bnA == bnB);
    }
}
