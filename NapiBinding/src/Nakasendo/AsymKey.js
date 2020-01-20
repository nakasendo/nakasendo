const bindings = require('../../build/Release/nakasendo.node');
var{
    ECPoint,
    AddECPoint,
    EC_Compare,
    EC_Double,
    EC_Invert,
    EC_ScalerMultiply,
    EC_MultiplyScalerByGenerator
} = require('./ECPoint.js')


class AsymKey{
    #m_PriKey;
    #m_PubKey;
    constructor(){
        var keyArr = bindings.GenerateKeyPairPEM();
        this.#m_PriKey=keyArr[1];
        this.#m_PubKey = keyArr[0];
    }
    
    
    
    get pubkey(){
        return this.#m_PubKey; 
    }
    
    FromEncryptedPEMStr (keyPemForm, passPhrase){
        var keyArr = bindings.ImportFromEncryptedPEM(keyPemForm, passPhrase);
        this.#m_PubKey = keyArr[0];
        this.#m_PriKey = keyArr[1];
     }

    ToEncryptedPEMStr (passPhrase){
        return bindings.ExportFromEncryptedPEM(passPhrase);
    }
            

    derivePrivateKey(msg){
        return bindings.DerivePrivate(this.#m_PriKey, msg);
    }
        
     
    derivePublicKey(msg){
        return bindings.DerivePublic(this.#m_PubKey, msg);
    }
        
    SplitKey(threshold=10,shares=100){
        return bindings.SplitKey(this.#m_PriKey, threshold, shares);
    }
               
    RecoverKey(shares){
        var keys = bindings.RestoreKey(shares);
        this.#m_PubKey = keys[0];
        this.#m_PriKey = keys[1];
    }
        
        
    sign(msg){
        return bindings.Sign ( msg, this.#m_PriKey)
    }
        
    CalculateSharedSecret(pubkey){
        return bindings.ShareSecret(this.#m_PriKey, pubkey)
    }
    
    toString(){
        return this.#m_PubKey; 
    }
    
    PubKeyHex (){
        var key = bindings.ExportKeyPairHEX(this.#m_PriKey);
        return key[0];
    }
    
}

function verify(msg, pubkey, rval, sval){
    return bindings.Verify(msg, pubkey, rval,sval);
}

function verifyPubKeyHex(msg, pubkey, rval, sval){
    pubKeyPt = new ECPoint();
    pubKeyPt.SetValue =  pubkey;
    var coords = pubKeyPt.GetAffineCoordinates();
    pubkeypem = pubKeyHexPtstoPEM(coords['x'],coords['y']);
    return bindings.Verify(msg, pubkeypem, rval,sval);
}

function verifyDER(msg, pubkey, sigDER){
    return bindings.VerifyDER(msg,pubkey,sigDER);
}
function verifyDERPubKeyHex(msg, pubkey, sigDER){
    pubKeyPt = new ECPoint();
    pubKeyPt.SetValue =  pubkey;
    var coords = pubKeyPt.GetAffineCoordinates();
    pubkeypem = pubKeyHexPtstoPEM(coords['x'],coords['y']);
    return bindings.VerifyDER(msg,pubkeypem,sigDER);
}
    
function createDERFormat(rValue, sValue,isdecimal){
    // check that the decimal flag is set both ways?
    hexSig = bindings.DERSignature(rValue, sValue,isdecimal);
    return hexSig;
}

function pubKeyAsHexPt(pubkey, iscompressed=false){
    pt = bindings.PubKeyPEMToHex(pubkey, iscompressed);
    return pt;
}

function pubKeyHexPtstoPEM(xVal, yVal,nid=714){
    pemStr = bindings.pubKeyHextoPEM(xVal,yVal,nid);
    return pemStr;
}

module.exports = {
    AsymKey,
    verify,
    verifyPubKeyHex,
    createDERFormat,
    verifyDER,
    pubKeyAsHexPt,
    pubKeyHexPtstoPEM
}

