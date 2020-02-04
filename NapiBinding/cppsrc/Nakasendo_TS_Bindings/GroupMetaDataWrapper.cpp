#include <sstream>
#include <GroupMetaDataWrapper.h>

Napi::FunctionReference GroupMetaDataWrap::constructor;

Napi::Object GroupMetaDataWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);


  Napi::Function func = DefineClass(env, "GroupMetaDataWrap", {
    InstanceMethod("printPlayerGrp", &GroupMetaDataWrap::printPlayerGrp),
    InstanceMethod("GetGroupID",&GroupMetaDataWrap::GetGroupID),
    InstanceMethod("SetGroupID",&GroupMetaDataWrap::SetGroupID),
    InstanceMethod("SetProposer",&GroupMetaDataWrap::SetProposer),
    InstanceMethod("GetProposer",&GroupMetaDataWrap::GetProposer),
    InstanceMethod("GetM",&GroupMetaDataWrap::GetM),
    InstanceMethod("SetM",&GroupMetaDataWrap::SetM),
    InstanceMethod("GetN",&GroupMetaDataWrap::GetN),
    InstanceMethod("SetN",&GroupMetaDataWrap::SetN),
    InstanceMethod("GetT",&GroupMetaDataWrap::GetN),
    InstanceMethod("SetT",&GroupMetaDataWrap::SetN),
    InstanceMethod("SetGroupSet",&GroupMetaDataWrap::GetGroupSet),
    InstanceMethod("SetGroupSet",&GroupMetaDataWrap::SetGroupSet),
    InstanceMethod("GetGroupInviteReplies",&GroupMetaDataWrap::GetGroupInviteReplies),
    InstanceMethod("SetGroupInviteReplies",&GroupMetaDataWrap::SetGroupInviteReplies),
    InstanceMethod("IncGroupInviteReplies",&GroupMetaDataWrap::IncGroupInviteReplies),
    InstanceMethod("GetGroupSignatureReplies",&GroupMetaDataWrap::GetGroupSignatureReplies),
    InstanceMethod("SetGroupSignatureReplies",&GroupMetaDataWrap::SetGroupSignatureReplies),
    InstanceMethod("IncGroupSignatureReplies",&GroupMetaDataWrap::IncGroupSignatureReplies),
    InstanceMethod("GetParticipantList",&GroupMetaDataWrap::GetParticipantList),
    InstanceMethod("AddUserToGroup",&GroupMetaDataWrap::AddUserToGroup),
    InstanceMethod("RemoveUserFromGroup",&GroupMetaDataWrap::removeUserFromGroup),
    InstanceMethod("SetCalculationType",&GroupMetaDataWrap::SetCalculationType),
    InstanceMethod("GetCalculationType",&GroupMetaDataWrap::GetCalculationType),
    InstanceMethod("CollectedHiddenPolys",&GroupMetaDataWrap::CollectedHiddenPolys),
    InstanceMethod("AddCollatedHiddenPolys",&GroupMetaDataWrap::AddCollatedHiddenPolys),
    InstanceMethod("CollatedHiddenEvals",&GroupMetaDataWrap::CollatedHiddenEvals),
    InstanceMethod("AddCollatedHiddenEvals",&GroupMetaDataWrap::AddCollatedHiddenEvals),
    InstanceMethod("CollatedVW",&GroupMetaDataWrap::CollatedVW),
    InstanceMethod("AddCollatedVW",&GroupMetaDataWrap::AddCollatedVW),
    InstanceMethod("CollatedPartialSignatures",&GroupMetaDataWrap::CollatedPartialSignatures),
    InstanceMethod("AddCollatedPartialSignature",&GroupMetaDataWrap::AddCollatedPartialSignature),
    InstanceMethod("ClearSharedDataContainers",&GroupMetaDataWrap::ClearSharedDataContainers),
    InstanceMethod("ClearSignatureContainers",&GroupMetaDataWrap::ClearSignatureContainers),
    InstanceMethod("ClearPreSignatureContainers",&GroupMetaDataWrap::ClearPreSignatureContainers),
  });
  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("GroupMetaDataWrap", func);
  return exports;
}

GroupMetaDataWrap::GroupMetaDataWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<GroupMetaDataWrap>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if(info.Length() == 0){
    GrpMetaDataPtr.reset(new GroupMetadata);
    return ; 
  }else if (info.Length() == 1){
    // copy constructor
     Napi::Object object_parent = info[0].As<Napi::Object>();
     GroupMetaDataWrap* parent = Napi::ObjectWrap<GroupMetaDataWrap>::Unwrap(object_parent);
     GrpMetaDataPtr.reset(parent->getInternalInstance().get());
     return;
  }

  if(info.Length() != 8 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsString() 
        || !info[4].IsString() || !info[5].IsNumber() || !info[6].IsNumber() || !info[7].IsNumber()){
    Napi::TypeError::New(env, "Expected 8 parameters -> 5 strings and 3 integers").ThrowAsJavaScriptException();
  }

  playerCommsInfo p;
  p.m_userid = info[1].As<Napi::String>();
  p.m_uri = info[2].As<Napi::String>();
  p.m_ip = info[3].As<Napi::String>();
  p.m_port = info[4].As<Napi::String>();

  GrpMetaDataPtr.reset(new GroupMetadata(info[0].As<Napi::String>(),p , info[5].As<Napi::Number>(), info[6].As<Napi::Number>(),
                        info[7].As<Napi::Number>()));
  return;
}

Napi::Value GroupMetaDataWrap::printPlayerGrp(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length () != 0){
        Napi::TypeError::New(env, "No parameters expected for printing").ThrowAsJavaScriptException();
    }
    std::stringstream output;
    output << *(GrpMetaDataPtr.get());
    return Napi::String::New(env, output.str());
}


Napi::Value GroupMetaDataWrap::GetGroupID(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    return Napi::String::New(env,GrpMetaDataPtr->groupid());
} 

Napi::Value GroupMetaDataWrap::SetGroupID(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if((info.Length() != 1) || !info[0].IsString()){
        Napi::TypeError::New(env,"A single parameter of type string is expected in call to SetGroupID").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->groupid() = info[0].As<Napi::String>();
    return Napi::Boolean::New(env,true);
}

Napi::Value GroupMetaDataWrap::GetProposer(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    return Napi::String::New(env,GrpMetaDataPtr->proposer());

}
Napi::Value GroupMetaDataWrap::SetProposer(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if((info.Length() != 1) || !info[0].IsString()){
        Napi::TypeError::New(env,"A single parameter of type string is expected in call to SetPropsoer").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->proposer() = info[0].As<Napi::String>();
    return Napi::Boolean::New(env,true);
}

Napi::Value GroupMetaDataWrap::GetM(const Napi::CallbackInfo& info){
    return Napi::Number::New(info.Env(),GrpMetaDataPtr->m());
}
Napi::Value GroupMetaDataWrap::SetM(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if((info.Length() != 0) || !info[0].IsNumber()){
        Napi::TypeError::New(env,"A single parameter of type integer expected inc all to SetM").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->m() = info[0].As<Napi::Number>();
    return Napi::Boolean::New(env,true);
}

Napi::Value GroupMetaDataWrap::GetN(const Napi::CallbackInfo& info){
    return Napi::Number::New(info.Env(),GrpMetaDataPtr->n());
}
Napi::Value GroupMetaDataWrap::SetN(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if((info.Length() != 0) || !info[0].IsNumber()){
        Napi::TypeError::New(env,"A single parameter of type integer expected inc all to SetN").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->n() = info[0].As<Napi::Number>();
    return Napi::Boolean::New(env,true);
}

Napi::Value GroupMetaDataWrap::GetT(const Napi::CallbackInfo& info){
    return Napi::Number::New(info.Env(),GrpMetaDataPtr->t());
}
Napi::Value GroupMetaDataWrap::SetT(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if((info.Length() != 0) || !info[0].IsNumber()){
        Napi::TypeError::New(env,"A single parameter of type integer expected inc all to SetT").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->t() = info[0].As<Napi::Number>();
    return Napi::Boolean::New(env,true);
}

Napi::Value GroupMetaDataWrap::GetGroupSet(const Napi::CallbackInfo& info){
    return Napi::Boolean::New(info.Env(),GrpMetaDataPtr->GroupSet());
}
Napi::Value GroupMetaDataWrap::SetGroupSet(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 1 || !info[0].IsBoolean()){
        Napi::TypeError::New(env, "a single parameter of type boolean expected in SetGroupSet").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->GroupSet() = info[0].As<Napi::Boolean>();
    return Napi::Boolean::New(env,true);
}
Napi::Value GroupMetaDataWrap::GetGroupInviteReplies(const Napi::CallbackInfo& info){
    return Napi::Number::New(info.Env(),GrpMetaDataPtr->GroupInviteReplies());
}
Napi::Value GroupMetaDataWrap::SetGroupInviteReplies(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length () != 1 || !info[0].IsNumber()){
        Napi::TypeError::New(env, "A single parameter of type integer expected in SetGroupInviteReplies").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->GroupInviteReplies() = info[0].As<Napi::Number>();
    return Napi::Boolean::New(env, true);
}
Napi::Value GroupMetaDataWrap::IncGroupInviteReplies(const Napi::CallbackInfo& info){
    GrpMetaDataPtr->GroupInviteReplies() += 1;
    return Napi::Boolean::New(info.Env(), true);
}

Napi::Value GroupMetaDataWrap::GetGroupSignatureReplies(const Napi::CallbackInfo& info){
    return Napi::Number::New(info.Env(),GrpMetaDataPtr->GroupSignatureReplies());
}
Napi::Value GroupMetaDataWrap::SetGroupSignatureReplies(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length () != 1 || !info[0].IsNumber()){
        Napi::TypeError::New(env, "A single parameter of type integer expected in SetGroupSignatureReplies").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->GroupSignatureReplies() = info[0].As<Napi::Number>();
    return Napi::Boolean::New(env, true);
}

Napi::Value GroupMetaDataWrap::IncGroupSignatureReplies(const Napi::CallbackInfo& info){
    GrpMetaDataPtr->GroupSignatureReplies() += 1;
    return Napi::Boolean::New(info.Env(), true);
}

Napi::Value GroupMetaDataWrap::GetParticipantList(const Napi::CallbackInfo& info){  
    Napi::Array playersarray = Napi::Array::New(info.Env(),GrpMetaDataPtr->participantList().size() * 4);
    int index(0);
    for(std::vector<playerCommsInfo>::const_iterator iter = GrpMetaDataPtr->participantList().begin();
            iter != GrpMetaDataPtr->participantList().end(); ++iter){       
        playersarray[index++] = Napi::String::New(info.Env(),iter->m_userid);
        playersarray[index++] = Napi::String::New(info.Env(),iter->m_uri);
        playersarray[index++] = Napi::String::New(info.Env(),iter->m_ip);
        playersarray[index++] = Napi::String::New(info.Env(),iter->m_port);
    }
    return playersarray;
}

Napi::Value GroupMetaDataWrap::AddUserToGroup(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length () != 4 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsString()){
        Napi::TypeError::New(env,"4 parameters of type string expected in call to AddUserToGroup").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->addUserToGroup(info[0].As<Napi::String>(), info[1].As<Napi::String>(), info[2].As<Napi::String>(),info[3].As<Napi::String>());
    return Napi::Boolean::New(env,true);
}

Napi::Value GroupMetaDataWrap::removeUserFromGroup(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 1 || !info[0].IsString()){
        Napi::TypeError::New(env,"a single parameter of type string expected in call to removeUserFromGroup").ThrowAsJavaScriptException();
    }
    return Napi::Boolean::New(env, GrpMetaDataPtr->removeUserFromparticipantList(info[0].As<Napi::String>()));
}

Napi::Value GroupMetaDataWrap::SetCalculationType(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 1 || !info[0].IsString()){
        Napi::TypeError::New(env,"a single parameter of type string expected in call to SetCalculationType").ThrowAsJavaScriptException();
    }
     GrpMetaDataPtr->calculationType() = info[0].As<Napi::String>();
     return Napi::Boolean::New(env, true);
}

Napi::Value GroupMetaDataWrap::GetCalculationType(const Napi::CallbackInfo& info){
    return Napi::String::New(info.Env(), GrpMetaDataPtr->calculationType());
}

Napi::Value GroupMetaDataWrap::CollectedHiddenPolys(const Napi::CallbackInfo& info){
    if(GrpMetaDataPtr->CollatedHiddenPolys ().size() == 0){
        Napi::TypeError::New(info.Env(),"No entries in the CollatedHiddenPolys data structure").ThrowAsJavaScriptException();
    }
    std::map<std::string, std::vector<ECPoint> >::const_iterator iter = GrpMetaDataPtr->CollatedHiddenPolys().begin();
    unsigned int polySize = iter->second.size();
    int index(0);
    Napi::Array hiddenPolysArray = Napi::Array::New(info.Env(),(GrpMetaDataPtr->CollatedHiddenPolys ().size() * polySize) + 1);
    hiddenPolysArray[index++] = Napi::Number::New(info.Env(),polySize);
    for(;iter != GrpMetaDataPtr->CollatedHiddenPolys ().end(); ++iter){
        hiddenPolysArray[index++] = Napi::String::New(info.Env(),iter->first);
        if( iter->second.size () != polySize){
            Napi::TypeError::New(info.Env(),"Invalid size for one of the hidden polys. It's not equal to the others").ThrowAsJavaScriptException();
        }
        for(std::vector<ECPoint>::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++innerIter){
            hiddenPolysArray[index++] = Napi::String::New(info.Env(),innerIter->ToHex());
        }
    }
    return hiddenPolysArray;
}

Napi::Value GroupMetaDataWrap::AddCollatedHiddenPolys(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 2 || !info[0].IsString() || !info[1].IsArray()){
        Napi::TypeError::New(env, "2 parameters expected of type string and array of strings in call to AddCollatedHiddenPolys").ThrowAsJavaScriptException();
    }
    Napi::Array polyarray = info[1].As<Napi::Array>();
    std::vector<std::string> poly; 
    for(unsigned int i =0;i<polyarray.Length();++i){
        Napi::Value v = polyarray[i];
        if(!v.IsString()){
            Napi::TypeError::New(env,"Unexpected value type passed as part of the array parameter in AddCollatedHiddenPolys").ThrowAsJavaScriptException();
        }
        poly.push_back(v.As<Napi::String>());
    }
    GrpMetaDataPtr->addCollatedHiddenPolys(info[0].As<Napi::String>(),poly);
    return Napi::Boolean::New(env,true);
}

Napi::Value GroupMetaDataWrap::CollatedHiddenEvals (const Napi::CallbackInfo& info){
    if(GrpMetaDataPtr->CollatedHiddenEvals().size() == 0){
        Napi::TypeError::New(info.Env(), "No entries in the CollatedHiddenEvals structure").ThrowAsJavaScriptException();
    }
    std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::const_iterator iter = GrpMetaDataPtr->CollatedHiddenEvals().begin();
    unsigned int evalVectSize = iter->second.size();
    int index(0);
    Napi::Array HiddenEvalsArray = Napi::Array::New(info.Env(),(GrpMetaDataPtr->CollatedHiddenEvals().size()*evalVectSize*2)+1);
    HiddenEvalsArray[index++] = Napi::Number::New(info.Env(),evalVectSize);
    for(;iter != GrpMetaDataPtr->CollatedHiddenEvals().end();++iter){
        HiddenEvalsArray[index++] = Napi::String::New(info.Env(),iter->first);
        if(iter->second.size() != evalVectSize){
            Napi::TypeError::New(info.Env(),"Some hidden evals appear to be missing or too many supplied CollatedHiddenEvals").ThrowAsJavaScriptException();
        }
        for(std::vector<std::pair<std::string, ECPoint> >::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++ innerIter){
            HiddenEvalsArray[index++] = Napi::String::New(info.Env(), innerIter->first);
            HiddenEvalsArray[index++] = Napi::String::New(info.Env(), innerIter->second.ToHex());
        }
    }
    return HiddenEvalsArray;
}

Napi::Value GroupMetaDataWrap::AddCollatedHiddenEvals(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString()){
        Napi::TypeError::New(env, "3 parameters expected of type string in call AddCollatedHiddenEvals").ThrowAsJavaScriptException();
    }

    GrpMetaDataPtr->addCollatedHiddenEvals(info[0].As<Napi::String>(),info[1].As<Napi::String>(),info[2].As<Napi::String>());
    
    return Napi::Boolean::New(env, true);
}

Napi::Value GroupMetaDataWrap::CollatedVW(const Napi::CallbackInfo& info){
    if(GrpMetaDataPtr->CollatedVW().size() ==0){
        Napi::TypeError::New(info.Env(), "No entries in the CollatedVW list").ThrowAsJavaScriptException();
    }
    int index(0);
    Napi::Array VWArray = Napi::Array::New(info.Env(), GrpMetaDataPtr->CollatedVW().size()*3);
    for(std::map<std::string, std::pair<BigNumber, ECPoint> >::const_iterator iter = GrpMetaDataPtr->CollatedVW().begin();
            iter != GrpMetaDataPtr->CollatedVW().end(); ++ iter){
        VWArray[index++] = Napi::String::New(info.Env(),iter->first);
        VWArray[index++] = Napi::String::New(info.Env(),iter->second.first.ToHex());
        VWArray[index++] = Napi::String::New(info.Env(),iter->second.second.ToHex());
    }
    return VWArray;
}
Napi::Value GroupMetaDataWrap::AddCollatedVW(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsString()){
        Napi::TypeError::New(env, "3 parameters of type string in call to AddCollatedVW").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->addCollatedVW(info[0].As<Napi::String>(),info[1].As<Napi::String>(),info[2].As<Napi::String>());
    return Napi::Boolean::New(env,true);
}

Napi::Value GroupMetaDataWrap::CollatedPartialSignatures(const Napi::CallbackInfo& info){
    if(GrpMetaDataPtr->CollatedPartialSignatures().size() == 0){
        Napi::TypeError::New(info.Env(),"No entries in the CollatedPartialSignatures").ThrowAsJavaScriptException();
    }
    Napi::Array sigArray = Napi::Array::New(info.Env(),GrpMetaDataPtr->CollatedPartialSignatures().size()*2);
    int index(0);
    for(std::vector<std::pair<std::string,BigNumber> >::const_iterator iter = GrpMetaDataPtr->CollatedPartialSignatures().begin();
            iter != GrpMetaDataPtr->CollatedPartialSignatures().end(); ++ iter){
        sigArray[index++] = Napi::String::New(info.Env(),iter->first);
        sigArray[index++] = Napi::String::New(info.Env(),iter->second.ToHex());
    }
    return sigArray;
}

Napi::Value GroupMetaDataWrap::AddCollatedPartialSignature(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() != 2 || !info[0].IsString() || !info[1].IsString()){
        Napi::TypeError::New(env, "2 parameters of type string expected in call to AddCollatedPartialSignature").ThrowAsJavaScriptException();
    }
    GrpMetaDataPtr->addCollatedPartialSignautre(info[0].As<Napi::String>(),info[1].As<Napi::String>());
    return Napi::Boolean::New(env,true);
}

Napi::Value GroupMetaDataWrap::ClearSharedDataContainers(const Napi::CallbackInfo& info){
    GrpMetaDataPtr->clearSharedDataContainers();
    return Napi::Boolean::New(info.Env(),true);
}
Napi::Value GroupMetaDataWrap::ClearSignatureContainers(const Napi::CallbackInfo& info){
    GrpMetaDataPtr->clearSignatureContainers();
    return Napi::Boolean::New(info.Env(),true);
}
Napi::Value GroupMetaDataWrap::ClearPreSignatureContainers(const Napi::CallbackInfo& info){
    GrpMetaDataPtr->clearPreSignatureContainers();
    return Napi::Boolean::New(info.Env(),true);
}

Napi::Value GroupMetaDataWrap::isGroupLocked(const Napi::CallbackInfo& info){
    return Napi::Boolean::New(info.Env(),GrpMetaDataPtr->isLocked());
}
Napi::Value GroupMetaDataWrap::LockGroup(const Napi::CallbackInfo& info){
    GrpMetaDataPtr->LockGroup();
    return Napi::Boolean::New(info.Env(),true);
}
Napi::Value GroupMetaDataWrap::UnLockGroup(const Napi::CallbackInfo& info){
    GrpMetaDataPtr->unLockGroup();
    return Napi::Boolean::New(info.Env(),true);
}

