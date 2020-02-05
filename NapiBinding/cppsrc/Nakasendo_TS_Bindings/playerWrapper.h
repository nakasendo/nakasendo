#ifndef __PLAYER_WRAPPER_H__
#define __PLAYER_WRAPPER_H__

#include <napi.h>
#include <memory>
#include <player.h>

class playerWrap : public Napi::ObjectWrap<playerWrap>{
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        playerWrap(const Napi::CallbackInfo&);
        const std::unique_ptr<player>& getInternalInstance() { return playerPtr;}
    private:
        static Napi::FunctionReference constructor;
        Napi::Value printPlayerInfo(const Napi::CallbackInfo&);
        Napi::Value GetUserID(const Napi::CallbackInfo&);
        Napi::Value GetURI(const Napi::CallbackInfo&);
        Napi::Value GetAddr(const Napi::CallbackInfo&);
        Napi::Value GetPort(const Napi::CallbackInfo&);
        std::unique_ptr<player> playerPtr;
};

class SinglePlayerWrap : public Napi::ObjectWrap<SinglePlayerWrap>{
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        SinglePlayerWrap(const Napi::CallbackInfo&);
        const std::unique_ptr<SinglePlayer>& getInternalInstance() { return spPtr;}
    private:
        static Napi::FunctionReference constructor;
        Napi::Value addGroup(const Napi::CallbackInfo&);
        Napi::Value GroupIDS (const Napi::CallbackInfo&);
        std::unique_ptr<SinglePlayer> spPtr;
};

//std::vector<std::string> GroupIDS () const; 

class jvrssWrap : public Napi::ObjectWrap<jvrssWrap> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        jvrssWrap(const Napi::CallbackInfo& info); //Constructor to initialise
        const std::unique_ptr<jvrss>& getInternalInstance() const { return jvrssPtr;}
        std::unique_ptr<jvrss>& getInternalInstance() { return jvrssPtr;}
    private:
        static Napi::FunctionReference constructor; //reference to store the class definition that needs to be exported to JS
        Napi::Value reset (const Napi::CallbackInfo& info);
        Napi::Value verifyCorrectness (const Napi::CallbackInfo& info); 
        Napi::Value verifyHonesty(const Napi::CallbackInfo& info); 
        Napi::Value printJVRSS(const Napi::CallbackInfo& info);

        Napi::Value setFX(const Napi::CallbackInfo&);
        Napi::Value GetEvalForPlayerOrdinal(const Napi::CallbackInfo&);
        std::unique_ptr<jvrss> jvrssPtr; //internal instance of jvrss used to perform actual operations.
};

class playerGroupMetaDataWrap : public Napi::ObjectWrap<playerGroupMetaDataWrap>{
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        playerGroupMetaDataWrap(const Napi::CallbackInfo& info);
        const std::unique_ptr<playerGroupMetaData>& getInternalInstance() { return playerGrpMetaDataPtr;}
    private:
        static Napi::FunctionReference constructor; 
        Napi::Value printPlayerGrp(const Napi::CallbackInfo& info);
        Napi::Value SetGrpID(const Napi::CallbackInfo& info);
        Napi::Value GetGrpID(const Napi::CallbackInfo& info);
        Napi::Value SetOrdinal(const Napi::CallbackInfo& info);
        Napi::Value GetOrdinal(const Napi::CallbackInfo& info);
        Napi::Value GetPolyDegree(const Napi::CallbackInfo& info);
        Napi::Value GetPrivateKeyShare(const Napi::CallbackInfo& info);
        Napi::Value GetGroupPublicKey(const Napi::CallbackInfo& info);
        Napi::Value SetPrivateKeyShare(const Napi::CallbackInfo&);
        Napi::Value GetEmpheralKeyList(const Napi::CallbackInfo& info);
        Napi::Value GetKValue (const Napi::CallbackInfo& info);
        Napi::Value SetKValue (const Napi::CallbackInfo&);
        Napi::Value GetAlpha (const Napi::CallbackInfo& info);
        Napi::Value SetAlpha (const Napi::CallbackInfo&);
        Napi::Value PrivateKeyShares(const Napi::CallbackInfo& info);
        Napi::Value GetModulo(const Napi::CallbackInfo& info);
        Napi::Value GetPlayerAndOrdinalList(const Napi::CallbackInfo& info);
        Napi::Value AddOrdinalAndPlayerToGrp(const Napi::CallbackInfo&);

        Napi::Value createPolynomial(const Napi::CallbackInfo&);
        Napi::Value PolynomialPreCalculation(const Napi::CallbackInfo&);
        Napi::Value SecretPreCalcWithKeySharePolynomial(const Napi::CallbackInfo&);
        Napi::Value SecretPreCalc(const Napi::CallbackInfo&);

        Napi::Value addPublicEvalsToJVRSS(const Napi::CallbackInfo&);
        Napi::Value addHiddenEvalsToJVRSS(const Napi::CallbackInfo&);
        Napi::Value addHiddenPolynomialToJVRSS(const Napi::CallbackInfo&) ; 
        Napi::Value addPreSignDataToJVRSS(const Napi::CallbackInfo&);

        Napi::Value verifyCorrectness(const Napi::CallbackInfo&);
        Napi::Value verifyHonesty(const Napi::CallbackInfo&);
        Napi::Value createSecret(const Napi::CallbackInfo&);
        Napi::Value createGroupPublicKey(const Napi::CallbackInfo&);
        Napi::Value CalculateEphemeralKey(const Napi::CallbackInfo&);
        Napi::Value Signature(const Napi::CallbackInfo&);
        Napi::Value CalcPartialSignature(const Napi::CallbackInfo&);
        Napi::Value CalculateGroupSignature(const Napi::CallbackInfo&);
        Napi::Value addPrivateKeyInfo(const Napi::CallbackInfo&);
        Napi::Value CalculateGroupPrivateKey(const Napi::CallbackInfo&);
        Napi::Value ValidateGroupPrivateKey(const Napi::CallbackInfo&);
        
        Napi::Value GetJVRSS(const Napi::CallbackInfo&);
        Napi::Value GetHiddenPolynomial(const Napi::CallbackInfo&);
        Napi::Value GetHiddenEvals(const Napi::CallbackInfo&);
        Napi::Value EphemeralKeyPlayerData(const Napi::CallbackInfo&);
        std::unique_ptr<playerGroupMetaData> playerGrpMetaDataPtr; 
};
#endif //ifndef __PLAYER_WRAPPER_H__
