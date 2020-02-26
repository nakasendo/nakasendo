#include<sstream>
#include "playerWrapper.h"

Napi::FunctionReference playerWrap::constructor;

Napi::Object playerWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "playerWrap", {
    InstanceMethod("printPlayerInfo", &playerWrap::printPlayerInfo),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("playerWrap", func);
  return exports;
}

playerWrap::playerWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<playerWrap>(info){
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if(info.Length() == 0){
    playerPtr.reset(new player);
    return;
  }else if(info.Length() == 1){
    //copy constructor
     Napi::Object object_parent = info[0].As<Napi::Object>();
     playerWrap* parent = Napi::ObjectWrap<playerWrap>::Unwrap(object_parent);
     playerPtr.reset(parent->getInternalInstance().get());
     return;
  }else if(info.Length() == 4 || info[0].IsString() || info[1].IsString() || info[2].IsString() || info[3].IsString()){
    playerPtr.reset(new player(info[0].As<Napi::String>(),info[1].As<Napi::String>(), info[2].As<Napi::String>(), info[3].As<Napi::String>()));
  }
}

Napi::Value playerWrap::printPlayerInfo(const Napi::CallbackInfo& info){
  std::stringstream output;
  output << *(playerPtr.get()) << std::endl;
  return Napi::String::New(info.Env(),output.str());
}

Napi::Value playerWrap::GetUserID(const Napi::CallbackInfo& info){
  return Napi::String::New(info.Env(),playerPtr->userID());
}
Napi::Value playerWrap::GetURI(const Napi::CallbackInfo& info){
  return Napi::String::New(info.Env(), playerPtr->uri());
}
Napi::Value playerWrap::GetAddr(const Napi::CallbackInfo& info){
  return Napi::String::New(info.Env(),playerPtr->addr());
}
Napi::Value playerWrap::GetPort(const Napi::CallbackInfo& info){
  return Napi::String::New(info.Env(),playerPtr->port());
}


Napi::FunctionReference SinglePlayerWrap::constructor;
Napi::Object SinglePlayerWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
  Napi::Function func = DefineClass(env, "playerWrap", {
    InstanceMethod("AddGroup",&SinglePlayerWrap::addGroup),
    InstanceMethod("GroupIDS",&SinglePlayerWrap::GroupIDS),
  });
  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("SinglePlayerWrap", func);
  return exports;
}

SinglePlayerWrap::SinglePlayerWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SinglePlayerWrap>(info){
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if(info.Length() == 1){
    //copy constructor
    Napi::TypeError::New(env, "Not allowed copy a Singleton Object").ThrowAsJavaScriptException();
  }
  spPtr.reset(SinglePlayer::Instance().get());
  return;
}

Napi::Value  SinglePlayerWrap::addGroup(const Napi::CallbackInfo& info){
  Napi::Object object_parent = info[0].As<Napi::Object>();
  playerGroupMetaDataWrap* parent = Napi::ObjectWrap<playerGroupMetaDataWrap>::Unwrap(object_parent);
  spPtr->addPlayerGroup(*(parent->getInternalInstance().get()));
  return Napi::Boolean::New(info.Env(), true);
}

Napi::Value SinglePlayerWrap::GroupIDS (const Napi::CallbackInfo& info){
  std::vector<std::string> ids = spPtr->GroupIDS ();
  Napi::Array idArray = Napi::Array::New(info.Env(), ids.size());
  int index(0);
  for(std::vector<std::string>::const_iterator iter = ids.begin(); iter != ids.end(); ++iter){
    idArray[index++] = Napi::String::New(info.Env(),*iter);
  }
  return idArray;
}

Napi::FunctionReference jvrssWrap::constructor;

Napi::Object jvrssWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "jvrssWrap", {
    InstanceMethod("reset", &jvrssWrap::reset),
    InstanceMethod("verifyCorrectness", &jvrssWrap::verifyCorrectness),
    InstanceMethod("verifyHonesty", &jvrssWrap::verifyHonesty),
    InstanceMethod("printJVRSS", &jvrssWrap::printJVRSS),
    InstanceMethod("setFX", &jvrssWrap::setFX),
    InstanceMethod("GetEvalForPlayerOrdinal", &jvrssWrap::GetEvalForPlayerOrdinal),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("jvrssWrap", func);
  return exports;
}

jvrssWrap::jvrssWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<jvrssWrap>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if(info.Length() == 0){
    jvrssPtr.reset (new jvrss);
    return ;
  }else if(info.Length() == 1){
    // copy constructor
     Napi::Object object_parent = info[0].As<Napi::Object>();
     jvrssWrap* parent = Napi::ObjectWrap<jvrssWrap>::Unwrap(object_parent);
     jvrssPtr.reset(parent->getInternalInstance().get());
     return;
  }
}

Napi::Value jvrssWrap::reset(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
        Napi::TypeError::New(env, "No parameters expected for a JVRSS reset call").ThrowAsJavaScriptException();
    }
    jvrssPtr->reset();
    return Napi::Boolean::New(env, true);
}


Napi::Value jvrssWrap::verifyCorrectness (const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 2){ // add a check for the types (vector<int> & string(bigNumbner))
        Napi::TypeError::New(env, "Two parameters expected, array of type integer & a string").ThrowAsJavaScriptException();
    }

    std::vector<int> vals;
    BigNumber bn = GenerateRand(512);
    bool success = jvrssPtr->verifyCorrectness(vals, bn);
    return Napi::Boolean::New(env, success);
}

Napi::Value jvrssWrap::verifyHonesty(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 2){ // add a check for the types (vector<int> & string(bigNumbner))
        Napi::TypeError::New(env, "Two parameters expected, array of type integer & a string").ThrowAsJavaScriptException();
    }

    std::vector<int> vals;
    BigNumber bn = GenerateRand(512);
    bool success = jvrssPtr->verifyHonesty(vals, bn);
    return Napi::Boolean::New(env, success);
}

Napi::Value jvrssWrap::printJVRSS(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length () != 0){
        Napi::TypeError::New(env, "No parameters expected for printing").ThrowAsJavaScriptException();
    }
    std::stringstream output;
    output << *(jvrssPtr.get());
    return Napi::String::New(env, output.str());
}

Napi::Value jvrssWrap::setFX(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length () != 1 || !info[0].IsString()){
    Napi::TypeError::New(env,"One parameter expected of type BigNumber").ThrowAsJavaScriptException();
  }
  BigNumber eval;
  eval.FromHex(info[0].As<Napi::String>());
  jvrssPtr->m_fx = eval; 
  return Napi::Boolean::New(env,true);
}

Napi::Value jvrssWrap::GetEvalForPlayerOrdinal(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 1 || !info[0].IsString())
    Napi::TypeError::New(env,"One parameter of type number expected").ThrowAsJavaScriptException();
  std::map<std::string, BigNumber>::const_iterator iter = jvrssPtr->m_evals.find(info[0].As<Napi::String>());
  if(iter == jvrssPtr->m_evals.end())
    Napi::TypeError::New(env,"No evalutaion for player label").ThrowAsJavaScriptException();
  return Napi::String::New(env,iter->second.ToHex());
}

Napi::FunctionReference playerGroupMetaDataWrap::constructor;

Napi::Object playerGroupMetaDataWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "playerGroupMetaDataWrap", {
    InstanceMethod("printPlayerGrp", &playerGroupMetaDataWrap::printPlayerGrp),
    InstanceMethod("SetGrpID", &playerGroupMetaDataWrap::SetGrpID),
    InstanceMethod("GetGrpID",&playerGroupMetaDataWrap::GetGrpID),
    InstanceMethod("SetOrdinal",&playerGroupMetaDataWrap::SetOrdinal),
    InstanceMethod("GetOrdinal",&playerGroupMetaDataWrap::GetOrdinal),
    InstanceMethod("GetPolyDegree",&playerGroupMetaDataWrap::GetPolyDegree),
    InstanceMethod("GetPrivateKeyShare",&playerGroupMetaDataWrap::GetPrivateKeyShare),
    InstanceMethod("SetPrivateKeyShare",&playerGroupMetaDataWrap::SetPrivateKeyShare),
    InstanceMethod("GetGroupPublicKey",&playerGroupMetaDataWrap::GetGroupPublicKey),
    InstanceMethod("GetEmpheralKeyList",&playerGroupMetaDataWrap::GetEmpheralKeyList),
    InstanceMethod("GetKValue",&playerGroupMetaDataWrap::GetKValue),
    InstanceMethod("SetKValue",&playerGroupMetaDataWrap::SetKValue),
    InstanceMethod("GetAlpha",&playerGroupMetaDataWrap::GetAlpha),
    InstanceMethod("SetAlpha",&playerGroupMetaDataWrap::SetAlpha),
    InstanceMethod("PrivateKeyShares",&playerGroupMetaDataWrap::PrivateKeyShares),
    InstanceMethod("GetModulo",&playerGroupMetaDataWrap::GetModulo),
    InstanceMethod("GetPlayerAndOrdinalList",&playerGroupMetaDataWrap::GetPlayerAndOrdinalList),
    InstanceMethod("AddOrdinalAndPlayerToGrp",&playerGroupMetaDataWrap::AddOrdinalAndPlayerToGrp),
    InstanceMethod("createPolynomial",&playerGroupMetaDataWrap::createPolynomial),
    InstanceMethod("PolynomialPreCalculation",&playerGroupMetaDataWrap::PolynomialPreCalculation),
    InstanceMethod("addPublicEvalsToJVRSS",&playerGroupMetaDataWrap::addPublicEvalsToJVRSS),
    InstanceMethod("addHiddenEvalsToJVRSS",&playerGroupMetaDataWrap::addHiddenEvalsToJVRSS),
    InstanceMethod("addHiddenPolynomialToJVRSS",&playerGroupMetaDataWrap::addHiddenPolynomialToJVRSS),
    InstanceMethod("addPreSignDataToJVRSS",&playerGroupMetaDataWrap::addPreSignDataToJVRSS),
    InstanceMethod("verifyCorrectness",&playerGroupMetaDataWrap::verifyCorrectness),
    InstanceMethod("verifyHonesty",&playerGroupMetaDataWrap::verifyHonesty),
    InstanceMethod("createSecret",&playerGroupMetaDataWrap::createSecret),
    InstanceMethod("createGroupPublicKey",&playerGroupMetaDataWrap::createGroupPublicKey),
    InstanceMethod("CalculateEphemeralKey",&playerGroupMetaDataWrap::CalculateEphemeralKey),
    InstanceMethod("Signature",&playerGroupMetaDataWrap::Signature),
    InstanceMethod("CalcPartialSignature",&playerGroupMetaDataWrap::CalcPartialSignature),
    InstanceMethod("CalculateGroupSignature",&playerGroupMetaDataWrap::CalculateGroupSignature),
    InstanceMethod("addPrivateKeyInfo",&playerGroupMetaDataWrap::addPrivateKeyInfo),
    InstanceMethod("CalculateGroupPrivateKey",&playerGroupMetaDataWrap::CalculateGroupPrivateKey),
    InstanceMethod("ValidateGroupPrivateKey",&playerGroupMetaDataWrap::ValidateGroupPrivateKey),
    InstanceMethod("GetJVRSS",&playerGroupMetaDataWrap::GetJVRSS),
    InstanceMethod("SecretPreCalcWithKeySharePolynomial",&playerGroupMetaDataWrap::SecretPreCalcWithKeySharePolynomial),
    InstanceMethod("SecretPreCalc",&playerGroupMetaDataWrap::SecretPreCalc),
    InstanceMethod("GetHiddenPolynomial",&playerGroupMetaDataWrap::GetHiddenPolynomial),
    InstanceMethod("GetHiddenEvals",&playerGroupMetaDataWrap::GetHiddenEvals),
    InstanceMethod("EphemeralKeyPlayerData",&playerGroupMetaDataWrap::EphemeralKeyPlayerData),
  });
  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("playerGroupMetaDataWrap", func);
  return exports;
}

playerGroupMetaDataWrap::playerGroupMetaDataWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<playerGroupMetaDataWrap>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if(info.Length() == 0){
    playerGrpMetaDataPtr.reset( new playerGroupMetaData);
    return ; 
  }else if (info.Length() == 1){
    // copy constructor
     Napi::Object object_parent = info[0].As<Napi::Object>();
     playerGroupMetaDataWrap* parent = Napi::ObjectWrap<playerGroupMetaDataWrap>::Unwrap(object_parent);
     playerGrpMetaDataPtr.reset(parent->getInternalInstance().get());
     return;
  }

  if(info.Length() != 2 || !info[0].IsNumber() || !info[1].IsString()){
    Napi::TypeError::New(env, "Expected two parameters of type int & BigNumber").ThrowAsJavaScriptException();
  }
  
  int degree = info[0].As<Napi::Number>();
  std::string bn = info[1].As<Napi::String> ();
  BigNumber mod; 
  mod.FromHex(bn);
  playerGrpMetaDataPtr.reset(new playerGroupMetaData(degree, mod));
  return;
}

Napi::Value playerGroupMetaDataWrap::printPlayerGrp(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length () != 0){
        Napi::TypeError::New(env, "No parameters expected for printing").ThrowAsJavaScriptException();
    }
    std::stringstream output;
    output << *(playerGrpMetaDataPtr.get());
    return Napi::String::New(env, output.str());
}

Napi::Value playerGroupMetaDataWrap::GetJVRSS(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 1 || !info[0].IsObject()){
    Napi::TypeError::New(env,"Empty object of type jvrssWrap expected").ThrowAsJavaScriptException();
  }
  Napi::Object object_parent = info[0].As<Napi::Object>();
  jvrssWrap* parent = Napi::ObjectWrap<jvrssWrap>::Unwrap(object_parent);
  parent->getInternalInstance().reset(&playerGrpMetaDataPtr->m_transientData);
  return Napi::Boolean::New(env,true); 
}

Napi::Value playerGroupMetaDataWrap::SetGrpID(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 1 || !info[0].IsString()){
      Napi::TypeError::New(env, "SetGrpID expected one parameter of type string").ThrowAsJavaScriptException();
    }
    playerGrpMetaDataPtr.get()->m_id = info[0].As<Napi::String> (); 
    return Napi::Boolean::New(env,true);

}
        
Napi::Value playerGroupMetaDataWrap::GetGrpID(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env, "No parameters expected for GrpGrpId").ThrowAsJavaScriptException();
    }
    return Napi::String::New(env,playerGrpMetaDataPtr.get()->m_id);

}

Napi::Value playerGroupMetaDataWrap::SetOrdinal(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 1 || !info[0].IsNumber()){
      Napi::TypeError::New(env, "A single parameter of type integer is expected").ThrowAsJavaScriptException();
    }
    playerGrpMetaDataPtr.get()->m_ordinal = info[0].As<Napi::Number>();
    return Napi::Boolean::New(env,true);
}

Napi::Value playerGroupMetaDataWrap::GetOrdinal(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env, "No parameters expected for GetOrdinal").ThrowAsJavaScriptException();
    }
    return Napi::Number::New(env,playerGrpMetaDataPtr.get()->m_ordinal);
}
        
Napi::Value playerGroupMetaDataWrap::GetPolyDegree(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env, "No parameters expected for GetPolyDegree").ThrowAsJavaScriptException();
    }
    return Napi::Number::New(env,playerGrpMetaDataPtr.get()->m_degree);
}

Napi::Value playerGroupMetaDataWrap::GetPrivateKeyShare(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env,"No parameters expected for GetPrivateKeyShare").ThrowAsJavaScriptException();
    }
    return Napi::String::New(env,playerGrpMetaDataPtr->m_privateKeyShare.ToHex());
}

Napi::Value playerGroupMetaDataWrap::SetPrivateKeyShare(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 1 || !info[0].IsString()){
    Napi::TypeError::New(env,"A single parameter of type string expected").ThrowAsJavaScriptException();
  }
  BigNumber secretshare;
  secretshare.FromHex(info[0].As<Napi::String>());
  playerGrpMetaDataPtr->m_privateKeyShare = secretshare;
  return Napi::Boolean::New(env,true);
}
        
Napi::Value playerGroupMetaDataWrap::GetGroupPublicKey(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env, "No parameters expected for GetGroupPublicKey").ThrowAsJavaScriptException();
    }
    return Napi::String::New(env,playerGrpMetaDataPtr.get()->m_GroupPublicKey.ToHex());
}

Napi::Value playerGroupMetaDataWrap::GetEmpheralKeyList(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env, "No parameters expected for GetEmpheralKeyList").ThrowAsJavaScriptException();
    }
    Napi::Array key_array = Napi::Array::New(env, playerGrpMetaDataPtr.get()->m_EmpheralKeyList.size()*2);
    int index(0);
    for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator iter = playerGrpMetaDataPtr.get()->m_EmpheralKeyList.begin(); 
        iter != playerGrpMetaDataPtr.get()->m_EmpheralKeyList.end();
        ++ iter)
    {
      key_array[index++] = Napi::String::New(env,iter->first.ToHex()); 
      key_array[index++] =  Napi::String::New(env,iter->second.ToHex()); 
    }
    return key_array;
}
        
Napi::Value playerGroupMetaDataWrap::GetKValue (const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env, "No parameters expected for GetKValue").ThrowAsJavaScriptException();
    }
    return Napi::String::New(env,playerGrpMetaDataPtr->m_littleK.ToHex());
}
Napi::Value playerGroupMetaDataWrap::SetKValue (const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length () != 1 || !info[0].IsString()){
    Napi::TypeError::New(env,"A single parameter of type string expected for call to SetKValue").ThrowAsJavaScriptException();
  }
  BigNumber k;
  k.FromHex(info[0].As<Napi::String>());
  playerGrpMetaDataPtr->m_littleK = k;
  return Napi::Boolean::New(env,true);
}
        
Napi::Value playerGroupMetaDataWrap::GetAlpha (const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env, "No parameters expected for GetAlpha").ThrowAsJavaScriptException();
    }
    return Napi::String::New(env,playerGrpMetaDataPtr->m_alpha.ToHex());
}

Napi::Value playerGroupMetaDataWrap::SetAlpha (const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length () != 1 || !info[0].IsString()){
    Napi::TypeError::New(env,"A single parameter of type string expected for call to SetAlpha").ThrowAsJavaScriptException();
  }
  BigNumber a;
  a.FromHex(info[0].As<Napi::String>());
  playerGrpMetaDataPtr->m_alpha = a;
  return Napi::Boolean::New(env,true);
}
      
Napi::Value playerGroupMetaDataWrap::PrivateKeyShares(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env,true);
}
        
Napi::Value playerGroupMetaDataWrap::GetModulo(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env,"No Parameters expected for GetModulo").ThrowAsJavaScriptException();
    }
    return Napi::String::New(env,playerGrpMetaDataPtr->m_modulo.ToHex());
}

Napi::Value playerGroupMetaDataWrap::GetPlayerAndOrdinalList(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 0){
      Napi::TypeError::New(env,"No parameters expectef for GetPlayerAndOrdinalList").ThrowAsJavaScriptException();
    }
    int index(0);
    Napi::Array playerArray = Napi::Array::New(env, playerGrpMetaDataPtr->m_ordinalAndPlayerList.size()*5);
    for(std::vector<std::pair<int, player> >::const_iterator iter=  playerGrpMetaDataPtr->m_ordinalAndPlayerList.begin();
        iter != playerGrpMetaDataPtr->m_ordinalAndPlayerList.end();
        ++ iter
    ){
      playerArray[index++] = Napi::Number::New(env, iter->first);
      playerArray[index++] = Napi::String::New(env,iter->second.userID());
      playerArray[index++] = Napi::String::New(env,iter->second.uri());
      playerArray[index++] = Napi::String::New(env,iter->second.addr());
      playerArray[index++] = Napi::String::New(env,iter->second.port());
    }
    return playerArray;
}

Napi::Value playerGroupMetaDataWrap::AddOrdinalAndPlayerToGrp(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 5 || !info[0].IsNumber() || !info[1].IsString() || !info[2].IsString() || !info[3].IsString() || !info[4].IsString()){
      Napi::TypeError::New(env,"1 parameter of type Integer and 4 parameters of type string expected").ThrowAsJavaScriptException();
    }
    player p(info[1].As<Napi::String>(), info[2].As<Napi::String>(),info[3].As<Napi::String>(),info[4].As<Napi::String>());
    playerGrpMetaDataPtr.get()->m_ordinalAndPlayerList.push_back(std::make_pair(info[0].As<Napi::Number>(), p));
    playerGrpMetaDataPtr.get()->m_ordinalList.push_back(info[0].As<Napi::Number>());
    return Napi::Boolean::New(env,true);
}

Napi::Value playerGroupMetaDataWrap::createPolynomial(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 2 || !info[0].IsNumber() || !info[1].IsString()){
    Napi::TypeError::New(env, "2 parameters expected of type integer and type string(BigNumber)").ThrowAsJavaScriptException();
  }
  int degree = info[0].As<Napi::Number>(); 
  std::string modStr = info[1].As<Napi::String>();
  BigNumber mod;
  mod.FromHex(modStr);
  Polynomial poly = playerGrpMetaDataPtr.get()->createPolynomial(degree,mod);
  std::vector<BigNumber> coeffs =  poly.getCoefficients();
  Napi::Array poly_array = Napi::Array::New(env, coeffs.size());
  int index(0);
  for(std::vector<BigNumber>::const_iterator iter = coeffs.begin(); iter != coeffs.end(); ++iter){
      poly_array[index++] = Napi::String::New(env, iter->ToHex());
  }
  return poly_array; 
}

Napi::Value playerGroupMetaDataWrap::PolynomialPreCalculation(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 1 || !info[0].IsArray()){
    Napi::TypeError::New(env,"1 parameter of type Array expected").ThrowAsJavaScriptException();
  }
  Napi::Array poly = info[0].As<Napi::Array>();
  std::vector<BigNumber> vecPoly; 
  for (unsigned int i=0;i<poly.Length();++i){
    Napi::Value v = poly[i];
    if(v.IsString()){
      Napi::String coeff = v.As<Napi::String>(); 
      BigNumber bn;
      bn.FromHex(coeff);
      vecPoly.push_back(bn);
    }else{
      Napi::TypeError::New(env, "Unexpected value passed as part of a polynomial in PolynomialPreCalculation").ThrowAsJavaScriptException();
    }
  }
  Polynomial p(vecPoly,playerGrpMetaDataPtr.get()->m_modulo);
  playerGrpMetaDataPtr->polynomialPreCalculation(p);
  return Napi::Boolean::New(env,true);
}

Napi::Value playerGroupMetaDataWrap::SecretPreCalcWithKeySharePolynomial(const Napi::CallbackInfo& info){
  playerGrpMetaDataPtr->polynomialPreCalculation(playerGrpMetaDataPtr->m_privateKeyPolynomial);
  return Napi::Boolean::New(info.Env(),true);
}

Napi::Value playerGroupMetaDataWrap::SecretPreCalc(const Napi::CallbackInfo& info){
  Polynomial p(playerGrpMetaDataPtr->m_degree,playerGrpMetaDataPtr->m_modulo);
  playerGrpMetaDataPtr->polynomialPreCalculation(p);
  return Napi::Boolean::New(info.Env(),true);
}

Napi::Value playerGroupMetaDataWrap::addPublicEvalsToJVRSS(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
    Napi::TypeError::New(env,"2 parameters of type string expected").ThrowAsJavaScriptException();
  }
  bool retval(true);
  try{
    playerGrpMetaDataPtr.get()->addPublicEvalsToJVRSS(info[0].As<Napi::String>(),info[1].As<Napi::String>());
  }
  catch(std::exception& e){
    std::cout << e.what() << std::endl;
    retval = !retval;
  }
  return Napi::Boolean::New(env,retval);
}

Napi::Value playerGroupMetaDataWrap::addHiddenEvalsToJVRSS(const Napi::CallbackInfo& info){
  //const std::string&, const std::vector<std::pair<std::string, std::string> >&
  Napi::Env env = info.Env();
  if(info.Length() != 2 || !info[0].IsString() || !info[1].IsArray()){
    Napi::TypeError::New(env,"2 parameters expected of type string and array for addHiddenEvalsToJVRSS").ThrowAsJavaScriptException();
  }
  std::vector<std::pair<std::string, std::string> > evalpairs; 
  Napi::Array hiddenEvalsArray = info[1].As<Napi::Array>();
  for(unsigned int i = 0; i< hiddenEvalsArray.Length(); ++i){
    Napi::Value vOrdEval = hiddenEvalsArray[i++];
    Napi::Value vEval = hiddenEvalsArray[i];
    if(!vOrdEval.IsString() || !vEval.IsString()){
      Napi::TypeError::New(env,"Unexpected parameters passed in addHiddenEvalsToJVRSS").ThrowAsJavaScriptException();
    }
    Napi::String OrdEval = vOrdEval.As<Napi::String>();
    Napi::String Eval = vEval.As<Napi::String>();
    evalpairs.push_back(std::make_pair(OrdEval,Eval));
  }
  playerGrpMetaDataPtr.get()->addHiddenEvalsToJVRSS(info[0].As<Napi::String>(), evalpairs);
  return Napi::Boolean::New(env,true);
}

Napi::Value playerGroupMetaDataWrap::addHiddenPolynomialToJVRSS(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 2 || !info[0].IsString() || !info[1].IsArray()){
    Napi::TypeError::New(env,"2 parameters expected of type string and array for addHiddenPolynomialToJVRSS ").ThrowAsJavaScriptException();
  }
  std::vector<std::string> polyStr;
  Napi::Array hiddenPolyArray = info[1].As<Napi::Array>();
  for(unsigned int i = 0; i<hiddenPolyArray.Length(); ++i){
    Napi::Value vCoeff = hiddenPolyArray[i];
    if(!vCoeff.IsString()){
      Napi::TypeError::New(env,"Unexpected type passed as part of the call to addHiddenPolynomialToJVRSS").ThrowAsJavaScriptException();
    }
    polyStr.push_back(vCoeff.As<Napi::String>());
  }
  playerGrpMetaDataPtr.get()->addHiddenPolynomialToJVRSS(info[0].As<Napi::String>(), polyStr);
  return Napi::Boolean::New(env,true);
}

Napi::Value playerGroupMetaDataWrap::addPreSignDataToJVRSS(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString()){
    Napi::TypeError::New(env,"3 parameters of type string expected in call to addPreSignDataToJVRSS").ThrowAsJavaScriptException();
  }
  playerGrpMetaDataPtr.get()->addPreSignDataToJVRSS(info[0].As<Napi::String>(), info[1].As<Napi::String>(), info[2].As<Napi::String>());
  return Napi::Boolean::New(env,true);
}

Napi::Value playerGroupMetaDataWrap::verifyCorrectness(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 0){
    Napi::TypeError::New(env, "No parameters expected for verifyCorrectness").ThrowAsJavaScriptException();
  }
  return Napi::Boolean::New(env, playerGrpMetaDataPtr.get()->verifyCorrectness());
}
Napi::Value playerGroupMetaDataWrap::verifyHonesty(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 0){
    Napi::TypeError::New(env, "No parameters expected for verifyHonesty").ThrowAsJavaScriptException();
  }
  return Napi::Boolean::New(env,playerGrpMetaDataPtr.get()->verifyHonesty());
}

Napi::Value playerGroupMetaDataWrap::createSecret(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 0){
    Napi::TypeError::New(env, "No parameters expcted for createSecret").ThrowAsJavaScriptException();
  }
  BigNumber secret = playerGrpMetaDataPtr.get()->createSecret();
  return Napi::String::New(env,secret.ToHex());
}

Napi::Value playerGroupMetaDataWrap::createGroupPublicKey(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 0){
    Napi::TypeError::New(env,"No parameters expected for createGroupPublicKey").ThrowAsJavaScriptException();
  }
  playerGrpMetaDataPtr->m_GroupPublicKey = playerGrpMetaDataPtr->createGroupPublicKey();
  return Napi::String::New(env,playerGrpMetaDataPtr->m_GroupPublicKey.ToHex());
}

Napi::Value playerGroupMetaDataWrap::CalculateEphemeralKey(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 0){
    Napi::TypeError::New(env,"No parameters expected for CalculateEphemeralKey").ThrowAsJavaScriptException();
  }
  return Napi::Boolean::New(env,playerGrpMetaDataPtr->CalculateEphemeralKey());
}

Napi::Value playerGroupMetaDataWrap::Signature(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 2 || !info[0].IsString() || !info[1].IsArray()){
    Napi::TypeError::New(env,"2 parameters expected of type string  and array in call to Signature").ThrowAsJavaScriptException();
  }

  Napi::Array keyPairArray = info[1].As<Napi::Array>();
  if(keyPairArray.Length() != 2){
    Napi::TypeError::New(env,"2 values expected for the R&S pair in Signature").ThrowAsJavaScriptException();
  }
  int index(0);
  Napi::Value vR = keyPairArray[index++];
  Napi::Value vS = keyPairArray[index];
  if(!vR.IsString() || !vS.IsString()){
    Napi::TypeError::New(env, "Unexpected parameters in the call to Signaure").ThrowAsJavaScriptException();
  }
  BigNumber bnR;
  bnR.FromHex(vR.As<Napi::String>());
  BigNumber bnS;
  bnS.FromHex(vS.As<Napi::String>());
  std::pair<BigNumber, BigNumber> keyPair = std::make_pair(bnR, bnS);
  BigNumber sig = playerGrpMetaDataPtr.get()->Signature(info[0].As<Napi::String>(), keyPair);
  return Napi::String::New(env,sig.ToHex());
}

Napi::Value playerGroupMetaDataWrap::CalcPartialSignature(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 2 || !info[0].IsString() || !info[1].IsNumber() ){
    Napi::TypeError::New(env,"2 parameters of type string and integer expected in call to CalcPartialSignature").ThrowAsJavaScriptException();
  }
  std::pair<std::string, BigNumber> sigInfo;
  try{
    sigInfo = playerGrpMetaDataPtr->CalcPartialSignature(info[0].As<Napi::String>(), info[1].As<Napi::Number>());
  }
  catch(std::exception& e){
    std::cout << e.what() << std::endl;
    Napi::TypeError::New(env,e.what()).ThrowAsJavaScriptException();
  }
  Napi::Array retVal = Napi::Array::New(env,2); 
  int index(0);
  retVal[index++] = Napi::String::New(env,sigInfo.first);
  retVal[index] = Napi::String::New(env,sigInfo.second.ToHex());
  return retVal;
}

Napi::Value playerGroupMetaDataWrap::CalculateGroupSignature(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 3 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsArray()){
    Napi::TypeError::New(env, "3 parameters expected of type string, interger, array in call to CalculateGroupSignature").ThrowAsJavaScriptException();
  }
  std::vector<std::pair<std::string, std::string> > sigs;
  Napi::Array sigArray = info[2].As<Napi::Array>();
  for(unsigned int i = 0;i<sigArray.Length();++i){
    Napi::Value vord = sigArray[i++];
    Napi::Value vS = sigArray[i];
    if(!vord.IsString() || !vS.IsString()){
      Napi::TypeError::New(env,"Unexpected parameters in signature array passed to CalculateGroupSignature").ThrowAsJavaScriptException();
    }
    sigs.push_back(std::make_pair(vord.As<Napi::String>(),vS.As<Napi::String>()));
  }
  std::pair<BigNumber, BigNumber> sig = playerGrpMetaDataPtr->CalculateGroupSignature(info[0].As<Napi::String>(),info[1].As<Napi::Number>(),sigs);
  Napi::Array sigRet = Napi::Array::New(env,2);
  int index(0);
  sigRet[index++]= Napi::String::New(env,sig.first.ToHex());
  sigRet[index]=Napi::String::New(env,sig.second.ToHex());
  return sigRet;
}

Napi::Value playerGroupMetaDataWrap::addPrivateKeyInfo(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
    Napi::TypeError::New(env,"2 parameters expected of type string in addPrivateKeyInfo").ThrowAsJavaScriptException();
  }
  playerGrpMetaDataPtr.get()->addPrivateKeyInfo(info[0].As<Napi::String>(),info[1].As<Napi::String>());
  return Napi::Boolean::New(env,true);
}

Napi::Value playerGroupMetaDataWrap::CalculateGroupPrivateKey(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 0){
    Napi::TypeError::New(env,"No parameters expected in call to CalculateGroupPrivateKey").ThrowAsJavaScriptException();

  }
  BigNumber priKey = playerGrpMetaDataPtr->CalculateGroupPrivateKey();
  return Napi::String::New(env,priKey.ToHex());
}

Napi::Value playerGroupMetaDataWrap::ValidateGroupPrivateKey(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  if(info.Length() != 1 || !info[0].IsString()){
    Napi::TypeError::New(env,"1 paramter expected of type string in ValidateGroupPrivateKey").ThrowAsJavaScriptException();
  }
  BigNumber key;
  key.FromHex(info[0].As<Napi::String>());
  return Napi::Boolean::New(env,playerGrpMetaDataPtr->ValidateGroupPrivateKey(key));
}

Napi::Value playerGroupMetaDataWrap::GetHiddenPolynomial(const Napi::CallbackInfo& info){
  int index(0);
  Napi::Array poly = Napi::Array::New(info.Env(),playerGrpMetaDataPtr->m_transientData.m_hiddenPolynomial.size());
  for(std::vector<ECPoint>::const_iterator iter = playerGrpMetaDataPtr->m_transientData.m_hiddenPolynomial.begin();
                                          iter != playerGrpMetaDataPtr->m_transientData.m_hiddenPolynomial.end(); ++ iter)
  {
    poly[index++] = Napi::String::New(info.Env(),iter->ToHex());
  }
  return poly;
}

//std::map<std::string, std::vector<std::pair<std::string, ECPoint> > > m_hiddenEvals; 
Napi::Value playerGroupMetaDataWrap::GetHiddenEvals(const Napi::CallbackInfo& info){
  int index(0);
  std::map<std::string,std::vector<std::pair<std::string, ECPoint> > >::const_iterator iter 
    = playerGrpMetaDataPtr->m_transientData.m_hiddenEvals.begin();
  // all the vectors in the container should be the same size
  unsigned int evalVectorSize = iter->second.size();
  // the extra element is so the code rebuilding the map on the other end knows how many items in the vectort
  Napi::Array hiddenevalarray = Napi::Array::New(info.Env(),(playerGrpMetaDataPtr->m_transientData.m_hiddenEvals.size() * evalVectorSize * 2)+1);
  hiddenevalarray[index++] = Napi::Number::New(info.Env(),evalVectorSize);
  for(; iter != playerGrpMetaDataPtr->m_transientData.m_hiddenEvals.end(); ++iter){
    if(evalVectorSize != iter->second.size())
      Napi::TypeError::New(info.Env(),"Invalid data in HiddenEvals data structure").ThrowAsJavaScriptException();
    hiddenevalarray[index++] = Napi::String::New(info.Env(),iter->first);
    for(std::vector<std::pair<std::string, ECPoint> >::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++ innerIter){
      hiddenevalarray[index++] = Napi::String::New(info.Env(), innerIter->first);
      hiddenevalarray[index++] = Napi::String::New(info.Env(),innerIter->second.ToHex());
    }
  }
  return hiddenevalarray;
}

Napi::Value playerGroupMetaDataWrap::EphemeralKeyPlayerData(const Napi::CallbackInfo& info){
    BigNumber kalpha = (playerGrpMetaDataPtr->m_littleK * playerGrpMetaDataPtr->m_alpha);
    ECPoint alphaG = MultiplyByGeneratorPt(playerGrpMetaDataPtr->m_alpha);
    Napi::Array keyarray = Napi::Array::New(info.Env(),2);
    int index(0);
    keyarray[index++] = Napi::String::New(info.Env(),kalpha.ToHex());
    keyarray[index] = Napi::String::New(info.Env(),alphaG.ToHex());
    return keyarray;
}