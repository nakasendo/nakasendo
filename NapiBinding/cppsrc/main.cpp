#include <napi.h>

#include <playerWrapper.h>
#include <GroupMetaDataWrapper.h>
#include <OrchestratorWrapper.h>

namespace bindings_BigNumber{
    Napi::String generate_random_wrapped(const Napi::CallbackInfo& info);
    Napi::String bn_add(const Napi::CallbackInfo& info);
    Napi::String bn_sub(const Napi::CallbackInfo& info);
    Napi::String bn_mul(const Napi::CallbackInfo& info);
    Napi::String bn_div(const Napi::CallbackInfo& info);
    Napi::String bn_mod (const Napi::CallbackInfo& info);
    Napi::String bn_inv_mod(const Napi::CallbackInfo& info);
    Napi::String bn_add_mod(const Napi::CallbackInfo& info);
    Napi::String bn_sub_mod(const Napi::CallbackInfo& info);
    Napi::String bn_mul_mod(const Napi::CallbackInfo& info);
    Napi::String bn_div_mod(const Napi::CallbackInfo& info);
    Napi::Boolean bn_greater(const Napi::CallbackInfo& info);
    Napi::Boolean bn_equal(const Napi::CallbackInfo& info);
}

namespace bindings_Polynomial{
    Napi::Array randomPolynomial(const Napi::CallbackInfo& info);
    Napi::String evalulatePolynomial(const Napi::CallbackInfo& info);
    Napi::String LGInterpolatorFullWrapper(const Napi::CallbackInfo& info); 
    Napi::String LGInterpolateSingleWrapper(const Napi::CallbackInfo& info);
    
    Napi::String ECLGInterpolater(const Napi::CallbackInfo& info);
}

namespace bindings_ECPoint{
    Napi::String generateRandomECWrapper(const Napi::CallbackInfo& info);
    Napi::String GetGroupOrder(const Napi::CallbackInfo& info);
    Napi::Number GetGroupDegree(const Napi::CallbackInfo& info);
    Napi::String MultiplyByGenerator(const Napi::CallbackInfo& info);
    Napi::Boolean CheckonCurve(const Napi::CallbackInfo& info);
    Napi::String multiplyScalar(const Napi::CallbackInfo& info);
    Napi::Value GetAffineCoOrdinates(const Napi::CallbackInfo& info);
    Napi::Value Add(const Napi::CallbackInfo& info);
    Napi::Boolean Compare(const Napi::CallbackInfo& info);
    Napi::String Double(const Napi::CallbackInfo& info);
    Napi::String Invert(const Napi::CallbackInfo& info);
    
}

namespace bindings_MessageHash{
    // have to be chainable
    Napi::String SHA256 (const Napi::CallbackInfo& info);
    Napi::String RIPEMD160 (const Napi::CallbackInfo& info);
    Napi::String Hash (const Napi::CallbackInfo& info);
    Napi::String EncB64(const Napi::CallbackInfo& info);
    Napi::String decB64(const Napi::CallbackInfo& info);
    
    Napi::String EncB58(const Napi::CallbackInfo& info);
    Napi::String DecB58(const Napi::CallbackInfo& info);
    
    Napi::String EncB58Checked(const Napi::CallbackInfo& info);
    Napi::String DecB58Checked(const Napi::CallbackInfo& info);
    
    
}

namespace bindings_AsymKey{
    Napi::Array GenKeyPairPem (const Napi::CallbackInfo& info);
    Napi::Array GenKeyPairHex (const Napi::CallbackInfo& info);
    Napi::Array ExportKeysHex (const Napi::CallbackInfo& info);
    Napi::Array sign (const Napi::CallbackInfo& info);
    Napi::Boolean verifySig(const Napi::CallbackInfo& info);
    Napi::Boolean verifySigDer(const Napi::CallbackInfo& info);
    Napi::String createSharedSecret(const Napi::CallbackInfo& info);
    Napi::String derPubKey (const Napi::CallbackInfo& info);
    Napi::String derPriKey (const Napi::CallbackInfo& info);
    Napi::Array splitPriKey (const Napi::CallbackInfo& info);
    Napi::Array recoverPriKey (const Napi::CallbackInfo& info);
    Napi::Array ImpFromPem (const Napi::CallbackInfo& info);
    Napi::Array ImpFromEncPem (const Napi::CallbackInfo& info);
    Napi::String ExportFromEncPem(const Napi::CallbackInfo& info);
    Napi::String DerSig(const Napi::CallbackInfo& info);
    Napi::String pubKeyPEMtoHEXPoint (const Napi::CallbackInfo& info);
    Napi::String pubKeyHextoPEM (const Napi::CallbackInfo& info);
}

namespace bindings_SymEnc{
    Napi::String encAES (const Napi::CallbackInfo& info);
    Napi::String decAES (const Napi::CallbackInfo& info);
    Napi::String GenNounce ( const Napi::CallbackInfo& info);
    Napi::String GenKey (const Napi::CallbackInfo& info);
}

namespace bindings_BSVAddress{
    Napi::Array createBSVAddress(const Napi::CallbackInfo& info);
    Napi::Array importBSVAddress(const Napi::CallbackInfo& info);
    Napi::String printBSVAddress(const Napi::CallbackInfo& info);
}


Napi::Object init(Napi::Env env, Napi::Object exports){

    exports.Set("generate_random", Napi::Function::New(env,bindings_BigNumber::generate_random_wrapped));
    exports.Set("bn_add",Napi::Function::New(env, bindings_BigNumber::bn_add));
    exports.Set("bn_sub",Napi::Function::New(env,bindings_BigNumber::bn_sub));
    exports.Set("bn_mul",Napi::Function::New(env,bindings_BigNumber::bn_mul));
    exports.Set("bn_div",Napi::Function::New(env,bindings_BigNumber::bn_div));
    exports.Set("bn_mod",Napi::Function::New(env,bindings_BigNumber::bn_mod));
    exports.Set("bn_inv_mod",Napi::Function::New(env,bindings_BigNumber::bn_inv_mod));
    exports.Set("bn_add_mod",Napi::Function::New(env,bindings_BigNumber::bn_add_mod));
    exports.Set("bn_sub_mod",Napi::Function::New(env,bindings_BigNumber::bn_sub_mod));
    exports.Set("bn_mul_mod",Napi::Function::New(env,bindings_BigNumber::bn_mul_mod));
    exports.Set("bn_div_mod",Napi::Function::New(env,bindings_BigNumber::bn_div_mod));
    exports.Set("bn_is_greater",Napi::Function::New(env,bindings_BigNumber::bn_greater));
    exports.Set("bn_is_equal",Napi::Function::New(env,bindings_BigNumber::bn_equal));
    
    
    exports.Set("randomPolynomial",Napi::Function::New(env,bindings_Polynomial::randomPolynomial));
    exports.Set("evalulatePolynomial", Napi::Function::New(env, bindings_Polynomial::evalulatePolynomial));
    exports.Set("LGInterpolatorFull", Napi::Function::New(env,bindings_Polynomial::LGInterpolatorFullWrapper));
    exports.Set("LGInterpolatorSingle",Napi::Function::New(env,bindings_Polynomial::LGInterpolateSingleWrapper));
    exports.Set("ECLGInterpolatorFull", Napi::Function::New(env,bindings_Polynomial::ECLGInterpolater));
    
    
    exports.Set("ec_pt_generate_random", Napi::Function::New(env,bindings_ECPoint::generateRandomECWrapper));
    exports.Set("ec_pt_GetGroupOrder",Napi::Function::New(env,bindings_ECPoint::GetGroupOrder));
    exports.Set("ec_pt_GetGroupDegree",Napi::Function::New(env,bindings_ECPoint::GetGroupDegree));
    exports.Set("ec_pt_MultiplyByGenerator",Napi::Function::New(env,bindings_ECPoint::MultiplyByGenerator));
    exports.Set("ec_pt_CheckonCurve",Napi::Function::New(env,bindings_ECPoint::CheckonCurve));
    exports.Set("ec_pt_multiplyScalar",Napi::Function::New(env,bindings_ECPoint::multiplyScalar));
    exports.Set("ec_pt_GetAffineCoordinates",Napi::Function::New(env,bindings_ECPoint::GetAffineCoOrdinates));
    exports.Set("ec_pt_Add",Napi::Function::New(env,bindings_ECPoint::Add));
    exports.Set("ec_pt_compare", Napi::Function::New(env,bindings_ECPoint::Compare));
    exports.Set("ec_pt_double",Napi::Function::New(env,bindings_ECPoint::Double));
    exports.Set("ec_pt_invert",Napi::Function::New(env,bindings_ECPoint::Invert));
    
    exports.Set("SHA256", Napi::Function::New(env,bindings_MessageHash::SHA256));
    exports.Set("RIPEMD160", Napi::Function::New(env,bindings_MessageHash::RIPEMD160));
    exports.Set("HASH", Napi::Function::New(env,bindings_MessageHash::Hash));
    exports.Set("EncodeBase64", Napi::Function::New(env,bindings_MessageHash::EncB64));
    exports.Set("DecodeBase64", Napi::Function::New(env,bindings_MessageHash::decB64));
    exports.Set("EncodeBase58", Napi::Function::New(env,bindings_MessageHash::EncB58));
    exports.Set("DecodeBase58", Napi::Function::New(env,bindings_MessageHash::DecB58));
    exports.Set("EncodeBase58Checked", Napi::Function::New(env,bindings_MessageHash::EncB58Checked));
    exports.Set("DecodeBase58Checked", Napi::Function::New(env,bindings_MessageHash::DecB58Checked));
    
    exports.Set("GenerateKeyPairPEM",Napi::Function::New(env, bindings_AsymKey::GenKeyPairPem));
    exports.Set("GenerateKeyPairHEX",Napi::Function::New(env, bindings_AsymKey::GenKeyPairHex));
    exports.Set("ExportKeyPairHEX",Napi::Function::New(env, bindings_AsymKey::ExportKeysHex));
    exports.Set("PubKeyPEMToHex", Napi::Function::New(env, bindings_AsymKey::pubKeyPEMtoHEXPoint));
    exports.Set("Sign",Napi::Function::New(env, bindings_AsymKey::sign));
    exports.Set("Verify",Napi::Function::New(env, bindings_AsymKey::verifySig));
    exports.Set("VerifyDER",Napi::Function::New(env, bindings_AsymKey::verifySigDer));
    exports.Set("ShareSecret",Napi::Function::New(env, bindings_AsymKey::createSharedSecret));
    exports.Set("DerivePublic",Napi::Function::New(env, bindings_AsymKey::derPubKey));
    exports.Set("DerivePrivate",Napi::Function::New(env, bindings_AsymKey::derPriKey));
    exports.Set("SplitKey",Napi::Function::New(env, bindings_AsymKey::splitPriKey));
    exports.Set("RestoreKey",Napi::Function::New(env, bindings_AsymKey::recoverPriKey));
    exports.Set("ImportFromPem",Napi::Function::New(env, bindings_AsymKey::ImpFromPem));
    exports.Set("ImportFromEncryptedPEM",Napi::Function::New(env, bindings_AsymKey::ImpFromEncPem));
    exports.Set("ExportFromEncryptedPEM",Napi::Function::New(env, bindings_AsymKey::ExportFromEncPem));
    exports.Set("DERSignature",Napi::Function::New(env, bindings_AsymKey::DerSig));
    exports.Set("pubKeyHextoPEM",Napi::Function::New(env,bindings_AsymKey::pubKeyHextoPEM));
    
    
    
    exports.Set("encodeAES",Napi::Function::New(env,bindings_SymEnc::encAES));
    exports.Set("decodeAES",Napi::Function::New(env,bindings_SymEnc::decAES));
    exports.Set("GenerateNounce",Napi::Function::New(env,bindings_SymEnc::GenNounce));
    exports.Set("GenerateKey",Napi::Function::New(env,bindings_SymEnc::GenKey));
    
    exports.Set("createBSVAddress",Napi::Function::New(env,bindings_BSVAddress::createBSVAddress));
    exports.Set("importBSVAddress",Napi::Function::New(env,bindings_BSVAddress::importBSVAddress));
    exports.Set("printBSVAddress", Napi::Function::New(env, bindings_BSVAddress::printBSVAddress));

    exports.Set("AddPlayerToGlobalList",Napi::Function::New(env,bindings_TS_Orchestrator::AddPlayerToGlobalList));
    exports.Set("GetGlobalPlayerList",Napi::Function::New(env,bindings_TS_Orchestrator::GetGlobalPlayerList));
    exports.Set("PrintGlobalPlayerList",Napi::Function::New(env,bindings_TS_Orchestrator::PrintGlobalPlayerList));
    exports.Set("CreateTSGroup", Napi::Function::New(env,bindings_TS_Orchestrator::CreateTSGroup));
    exports.Set("AddUserToTSGroup", Napi::Function::New(env,bindings_TS_Orchestrator::AddUserToTSGroup));
    exports.Set("RemoveUserFromTSGroup", Napi::Function::New(env,bindings_TS_Orchestrator::RemoveUserFromTSGroup));
    exports.Set("DeleteGroup", Napi::Function::New(env,bindings_TS_Orchestrator::DeleteGroup));
    exports.Set("DeleteGrpSecretSharingContainers", Napi::Function::New(env,bindings_TS_Orchestrator::DeleteGrpSecretSharingContainers));
    exports.Set("DeleteGrpSignatureContainers", Napi::Function::New(env,bindings_TS_Orchestrator::DeleteGrpSignatureContainers));
    exports.Set("DeleteGrpPreSignatureContainers", Napi::Function::New(env,bindings_TS_Orchestrator::DeleteGrpPreSignatureContainers));
    exports.Set("lockGroup", Napi::Function::New(env,bindings_TS_Orchestrator::lockGroup));
    exports.Set("AddUserToTSGroup", Napi::Function::New(env,bindings_TS_Orchestrator::AddUserToTSGroup));
    exports.Set("unLockGroup", Napi::Function::New(env,bindings_TS_Orchestrator::unLockGroup));

    exports.Set("GenerateUUID", Napi::Function::New(env,bindings_TS_Orchestrator::GenerateUUID));
    jvrssWrap::Init(env, exports);
    playerGroupMetaDataWrap::Init(env,exports);
    GroupMetaDataWrap::Init(env,exports);
    playerWrap::Init(env,exports);
    SinglePlayerWrap::Init(env,exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, init)
