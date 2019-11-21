#include <napi.h>
#include <iostream>
#include <ECPoint/ECPoint.h>


namespace ECPointHelper{
     std::unique_ptr<ECPoint> makeECPoint(const int& curveID)
    {
        std::unique_ptr<ECPoint> pointPtr ;
        if (curveID == 0) {
            // use defualt constructor
            pointPtr =  std::unique_ptr<ECPoint> (new ECPoint()) ;
        }
        else{
            pointPtr =  std::unique_ptr< ECPoint> ( new ECPoint(curveID)); 
        }
        return pointPtr ;
    }
    
    
    
    std::string generateRandomEC(const int nid, const bool decimal, const bool isCompressed){

        std::unique_ptr<ECPoint> ecpt = makeECPoint(nid);
        ecpt->SetRandom();
        if(decimal){
            return ecpt->ToDec(isCompressed);
        }else{
            return ecpt->ToHex(isCompressed);
        }
    }
    
    std::string GroupOrder(const int nid, const bool decimal){
        std::unique_ptr<ECPoint> ecpt = makeECPoint(nid);
        ecpt->SetRandom();
        BigNumber bnVal = ecpt->getECGroupOrder();
        if(decimal){
            return bnVal.ToDec();
        }else{
            return bnVal.ToHex();
        }
    }
    
    int GroupDegree(const int nid, const bool decimal){
        std::unique_ptr<ECPoint> ecpt = makeECPoint(nid);
        ecpt->SetRandom();
        int degree = ecpt->getECGroupDegree();
        return degree;
    }
    
    std::string MulbyGen(const std::string& bnpoint, const int& nid, const bool& decimal, const bool& compressed){
        std::unique_ptr<ECPoint> genpt = makeECPoint(nid);
        ECPoint GEN = genpt->getGenerator();
        
        if(decimal){
            ECPoint res = GEN.MulDec(bnpoint, std::string());
            return res.ToDec(compressed);
        }else{
            ECPoint res = GEN.MulHex(bnpoint, std::string());
            return res.ToHex(compressed);
        }
    }
    
    std::string AddECPoint(const std::string bnpointA,const std::string bnpointB,const int nid, const bool dec, const bool compressed){
        std::unique_ptr<ECPoint> ecptA = makeECPoint(nid);
        std::unique_ptr<ECPoint> ecptB = makeECPoint(nid);
         
        if(dec){
            ecptA.get()->FromDec(bnpointA);
            ecptB.get()->FromDec(bnpointB);
            
        }else{
            ecptA->FromHex(bnpointA);
            ecptB->FromHex(bnpointB);
        }
        
        ECPoint C = *ecptA + *ecptB; 
        if(dec){
            return C.ToDec(compressed);
        }else{
            return C.ToHex(compressed);
        }
    }
    
    std::string mulByScalarM(const std::string& ecptrStr,const std::string& bnptrStr,const int& nid, const bool& dec, const bool& compressed){
        std::unique_ptr<ECPoint> ecptA = makeECPoint(nid);
        ECPoint res; 
        
        if(dec){
            ecptA->FromDec(ecptrStr);
            res = ecptA->MulDec(bnptrStr,std::string());
            return res.ToDec(compressed);
        }else{
            ecptA->FromHex(ecptrStr);
            res = ecptA->MulHex(bnptrStr,std::string());
            return res.ToHex(compressed);
        }
    }
    
    bool comp(const std::string& ecpStrA, const std::string& ecpStrB, const int& nid, const bool& dec){
        std::unique_ptr<ECPoint> ecpA = makeECPoint(nid);
        std::unique_ptr<ECPoint> ecpB = makeECPoint(nid);
        
        if(dec){
            ecpA->FromDec(ecpStrA);
            ecpB->FromDec(ecpStrB);
        }else{
            ecpA->FromHex(ecpStrA,nid);
            ecpB->FromHex(ecpStrB,nid);
        }
        return (*ecpA.get() == *ecpB.get());
    }
    
    std::string doublept(const std::string& ecpStr, const int& nid, const bool& dec, const bool& compressed){
        std::unique_ptr<ECPoint> ecp = makeECPoint(nid);
        if(dec){
            ecp->FromDec(ecpStr);
        }else{
            ecp->FromHex(ecpStr);
        }
        ECPoint res = ecp->Double();
        if(dec){
            return res.ToDec(compressed);
        }else{
            return res.ToHex(compressed);
        }
    }
    
    std::string invertpt(const std::string& ecpStr, const int& nid, const bool& dec, const bool& compressed){
        std::unique_ptr<ECPoint> ecp = makeECPoint(nid);
        if(dec){
            ecp->FromDec(ecpStr);
        }else{
            ecp->FromHex(ecpStr);
        }
        
        ecp->Invert();
        
        if(dec){
            return ecp->ToDec(compressed);
        }else{
            return ecp->ToHex(compressed);
        }
    }
    
}


namespace bindings_ECPoint{

    Napi::String generateRandomECWrapper(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 3 || !info[0].IsNumber() || !info[1].IsBoolean() || !info[2].IsBoolean()){
            Napi::TypeError::New(env,"3 parameters expected of type integer, boolean and boolean").ThrowAsJavaScriptException();
        }
        Napi::Number nid = info[0].As<Napi::Number>();
        Napi::Boolean dec = info[1].As<Napi::Boolean>();
        Napi::Boolean compressed = info[2].As<Napi::Boolean>();
        
        std::string retval = ECPointHelper::generateRandomEC(nid.Int32Value(), dec, compressed);
        return Napi::String::New(env,retval);
    }
    
    
    
    Napi::String GetGroupOrder(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 2 || !info[0].IsNumber() || !info[1].IsBoolean()){
            Napi::TypeError::New(env, "2 parameters expected of type integer and boolean").ThrowAsJavaScriptException();
        }
        Napi::Number nid = info[0].As<Napi::Number>();
        Napi::Boolean dec = info[1].As<Napi::Boolean>();
        
        std::string order = ECPointHelper::GroupOrder(nid.Int32Value(),dec);
        return Napi::String::New(env,order);
    }
    
    Napi::Number GetGroupDegree(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 2 || !info[0].IsNumber() || !info[1].IsBoolean()){
            Napi::TypeError::New(env,"2 parameters expected of type integer and boolean").ThrowAsJavaScriptException();
        }
        Napi::Number nid = info[0].As<Napi::Number>();
        Napi::Boolean dec = info[1].As<Napi::Boolean>();
        
        int degree = ECPointHelper::GroupDegree(nid.Int32Value(), dec);
        return Napi::Number::New(env,degree);
    }
    
    
    Napi::String MultiplyByGenerator(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        
        if(info.Length() < 4 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsBoolean() || !info[3].IsBoolean()){
            Napi::TypeError::New(env,"4 parameters expected of type string, integer, boolean, boolean").ThrowAsJavaScriptException();
        }
        
        Napi::String bnpoint = info[0].As<Napi::String>();
        Napi::Number nid = info[1].As<Napi::Number>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        Napi::Boolean compressed = info[3].As<Napi::Boolean>();
        
        std::string res = ECPointHelper::MulbyGen(bnpoint, nid.Int32Value(),dec,compressed);
        return Napi::String::New(env,res);
    }
    
    Napi::Boolean CheckonCurve(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 3 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsBoolean()){
            Napi::TypeError::New(env, "3 parameters expected of type string, integer, boolean").ThrowAsJavaScriptException();
        }
        
        Napi::String bnpoint = info[0].As<Napi::String>();
        Napi::Number nid = info[1].As<Napi::Number>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        std::unique_ptr<ECPoint> pt = ECPointHelper::makeECPoint(nid);
        if(dec){
            pt->FromDec(bnpoint);
        }else{
            pt->FromHex(bnpoint);
        }
        return Napi::Boolean::New(env,pt->CheckOnCurve());
    }
    
    Napi::String Add(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 5 || !info[0].IsString() || !info[1].IsString() || !info[2].IsNumber() || !info[3].IsBoolean() || !info[4].IsBoolean()){
            Napi::TypeError::New(env, "5 parameters expected of type string,string, integer, boolean,boolean").ThrowAsJavaScriptException();
        }
        Napi::String bnpointA = info[0].As<Napi::String>();
        Napi::String bnpointB = info[1].As<Napi::String>();
        Napi::Number nid = info[2].As<Napi::Number>();
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        Napi::Boolean compressed = info[4].As<Napi::Boolean>();
        
        std::string retval = ECPointHelper::AddECPoint(bnpointA,bnpointB,nid.Int32Value(), dec, compressed);
       
        return Napi::String::New(env,retval);
    }
    
    Napi::String multiplyScalar(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 5 ||!info[0].IsString() || !info[1].IsString() || !info[2].IsNumber() || !info[3].IsBoolean() || !info[4].IsBoolean()){
            Napi::TypeError::New(env,"5 parameters expected of type string, string, integer, boolean, boolean").ThrowAsJavaScriptException();
        }
        
        Napi::String ecptStr = info[0].As<Napi::String>();
        Napi::String bnptrStr = info[1].As<Napi::String>();
        Napi::Number nid = info[2].As<Napi::Number>();
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        Napi::Boolean compressed = info[4].As<Napi::Boolean>();
        
        std::string retval = ECPointHelper::mulByScalarM(ecptStr, bnptrStr, nid.Int32Value(), dec, compressed);
        return Napi::String::New(env,retval);
    }
    
    Napi::Value GetAffineCoOrdinates(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 3 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsBoolean()){
            Napi::TypeError::New(env, "3 parameters expected of type string, integer, boolean").ThrowAsJavaScriptException();
        }
        
        Napi::String ecpStr = info[0].As<Napi::String>();
        Napi::Number nid = info[1].As<Napi::Number>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        
        std::pair<std::string, std::string> coords;
        std::unique_ptr<ECPoint> ecptA = ECPointHelper::makeECPoint(nid);
        if(dec){
            ecptA->FromDec(ecpStr);
            coords = ecptA->GetAffineCoords_GFp_Dec();
        }else{
            ecptA->FromHex(ecpStr);
            coords = ecptA->GetAffineCoords_GFp ();
        }
        
        
        Napi::Object affineCoOrds = Napi::Object::New(env);
        affineCoOrds.Set ("x", Napi::String::New(env,coords.first));
        affineCoOrds.Set ("y", Napi::String::New(env,coords.second));
        
        return affineCoOrds;
    }


    Napi::Boolean Compare(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 4 || !info[0].IsString() || !info[1].IsString() || !info[2].IsNumber() || !info[3].IsBoolean()){
            Napi::TypeError::New(env, "4 parameters expected of type string, string, integer, boolean").ThrowAsJavaScriptException();
        }
        
        Napi::String ecpAStr = info[0].As<Napi::String>();
        Napi::String ecpBStr = info[1].As<Napi::String>();
        Napi::Number nid = info[2].As<Napi::Number>();
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        
        return Napi::Boolean::New(env, ECPointHelper::comp(ecpAStr, ecpBStr, nid, dec));
    }
    
    Napi::String Double(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 4 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsBoolean() || !info[3].IsBoolean()){
            Napi::TypeError::New(env,"4 parameters expected of type string, integer, boolean, boolean").ThrowAsJavaScriptException();
        }
        Napi::String ecpStr = info[0].As<Napi::String>();
        Napi::Number nid = info[1].As<Napi::Number>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        Napi::Boolean compressed = info[3].As<Napi::Boolean>();
        
        return Napi::String::New(env, ECPointHelper::doublept(ecpStr,nid,dec,compressed));
        
    }
    Napi::String Invert(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 4 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsBoolean() || !info[3].IsBoolean()){
            Napi::TypeError::New(env,"4 parameters expected of type string, integer, boolean, boolean").ThrowAsJavaScriptException();
        }
        Napi::String ecpStr = info[0].As<Napi::String>();
        Napi::Number nid = info[1].As<Napi::Number>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        Napi::Boolean compressed = info[3].As<Napi::Boolean>();
        
        return Napi::String::New(env, ECPointHelper::invertpt(ecpStr,nid,dec,compressed));
        
    }

}
