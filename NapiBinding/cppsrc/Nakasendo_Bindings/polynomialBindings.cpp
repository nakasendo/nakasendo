#include <napi.h>
#include <iostream>
#include <Polynomial/Polynomial.h>
#include <Polynomial/LGInterpolator.h>
#include <ECPoint/ECPoint.h>

namespace PolynomialHelper{
    std::vector<std::string> randomPoly(const int degree, const std::string mod, const bool dec){    
        BigNumber modulo ;
        if ( dec ){    
            modulo.FromDec( mod ) ;
        }
        else{    
            modulo.FromHex( mod ) ;
        }
        
        
        // construct using degree and mod
        Polynomial poly = Polynomial( degree, modulo ) ;
        
        std::vector<std::string> retval;
        std::vector<BigNumber> coeffs =  poly.getCoefficients();
        for(std::vector<BigNumber>::const_iterator iter = coeffs.begin(); iter != coeffs.end(); ++iter){
            if(dec){
                retval.push_back(iter->ToDec());
            }else{
                retval.push_back(iter->ToHex());
            }
        }
        return retval;    
    }

    std::string evalPolynomial(const std::vector<std::string> poly, const std::string xVal, const std::string mod, const bool dec){
        std::vector<BigNumber> bnCoeffs; 
        for (std::vector<std::string>::const_iterator iter = poly.begin(); iter != poly.end(); ++ iter){
            BigNumber bn;
            if(dec){
                bn.FromDec(*iter);
            }else{
                bn.FromHex(*iter);
            }
            bnCoeffs.push_back(bn); 
        }
        BigNumber modulo; 
        BigNumber bnXval; 
        if(dec){
            modulo.FromDec(mod);
            bnXval.FromDec(xVal);
        }else{
            modulo.FromHex(mod); 
            bnXval.FromHex(xVal); 
        }
        
        Polynomial bnPoly = Polynomial( bnCoeffs, modulo ) ;
        
        
        BigNumber eval = bnPoly(bnXval); 
        if(dec){
            return eval.ToDec(); 
        }else{
            return eval.ToHex(); 
        }
    }
    
    std::string LGInterpolatorFull(const std::vector<std::pair<std::string, std::string> > xfx, const std::string xVal, const std::string mod, const bool dec){

        std::vector<std::pair<BigNumber, BigNumber> > xfxBn;
        BigNumber bnxVal;
        BigNumber modulo;
        
        for(std::vector<std::pair<std::string,std::string> >::const_iterator iter = xfx.begin(); iter != xfx.end(); ++ iter){
            BigNumber bnx;
            BigNumber bny;
            if(dec){
                bnx.FromDec(iter->first);
                bny.FromDec(iter->second);
                xfxBn.push_back(std::make_pair(bnx,bny));
            }else{
                bnx.FromHex(iter->first);
                bny.FromHex(iter->second);
                xfxBn.push_back(std::make_pair(bnx,bny));
            }
        }
        if(dec){
            bnxVal.FromDec(xVal);
            modulo.FromDec(mod);
            
        }else{
            bnxVal.FromHex(xVal);
            modulo.FromHex(mod);
        }
        
        LGInterpolator lgInterpolator ( xfxBn, modulo ) ;
        BigNumber val = lgInterpolator( bnxVal ) ;
        
        if(dec){
            return val.ToDec();
        }else{
            return val.ToHex();
        }
        
    }
    
    
    std::string LGInterpolateSingle(const std::vector<std::pair<std::string, std::string> > xfx, const std::string xVal, const std::string mod, const bool dec, const int basisPoint){
        std::vector<std::pair<BigNumber, BigNumber> > xfxBn;
        BigNumber bnxVal;
        BigNumber modulo;
        
        for(std::vector<std::pair<std::string,std::string> >::const_iterator iter = xfx.begin(); iter != xfx.end(); ++ iter){
            BigNumber bnx;
            BigNumber bny;
            if(dec){
                bnx.FromDec(iter->first);
                bny.FromDec(iter->second);
                xfxBn.push_back(std::make_pair(bnx,bny));
            }else{
                bnx.FromHex(iter->first);
                bny.FromHex(iter->second);
                xfxBn.push_back(std::make_pair(bnx,bny));
            }
        }
        if(dec){
            bnxVal.FromDec(xVal);
            modulo.FromDec(mod);
            
        }else{
            bnxVal.FromHex(xVal);
            modulo.FromHex(mod);
        }
        
        LGInterpolator lgInterpolator ( xfxBn, modulo ) ;
        BigNumber val = lgInterpolator(basisPoint, bnxVal ) ;
        
        if(dec){
            return val.ToDec();
        }else{
            return val.ToHex();
        }
    }
    
    
    std::string ECLGInterpolatorFull(const std::vector<std::pair<std::string, std::pair<std::string,std::string> > > xfx, const std::string xVal, const std::string mod, const bool dec){

        std::vector<std::pair<BigNumber, ECPoint> > xfxbn;
        BigNumber bnxVal;
        BigNumber modulo;
        
        for(std::vector<std::pair<std::string,std::pair<std::string,std::string> > >::const_iterator iter = xfx.begin(); iter != xfx.end(); ++ iter){
            BigNumber bnx;
            BigNumber bna;
            BigNumber bnb;
            
            if(dec){
                bnx.FromDec(iter->first);
                bna.FromDec(iter->second.first);
                bnb.FromDec(iter->second.second);
             
            }else{
                bnx.FromHex(iter->first);
                bna.FromHex(iter->second.first);
                bnb.FromHex(iter->second.second);
            }
            ECPoint y(bna, bnb);
            xfxbn.push_back(std::make_pair(bnx, y));
            
        }
        if(dec){
            bnxVal.FromDec(xVal);
            modulo.FromDec(mod);
            
        }else{
            bnxVal.FromHex(xVal);
            modulo.FromHex(mod);
        }
        
        LGECInterpolator eclgInterpolator ( xfxbn, modulo ) ;
        ECPoint val = eclgInterpolator(bnxVal ) ;
        
        if(dec){
            return val.ToDec();
        }else{
            return val.ToHex();
        }
        
    }

}

namespace bindings_Polynomial{

    Napi::Array randomPolynomial(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        if(info.Length() < 3 || !info[0].IsNumber() || !info[1].IsString() || !info[2].IsBoolean()){
            Napi::TypeError::New(env, " 3 parameters expected: A degree of type integer, a modulo of type std::string and a boolean indicating decimal or hex representation").ThrowAsJavaScriptException();
        }
        Napi::Number degree = info[0].As<Napi::Number>();
        Napi::String mod = info[1].As<Napi::String>();
        Napi::Boolean dec = info[2].As<Napi::Boolean>();
        
        std::vector<std::string> poly = PolynomialHelper::randomPoly(degree,mod,dec);
        Napi::Array poly_array = Napi::Array::New(env, poly.size());
        int index(0);
        for(std::vector<std::string>::const_iterator iter = poly.begin(); iter != poly.end(); ++iter){
             poly_array[index++] = Napi::String::New(env, *iter);
        }
        return poly_array; 
    }


    Napi::String evalulatePolynomial(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if (info.Length() < 4 || !info[0].IsArray() || !info[1].IsString() || !info[2].IsString() || !info[3].IsBoolean()){
            Napi::TypeError::New(env, " 4 params expected of type, Array, string, string, boolean").ThrowAsJavaScriptException();
        }
        
        Napi::Array poly = info[0].As<Napi::Array>();
        Napi::String xval = info[1].As<Napi::String>();
        Napi::String mod = info[2].As<Napi::String>();
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        
        int arrayLen = poly.Length();
        std::vector<std::string> polyAsStr; 
        for(int i = 0;i < arrayLen; ++i){
            Napi::Value v = poly[i];
            if(v.IsString()){
                Napi::String coeff = v.As<Napi::String>(); 
                polyAsStr.push_back(coeff);
            }else{
                Napi::TypeError::New(env, "Unexpected type passed in the polynomial coefficient").ThrowAsJavaScriptException();
            }
        }
        
        std::string retVal = PolynomialHelper::evalPolynomial(polyAsStr, xval,mod,dec); 
        return Napi::String::New(env,retVal); 
    }
    
    Napi::String LGInterpolatorFullWrapper(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if(info.Length() < 4 || !info[0].IsArray() || !info[1].IsString() || !info[2].IsString() || !info[3].IsBoolean()){
            Napi::TypeError::New(env, "4 parameters expected of type Array, string, string, boolean").ThrowAsJavaScriptException();
        }
        Napi::Array xfx = info[0].As<Napi::Array>();
        Napi::String xval = info[1].As<Napi::String>();
        Napi::String mod = info[2].As<Napi::String>();
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        
     
       
        int arrayLen = xfx.Length();
        std::vector<std::pair<std::string, std::string> > xfxAsStr;
        for(int i=0;i<arrayLen;++i){
            Napi::Value v = xfx[i];
            if(v.IsArray()){
                Napi::Array xfxVals = v.As<Napi::Array>();
                if(xfxVals.Length() != 2){
                    Napi::TypeError::New(env,"xfx should be a pair of numbers...invalid construction").ThrowAsJavaScriptException();
                }
                int index=0;
                Napi::Value val1 = xfxVals[index];
                ++index;
                Napi::Value val2 = xfxVals[index];
                if(val1.IsString() && val2.IsString()){
                    Napi::String x = val1.As<Napi::String>();
                    Napi::String y = val2.As<Napi::String>();
                    xfxAsStr.push_back(std::make_pair(x,y));
                    
                }
            }
        }
        
        std::string retval = PolynomialHelper::LGInterpolatorFull(xfxAsStr,xval,mod,dec) ;
        return Napi::String::New(env,retval);
    }

    Napi::String LGInterpolateSingleWrapper(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env();
        
        if(info.Length() < 5 || !info[0].IsArray() || !info[1].IsString() || !info[2].IsString() || !info[3].IsBoolean() || !info[4].IsNumber()){
            Napi::TypeError::New(env, "5 parameters expected of type Array, string, string, boolean, int").ThrowAsJavaScriptException();
        }
        Napi::Array xfx = info[0].As<Napi::Array>();
        Napi::String xval = info[1].As<Napi::String>();
        Napi::String mod = info[2].As<Napi::String>();
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        Napi::Number basisPt = info[4].As<Napi::Number>();
        
     
       
        int arrayLen = xfx.Length();
        std::vector<std::pair<std::string, std::string> > xfxAsStr;
        for(int i=0;i<arrayLen;++i){
            Napi::Value v = xfx[i];
            if(v.IsArray()){
                Napi::Array xfxVals = v.As<Napi::Array>();
                if(xfxVals.Length() != 2){
                    Napi::TypeError::New(env,"xfx should be a pair of numbers...invalid construction").ThrowAsJavaScriptException();
                }
                int index=0;
                Napi::Value val1 = xfxVals[index];
                ++index;
                Napi::Value val2 = xfxVals[index];
                if(val1.IsString() && val2.IsString()){
                    Napi::String x = val1.As<Napi::String>();
                    Napi::String y = val2.As<Napi::String>();
                    xfxAsStr.push_back(std::make_pair(x,y));
                    
                }
            }
        }
        std::string retval = PolynomialHelper::LGInterpolateSingle(xfxAsStr,xval,mod,dec,basisPt.Int32Value());
        return Napi::String::New(env,retval);
    }
    
    Napi::String ECLGInterpolater(const Napi::CallbackInfo& info){
        Napi::Env env = info.Env(); 
        if(info.Length() < 4 || !info[0].IsArray() || !info[1].IsString() || !info[2].IsString() || !info[3].IsBoolean()){
            Napi::TypeError::New(env, "4 parameters expected of type Array, string, string, boolean").ThrowAsJavaScriptException();
        }
        Napi::Array xfx = info[0].As<Napi::Array>();
        Napi::String xval = info[1].As<Napi::String>();
        Napi::String mod = info[2].As<Napi::String>();
        Napi::Boolean dec = info[3].As<Napi::Boolean>();
        
     
       
        int arrayLen = xfx.Length();
        std::vector<std::pair<std::string, std::pair<std::string, std::string> > > xfxAsStr;
        for(int i=0;i<arrayLen;++i){
            Napi::Value v = xfx[i];
            if(v.IsArray()){
                Napi::Array xfxVals = v.As<Napi::Array>();
                if(xfxVals.Length() != 3){
                    Napi::TypeError::New(env,"xfx should be 3 values (string, string, string)...invalid construction").ThrowAsJavaScriptException();
                }
                int index=0;
                Napi::Value val1 = xfxVals[index];
                ++index;
                Napi::Value val2 = xfxVals[index];
                ++index ;
                Napi::Value val3 = xfxVals[index];
                if(val1.IsString() && val2.IsString() && val3.IsString()){
                    Napi::String x = val1.As<Napi::String>();
                    Napi::String a = val2.As<Napi::String>();
                    Napi::String b = val3.As<Napi::String>();
                    
                    xfxAsStr.push_back(std::make_pair(x,std::make_pair(a,b)));
                    
                }
            }
        }
        
        std::string retval = PolynomialHelper::ECLGInterpolatorFull(xfxAsStr,xval,mod,dec) ;
        return Napi::String::New(env,retval);
    }

}
