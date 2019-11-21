#include <napi.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>
#include <AsymKey/AsymKeyConfig.h>
#include <AsymKey/AsymKey.h>
#include <BigNumbers/BigNumbers.h>
#include <MessageHash/conversions.h>
#include <SecretSplit/KeyShare.h>


namespace bindings_AsymKey{
        Napi::Array GenKeyPairPem (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env(); 
            if(info.Length() != 0 ){
                Napi::TypeError::New(env,"No parameters expected").ThrowAsJavaScriptException();
            }
            AsymKey keyGen;
            const std::pair<std::string, std::string> keyPairPEM = std::make_pair(keyGen.exportPublicPEM(), keyGen.exportPrivatePEM());
            Napi::Array key_array = Napi::Array::New(env, 2);
            int index(0); 
            key_array[index++] = Napi::String::New(env,keyPairPEM.first); 
            key_array[index] = Napi::String::New(env, keyPairPEM.second); 
            return key_array; 
            
        }
        Napi::Array GenKeyPairHex (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env();
            if(info.Length() != 0){
                Napi::TypeError::New(env,"No parameters expected").ThrowAsJavaScriptException();
            }
            AsymKey keyGen;
            const std::pair<std::string, std::string> keyPairHEX = std::make_pair(keyGen.exportPublicHEXStr(), keyGen.exportPrivateHEX());
            Napi::Array key_array = Napi::Array::New(env, 2);
            int index(0); 
            key_array[index++] = Napi::String::New(env,keyPairHEX.first); 
            key_array[index] = Napi::String::New(env, keyPairHEX.second); 
            return key_array; 
        }
        Napi::Array ExportKeysHex (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env();
            if(info.Length() != 1 || !info[0].IsString()){
                Napi::TypeError::New(env, "1 parameter expected of type string").ThrowAsJavaScriptException();
            }
            Napi::String importedKeyStr = info[0].As<Napi::String>();
            AsymKey imported_key;
            imported_key.importPrivatePEM(importedKeyStr);
            const std::pair<std::string, std::string> keyPairHEX = std::make_pair(imported_key.exportPublicHEXStr(), imported_key.exportPrivateHEX());
            Napi::Array key_array = Napi::Array::New(env, 2);
            int index(0); 
            key_array[index++] = Napi::String::New(env,keyPairHEX.first); 
            key_array[index] = Napi::String::New(env, keyPairHEX.second); 
            return key_array;
        }

        Napi::Array sign (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env();
            if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
                Napi::TypeError::New(env, "2 Parameters of type string are expected").ThrowAsJavaScriptException();
            }
            Napi::String cMsg = info[0].As<Napi::String>(); 
            Napi::String cPrivKey = info[1].As<Napi::String>();
            AsymKey key;
            key.importPrivatePEM(cPrivKey);
            const std::pair<std::string, std::string> sign = key.sign(cMsg);
            Napi::Array key_array = Napi::Array::New(env, 2);
            int index(0); 
            key_array[index++] = Napi::String::New(env,sign.first); 
            key_array[index] = Napi::String::New(env, sign.second); 
            return key_array;                
        }
        
        Napi::Boolean verifySig(const Napi::CallbackInfo& info){
            Napi::Env env = info.Env(); 
            if(info.Length() != 4 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsString()){
                Napi::TypeError::New(env, "4 parameters of type string expected").ThrowAsJavaScriptException();
            }
            Napi::String cMsg = info[0].As<Napi::String>();
            Napi::String cPubKey = info[1].As<Napi::String>();
            Napi::String cSigR = info[2].As<Napi::String>();
            Napi::String cSigS = info[3].As<Napi::String>();
            
            const bool verifyOK = verify(cMsg, cPubKey, std::make_pair(cSigR, cSigS));
            
            return Napi::Boolean::New(env, verifyOK);
        }

        Napi::Boolean verifySigDer(const Napi::CallbackInfo& info){
            Napi::Env env = info.Env();
            if(info.Length() != 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString()){
                Napi::TypeError::New(env, "3 parameters of type string expected").ThrowAsJavaScriptException();
            }
            Napi::String cMsg = info[0].As<Napi::String>();
            Napi::String cPubKey = info[1].As<Napi::String>();
            Napi::String cSigDer = info[2].As<Napi::String>();
            size_t sigBinLen(0);
            std::unique_ptr<unsigned char[]> sigBin = HexStrToBin(cSigDer,&sigBinLen);
            const bool verifyOK = verifyDER(cMsg, cPubKey, sigBin,sigBinLen);
            return Napi::Boolean::New(env,verifyOK);
        }

        Napi::String createSharedSecret(const Napi::CallbackInfo& info){
            Napi::Env env = info.Env() ; 
            if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
                Napi::TypeError::New(env, "2 parameters of type string expected").ThrowAsJavaScriptException();
            }
            Napi::String cMyPrivateKeyPEM = info[0].As<Napi::String>();
            Napi::String cTheirPublicKeyPEM = info[1].As<Napi::String>();
            

            AsymKey my_private_key;
            my_private_key.importPrivatePEM(cMyPrivateKeyPEM);
            const std::string shared_secret = my_private_key.exportSharedSecretHex(cTheirPublicKeyPEM);

            return Napi::String::New(env, shared_secret);
        }
        
        Napi::String derPubKey (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env();
            if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
                Napi::TypeError::New(env, "2 parameters expected of type string").ThrowAsJavaScriptException();
            }
            
            Napi::String cPublicKey = info[0].As<Napi::String>();
            Napi::String cMsg = info[0].As<Napi::String>();
            const std::string derived_pub_key = derive_pubkey(cPublicKey, cMsg);
            return Napi::String::New(env, derived_pub_key); 
        
        }
        
        Napi::String derPriKey (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env(); 
            if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
                Napi::TypeError::New(env, "2 parameters expected of type string").ThrowAsJavaScriptException();
            }
            
            Napi::String cPrivateKey = info[0].As<Napi::String>();
            Napi::String cMsg = info[1].As<Napi::String>();
            
            AsymKey private_key;
            private_key.importPrivatePEM(cPrivateKey);

            const AsymKey derived_key = private_key.derive(cMsg);
            const std::string derived_key_PEM = derived_key.exportPrivatePEM();
            return Napi::String::New(env, derived_key_PEM);
        }
        
        Napi::Array splitPriKey (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env(); 
            if(info.Length() != 3 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()){
                Napi::TypeError::New(env, "3 parameters of type string, integer, integer expected").ThrowAsJavaScriptException();
            }
            
            Napi::String cPrivateKey = info[0].As<Napi::String>(); 
            Napi::Number ThresholdNumber = info[1].As<Napi::Number>();
            Napi::Number TotalNumberOfShares = info[2].As<Napi::Number>();
            


            AsymKey private_key;
            private_key.importPrivatePEM(cPrivateKey);

            std::vector<KeyShare> shares = private_key.split(ThresholdNumber.Int32Value(), TotalNumberOfShares.Int32Value());
            Napi::Array shares_array = Napi::Array::New(env, shares.size());
            int index(0);
            for(std::vector<KeyShare>::const_iterator iter = shares.begin(); iter != shares.end(); ++iter){
                shares_array[index++] = Napi::String::New(env, keyshare_to_json(*iter));
            }
            return shares_array; 
        }
        
        Napi::Array recoverPriKey (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env(); 
            if(info.Length() != 1 || !info[0].IsArray()){
                Napi::TypeError::New(env, "1 parameter of type array of shares expected").ThrowAsJavaScriptException(); 
            }
            std::vector<KeyShare> shares;
            Napi::Array sharesArray = info[0].As<Napi::Array>(); 
            for(unsigned int i =0; i<sharesArray.Length(); ++i){
                Napi::Value v = sharesArray[i];
                if(v.IsString()){
                    shares.push_back(keyshare_from_json(v.As<Napi::String>()));
                }
            }
            AsymKey private_key;
            try{
                private_key.recover(shares);
            }
            catch(std::exception& err){
                throw Napi::Error::New(env, err.what());
            }
            Napi::Array key_array = Napi::Array::New(env, 2);
            int index(0); 
            key_array[index++] = Napi::String::New(env,private_key.exportPublicPEM()); 
            key_array[index] = Napi::String::New(env,private_key.exportPrivatePEM()); 
            return key_array;                
        }
        
        
        Napi::Array ImpFromPem (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env() ; 
            if(info.Length() != 1 || !info[0].IsString() ){
                Napi::TypeError::New(env, "1 parameter of type string expected").ThrowAsJavaScriptException();
            }
            Napi::String cPrivateKey = info[0].As<Napi::String>(); 
            AsymKey private_key;
            private_key.importPrivatePEM(cPrivateKey);
            if (private_key.is_valid()){
                Napi::Array key_array = Napi::Array::New(env, 2);
                int index(0); 
                key_array[index++] = Napi::String::New(env,private_key.exportPublicPEM()); 
                key_array[index] = Napi::String::New(env,private_key.exportPrivatePEM()); 
                return key_array;
            }else{
                throw Napi::Error::New(env,"Invalid Pem format for key");
            }
        }
            
        Napi::Array ImpFromEncPem (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env(); 
            if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
                Napi::TypeError::New(env, "2 parameters of type string expected").ThrowAsJavaScriptException();
            }
            Napi::String cPrivateKey = info[0].As<Napi::String>(); 
            Napi::String cPassPhrase = info[1].As<Napi::String>(); 
            
            AsymKey private_key;
            private_key.importPrivatePEMEncrypted(cPrivateKey, cPassPhrase);
             if (private_key.is_valid()){
                Napi::Array key_array = Napi::Array::New(env, 2);
                int index(0); 
                key_array[index++] = Napi::String::New(env,private_key.exportPublicPEM()); 
                key_array[index] = Napi::String::New(env,private_key.exportPrivatePEM()); 
                return key_array;
             }else{
                throw Napi::Error::New(env, "Unable to import encrypted PEM key"); 
             }
        }
        
        Napi::String ExportFromEncPem(const Napi::CallbackInfo& info){
            Napi::Env env = info.Env(); 
            if(info.Length() != 1 || !info[0].IsString()){
                Napi::TypeError::New(env, "1 parameter of type string expected").ThrowAsJavaScriptException(); 
            }
            Napi::String cPassPhrase = info[0].As<Napi::String>();
            AsymKey private_key;
            std::string privkey_str( private_key.exportPrivatePEMEncrypted(cPassPhrase));
            if(private_key.is_valid()){
                return Napi::String::New(env, privkey_str); 
            }else{
                throw Napi::Error::New(env,"Unable to export Key in encrypted PEM format"); 
            }
        }
        
        Napi::String DerSig(const Napi::CallbackInfo& info){
            Napi::Env env = info.Env(); 
            if(info.Length() != 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsBoolean()){
                Napi::TypeError::New(env, "3 parameters of type string string and boolean expected").ThrowAsJavaScriptException(); 
            }
            
            Napi::String cSigR = info[0].As<Napi::String>(); 
            Napi::String cSigS = info[1].As<Napi::String>();
            Napi::Boolean isdecimal = info[2].As<Napi::Boolean>(); 

            BigNumber rValue, sValue;
            if(isdecimal){
                rValue.FromDec(cSigR);
                sValue.FromDec(cSigS);
            }else{
                rValue.FromHex(cSigR);
                sValue.FromHex(cSigS);
            }
             
            size_t len(-1); 
            std::unique_ptr<unsigned char[]>  sigDERTest = DEREncodedSignature(rValue,sValue,len);
            std::string sigAsHex = binTohexStr(sigDERTest,len);
            
            if(isdecimal){
                BigNumber decimalRet;
                decimalRet.FromHex(sigAsHex);
                return Napi::String::New(env, decimalRet.ToDec());
            }else{
                return Napi::String::New(env,sigAsHex);
            }
            
        }
        Napi::String pubKeyPEMtoHEXPoint (const Napi::CallbackInfo& info){
            
            Napi::Env env = info.Env();
            if(info.Length() != 2 || !info[0].IsString() || !info[1].IsBoolean()){
                Napi::TypeError::New(env,"1 parameter expected of type string").ThrowAsJavaScriptException();
            }
            Napi::String pubkeypem = info[0].As<Napi::String>();
            Napi::Boolean compressed = info[1].As<Napi::Boolean>();
            
            std::string keyPt = pubkey_pem2Hex_point(pubkeypem, compressed);
            return Napi::String::New(env, keyPt);
        
        }

        Napi::String pubKeyHextoPEM (const Napi::CallbackInfo& info){
            Napi::Env env = info.Env();
            if(info.Length() != 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsNumber()){
                Napi::TypeError::New(env,"2 parameters expected of type string").ThrowAsJavaScriptException();
            }
            Napi::String xpt = info[0].As<Napi::String>();
            Napi::String ypt = info[1].As<Napi::String>();
            Napi::Number nid = info[2].As<Napi::Number>();
    
            std::string pemPubKey = pubkey_coordinates2pem(xpt,ypt, nid.Int32Value());
            return Napi::String::New(env,pemPubKey);
        }
}
