 #include <Python.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>
#include <AsymKey/AsymKeyConfig.h>
#include <AsymKey/AsymKey.h>
#include <BigNumbers/BigNumbers.h>
#include <MessageHash/conversions.h>
#include <SecretSplit/KeyShare.h>

struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject* wrap_GenerateKeyPairPEM(PyObject* self, PyObject *args)
{
    AsymKey keyGen;
    const std::pair<std::string, std::string> keyPairPEM = std::make_pair(keyGen.exportPublicPEM(), keyGen.exportPrivatePEM());
    return Py_BuildValue("ss", keyPairPEM.first.c_str(), keyPairPEM.second.c_str());
}

static PyObject* wrap_GenerateKeyPairHEX(PyObject* self, PyObject *args)
{
    AsymKey keyGen;
    const std::pair<std::string, std::string> keyPairHEX = std::make_pair(keyGen.exportPublicHEXStr(), keyGen.exportPrivateHEX());
    return Py_BuildValue("ss", keyPairHEX.first.c_str(), keyPairHEX.second.c_str());
}

static PyObject* wrap_ExportKeyPairHEX(PyObject* self, PyObject *args)
{
    char* cPrivKeyPEM = nullptr;

    if (!PyArg_ParseTuple(args, "s", &cPrivKeyPEM))
        return NULL;

    const std::string privPEMKey(cPrivKeyPEM);

    AsymKey imported_key;
    imported_key.importPrivatePEM(privPEMKey);
    const std::pair<std::string, std::string> keyPairHEX = std::make_pair(imported_key.exportPublicHEXStr(), imported_key.exportPrivateHEX());
    return Py_BuildValue("ss", keyPairHEX.first.c_str(), keyPairHEX.second.c_str());
}

static PyObject* wrap_ExportPublicPEM(PyObject* self, PyObject *args)
{
    char* cPrivateKeyPEM = nullptr;

    if (!PyArg_ParseTuple(args, "s", &cPrivateKeyPEM))
        return NULL;

    const std::string private_key_pem(cPrivateKeyPEM);

    AsymKey private_key;
    private_key.importPrivatePEM(private_key_pem);
    const std::string public_key_PEM = private_key.exportPublicPEM();

    return Py_BuildValue("s", public_key_PEM.c_str());
}

static PyObject* wrap_Sign(PyObject* self, PyObject *args)
{
    char* cPrivKey = nullptr;
    char* cMsg = nullptr;

    if (!PyArg_ParseTuple(args, "ss", &cMsg, &cPrivKey))
        return NULL;
    const std::string privPEMKey(cPrivKey);
    const std::string msg(cMsg);

    AsymKey key;
    key.importPrivatePEM(privPEMKey);
    const std::pair<std::string, std::string> sign = key.sign(msg);

    return Py_BuildValue("ss", sign.first.c_str(), sign.second.c_str());
}

static PyObject* wrap_Verify(PyObject* self, PyObject *args)
{
    char* cMsg = nullptr;
    char* cPubKey = nullptr;
    char* cSigR = nullptr;
    char* cSigS = nullptr;

    if (!PyArg_ParseTuple(args, "ssss", &cMsg, &cPubKey, &cSigR, &cSigS))
        return NULL;

    const std::string msg(cMsg);
    const std::string pubKey(cPubKey);
    const std::string sigR(cSigR);
    const std::string sigS(cSigS);

    const bool verifyOK = verify(msg, pubKey, std::make_pair(sigR, sigS));

    return Py_BuildValue("i", verifyOK);
}

static PyObject* wrap_DERSignature(PyObject* self, PyObject *args)
{
    char* cSigR = nullptr;
    char* cSigS = nullptr;
    int dec(0);

    BigNumber rValue;
    BigNumber sValue;
    if (!PyArg_ParseTuple(args, "ssi",&cSigR, &cSigS,&dec))
        return NULL;

    if(dec){
        rValue.FromDec(cSigR);
        sValue.FromDec(cSigS);
    }else{
        rValue.FromHex(cSigR);
        sValue.FromHex(cSigS);
    }

    size_t len(-1); 
    std::unique_ptr<unsigned char[]>  sigDERTest = DEREncodedSignature(rValue,sValue,len);
    
    std::string sigAsHex = binTohexStr(sigDERTest,len);
    if(dec){
        BigNumber decimalRet;
        decimalRet.FromHex(sigAsHex);
        return Py_BuildValue("s", decimalRet.ToDec().c_str());
    }
    else{
        return Py_BuildValue("s", sigAsHex.c_str());
    }
}

static PyObject* wrap_verifyDER(PyObject* self, PyObject *args)
{
    char* cSig = nullptr;
    char* cPubKey = nullptr;
    char* msg = nullptr;
    int dec(0);

    if (!PyArg_ParseTuple(args, "sssi",&msg, &cPubKey, &cSig,&dec))
        return NULL;

    std::string sig(cSig);
    std::string key(cPubKey); 
    std::string msgStr(msg);

    size_t sigBinLen(0);
    std::unique_ptr<unsigned char[]> sigBin = HexStrToBin(sig,&sigBinLen);

    const bool verifyOK = verifyDER(msgStr, key, sigBin,sigBinLen);

    return Py_BuildValue("i", verifyOK);
}


static PyObject* wrap_ShareSecret(PyObject* self, PyObject *args)
{
    char* cMyPrivateKeyPEM = nullptr;
    char* cTheirPublicKeyPEM = nullptr;

    if (!PyArg_ParseTuple(args, "ss", &cMyPrivateKeyPEM, &cTheirPublicKeyPEM))
        return NULL;

    const std::string my_private_key_pem(cMyPrivateKeyPEM);
    const std::string their_public_key_pem(cTheirPublicKeyPEM);

    AsymKey my_private_key;
    my_private_key.importPrivatePEM(my_private_key_pem);
    const std::string shared_secrete = my_private_key.exportSharedSecretHex(their_public_key_pem);

    return Py_BuildValue("s", shared_secrete.c_str());
}

static PyObject* wrap_DerivePublic(PyObject* self, PyObject *args)
{
    char* cPublicKey = nullptr;
    char* cMsg = nullptr;

    if (!PyArg_ParseTuple(args, "ss", &cPublicKey, &cMsg))
        return NULL;

    const std::string public_key_pem(cPublicKey);
    const std::string msg(cMsg);

    const std::string derived_pub_key = derive_pubkey(public_key_pem, msg);

    return Py_BuildValue("s", derived_pub_key.c_str());
}

static PyObject* wrap_DerivePrivate(PyObject* self, PyObject *args)
{
    char* cPrivateKey = nullptr;
    char* cMsg = nullptr;

    if (!PyArg_ParseTuple(args, "ss", &cPrivateKey, &cMsg))
        return NULL;

    const std::string private_key_pem(cPrivateKey);
    const std::string msg(cMsg);

    AsymKey private_key;
    private_key.importPrivatePEM(private_key_pem);

    const AsymKey derived_key = private_key.derive(msg);
    const std::string derived_key_PEM = derived_key.exportPrivatePEM();

    return Py_BuildValue("s", derived_key_PEM.c_str());
}

static PyObject* wrap_SplitKey(PyObject* self, PyObject *args)
{
    char* cPrivateKey = nullptr;
    int ThresholdNumber(0);
    int TotalNumberOfShares(0);

    if (!PyArg_ParseTuple(args, "sii", &cPrivateKey, &ThresholdNumber, &TotalNumberOfShares))
        return NULL;

    const std::string private_key_pem(cPrivateKey);


    AsymKey private_key;
    private_key.importPrivatePEM(private_key_pem);

    std::vector<KeyShare> shares = private_key.split(ThresholdNumber, TotalNumberOfShares);
    //std::stringstream shareBuilder;
    PyObject *pyList, *item;
    int i=0;
    pyList = PyList_New(shares.size());
    for(std::vector<KeyShare>::const_iterator iter = shares.begin(); iter != shares.end(); ++ iter ){
        std::string itemJson = keyshare_to_json(*iter);
        item = PyUnicode_FromStringAndSize(itemJson.c_str(),itemJson.length());
        PyList_SetItem(pyList, i,item);
        ++ i;
    }
    return pyList;
}
static PyObject* wrap_RestoreKey(PyObject* self, PyObject *args)
{
    char* sharesAsStr = nullptr;

    if (!PyArg_ParseTuple(args, "s", &sharesAsStr))
        return NULL;
 
    std::string inputval(sharesAsStr);
    std::regex reg("\\;");
    std::sregex_token_iterator iter(inputval.begin(), inputval.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
    std::vector<KeyShare> shares;
    for(std::vector<std::string>::const_iterator iter = vec.begin(); iter != vec.end(); ++ iter){
        shares.push_back(keyshare_from_json(*iter));
    }

    AsymKey private_key;
    try{
        private_key.recover(shares);
    }
    catch(std::exception& err){
        return Py_BuildValue("ss", err.what(), err.what());
    }
    return Py_BuildValue("ss", private_key.exportPublicPEM().c_str(),private_key.exportPrivatePEM().c_str());
}

static PyObject* wrap_ImportFromPEM(PyObject* self, PyObject *args)
{
    char* cPrivateKey = nullptr;

    if (!PyArg_ParseTuple(args, "s", &cPrivateKey))
        return NULL;

    const std::string private_key_pem(cPrivateKey);

    AsymKey private_key;
    private_key.importPrivatePEM(private_key_pem);
    if (private_key.is_valid()){
        return Py_BuildValue("ss",private_key.exportPublicPEM().c_str(), private_key.exportPrivatePEM().c_str());
    }else{
        std::string val1, val2; 
        return Py_BuildValue("ss",std::string().c_str(), std::string().c_str());
    }

}

static PyObject* wrap_ImportFromEncryptedPEM(PyObject* self, PyObject *args)
{
    char* cPrivateKey   = nullptr ;
    char* cPassPhrase   = nullptr ;

    if (!PyArg_ParseTuple(args, "ss", &cPrivateKey, &cPassPhrase))
        return NULL;

    const std::string private_key_pem( cPrivateKey ) ;
    const std::string pass_phrase( cPassPhrase ) ;

    AsymKey private_key;
    private_key.importPrivatePEMEncrypted(private_key_pem, pass_phrase);
    if (private_key.is_valid()){
        return Py_BuildValue("ss",private_key.exportPublicPEM().c_str(), private_key.exportPrivatePEM().c_str());
    }else{
        std::string val1, val2; 
        return Py_BuildValue("ss",std::string().c_str(), std::string().c_str());
    }
}

static PyObject* wrap_ExportFromEncryptedPEM(PyObject* self, PyObject *args)
{
    char* cPassPhrase   = nullptr ;

    if (!PyArg_ParseTuple(args, "s", &cPassPhrase))
        return NULL;

    const std::string pass_phrase( cPassPhrase ) ;

    AsymKey private_key;
    std::string privkey_str( private_key.exportPrivatePEMEncrypted( pass_phrase) );

    if (private_key.is_valid())
    {
        return Py_BuildValue("s", privkey_str.c_str());
    }
    else
    {        
        return Py_BuildValue("s", "") ;
    }

}

static PyObject*  wrap_PubKeyPEMToHex(PyObject* self, PyObject *args)
{
    char* pubkeypem   = nullptr ;
    int compressed=0;

    if (!PyArg_ParseTuple(args, "si", &pubkeypem,&compressed))
        return NULL;

    const std::string pubkeystr( pubkeypem ) ;

    std::string keyPt;
    if(compressed){
        keyPt = pubkey_pem2Hex_point(pubkeystr, true);
    }else{
        keyPt = pubkey_pem2Hex_point(pubkeystr, false);
    }
    return Py_BuildValue("s",keyPt.c_str());
}


static PyObject*  wrap_PubKeyHexPtToPEM(PyObject* self, PyObject *args){
    char* xpt = nullptr;
    char* ypt = nullptr;
    int nid=0;

    if (!PyArg_ParseTuple(args, "ssi", &xpt,&ypt,&nid))
        return NULL;
    
    std::string pemPubKey = pubkey_coordinates2pem(xpt,ypt, nid);
    return Py_BuildValue("s",pemPubKey.c_str());
}

static PyMethodDef ModuleMethods[] =
{
    // {"test_get_data_nulls", wrap_test_get_data_nulls, METH_NOARGS, "Get a string of fixed length with embedded nulls"},
    {"GenerateKeyPairPEM"       , wrap_GenerateKeyPairPEM,METH_VARARGS,"Generate pair of keys in pem format"},
    {"GenerateKeyPairHEX"       , wrap_GenerateKeyPairHEX,METH_VARARGS,"Generate pair of keys in hex format"},
    {"ExportKeyPairHEX"         , wrap_ExportKeyPairHEX,METH_VARARGS,"Get pair of keys in hex format from a private PEM key"},
    {"ExportPublicPEM"          , wrap_ExportPublicPEM,METH_VARARGS,"Get public key PEM given the private key PEM"},
    {"Sign"                     , wrap_Sign,METH_VARARGS,"Sign message with private Key"},
    {"Verify"                   , wrap_Verify,METH_VARARGS,"Verify message's signature with public key"},
    {"ShareSecret"              , wrap_ShareSecret,METH_VARARGS,"Calculate shared secret from my private key and their public key"},
    {"DerivePublic"             , wrap_DerivePublic,METH_VARARGS,"Derive public key from a given public key and a additive message"},
    {"DerivePrivate"            , wrap_DerivePrivate,METH_VARARGS,"Derive private key from a given private key and a additive message"},
    {"SplitKey"                 , wrap_SplitKey,METH_VARARGS,"Split a private key into a given number of shares"},
    {"RestoreKey"               , wrap_RestoreKey, METH_VARARGS,"Restore a private key from a given number of shares"},
    {"ImportFromPem"            , wrap_ImportFromPEM, METH_VARARGS,"Imports a key in PEM format"},
    {"ImportFromEncryptedPEM"   , wrap_ImportFromEncryptedPEM, METH_VARARGS,"Imports a key in Encrypted PEM format, with pass phrase"},
    {"ExportFromEncryptedPEM"   , wrap_ExportFromEncryptedPEM, METH_VARARGS,"Exports a key to Encrypted PEM format, with pass phrase"},
    {"DERSignature"             , wrap_DERSignature, METH_VARARGS,"return a DER encoding of an (r,s) signature"},
    {"PubKeyPEMToHexPt"         , wrap_PubKeyPEMToHex, METH_VARARGS,"Convert a PEM public key to a hex pt (compressed or uncompressed)"},
    {"PubKeyHexPtToPEM"         , wrap_PubKeyHexPtToPEM, METH_VARARGS,"Convert an EC public key (affine coordinates to PEM"},
    {"VerifyDER"                , wrap_verifyDER,METH_VARARGS,"Verify message's signature in DER format with public key"},
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int PyAsymKey_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int PyAsymKey_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "PyAsymKey",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        PyAsymKey_traverse,
        PyAsymKey_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_PyAsymKey(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("PyAsymKeyExtension", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("PyAsymKeyExtension.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
