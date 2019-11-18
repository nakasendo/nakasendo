#!/usr/bin/python

import random
import sys
import hashlib
import ecdsa

G= ecdsa.SECP256k1.generator
N = ecdsa.SECP256k1.order

# NIST Curve P-192:
p = 6277101735386680763835789423207666416083908700390324961279
r = 6277101735386680763835789423176059013767194773182842284081
# s = 0x3045ae6fc8422f64ed579528d38120eae12196d5
# c = 0x3099d2bbbfcb2538542dcd5fb078b6ef5f3d6fe2c745de65
b = 0x64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1
Gx = 0x188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012
Gy = 0x07192b95ffc8da78631011ed6b24cdd573f977a11e794811


c192 = ecdsa.ellipticcurve.CurveFp(p, -3, b)
p192 = ecdsa.ellipticcurve.Point(c192, Gx, Gy, r)



try:
    import PyBigNumbers
    import PyECPoint
    import PySymEncDec
    import PyMessageHash
    import PyAsymKey
    import PyPolynomial
    import Nakasendo
except ImportError as e:
    print('Error while loading SDKLibraries python modules {}'.format(e.message))
    print('Try to define environment variable SDKLIBRARIES_ROOT pointing to the location of installed SDKLibraries or add this to PYTHONPATH')
    raise ImportError('Unable to load SDKLibraries python modules')


def HashMsg(msg,modulo=None, IsDec=False):
    HASHMSG = Nakasendo.hash256(msg)
    if(IsDec):
        Hm = Nakasendo.BigNum(str(int(HASHMSG.value,16)), modulo.value, isDec=True)
    else:
        Hm = Nakasendo.BigNum(HASHMSG.value,modulo.value, isDec=False)
    return Hm

if __name__ == "__main__":
    print ("...starting....")
    #print (sys.argv)
    sigRStr = sys.argv[1]
    sigSStr = sys.argv[2]
    pubKeyStr = sys.argv[3]
    message = sys.argv[4]
    
    dersigStr = None
    if(len(sys.argv) == 6):
        dersigStr=sys.argv[5]
    
    print ('[%s, %s] \n pubkey %s \nmessage = %s' %(sigRStr,sigSStr,pubKeyStr,message))
    # create an ECPoint
    pt = Nakasendo.ECPoint()
    pt.value = pubKeyStr
    print(pt.GetAffineCoOrdinates())
    
    coords = pt.GetAffineCoOrdinates()
    
    pempubkey = Nakasendo.pubKeyHexPtasPem(coords[0],coords[1])
    print(pempubkey)
    print('verify the pem format')
    pubKeyStrValidate = Nakasendo.pubKeyPEMasHex(pempubkey, True);
    print(pubKeyStrValidate)
    #message = 'I love deadlines. I love the whooshing noise they make as they go by.'
    # get the affine co-ordinates
    # get pem format
    
    val = Nakasendo.verify(message, pempubkey, sigRStr, sigSStr)
    if(val == True):
        print('Calculated signature verified via Nakasendo verify')
        
    #hmMessage = '8C31C0E267AFC3FA1AAF7955960874FCDBE9BEE0F798FD9EF3E7B07E95964752'
    hmMessage = Nakasendo.hash256(message=message,modulo="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
    #create an ecdsa public key from the program parameter
    ecdsaPubKeyPoint = ecdsa.ellipticcurve.Point(ecdsa.SECP256k1.generator.curve(), int(coords[0],16), int(coords[1],16))
    pubkeytestB = ecdsa.ecdsa.Public_key(G, ecdsaPubKeyPoint)
    
    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(sigRStr,16),int(sigSStr,16))
        
    if(pubkeytestB.verifies(int(hmMessage.value,16), thresHoldSigLocalCal)):
        print ("SUCESS WITH TS via ECDSA library .. point constructed from public key parameter")
    else:
        print("FAILURE WITH TS via ECDSA library")
        
    
    if(dersigStr is not None):
        orderlen = (1+len("%x" % N))//2 
        print(orderlen)
        print(len(dersigStr))
        print(len(bytes(dersigStr,'utf-8')))
        
        
        rBN = Nakasendo.BigNum(value=sigRStr,mod="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
        sBN = Nakasendo.BigNum(value=sigSStr,mod="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
        derSigInputBN = Nakasendo.BigNum(value=dersigStr,mod="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
        DerFormatSigTest = Nakasendo.createDERFormat(rBN,sBN)
        assert (DerFormatSigTest == DerFormatSigTest)
        
        #ecdsaDerSig = ecdsa.util.sigdecode_string(dersigStr,N)
        #thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(ecdsaDerSig[0],16),int(ecdsaDerSig[1],16))
        #thresHoldSigLocalCal = ecdsa.ecdsa.Signature(ecdsaDerSig[0],ecdsaDerSig[1])
    
        #if(pubkeytestB.verifies(int(hmMessage.value,16), thresHoldSigLocalCal)):
        #else:
        #    print ("SUCESS WITH TS .. point constructed from public key parameter and signature constructed from DER")
        #    print("FAILURE WITH TS - DER test")
        val = Nakasendo.verifyDER(message, pempubkey, dersigStr,False)
        if(val == True):
            print ('TS DER format verified')
            
    #creata an Asmkey .. 
    #use ECDSA to sign
    #use Nakasendo to verify
    
    
    '''
    
    #modulo = Nakasendo.BigNum( value = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
    
    print(val)
    
    
    modulo = Nakasendo.BigNum( value = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
    Hm = HashMsg(message,modulo)
    print(Hm.value)
    #priKey = Nakasendo.BigNum()
    #priKey.mod = modulo.value
    
    #hard-coded polynomials
    #pointa = Nakasendo.BigNum(value='AB0B87ABF066FFB901FA42EBA1C88FA1AEF967814373E489EE60DB7EB3843F66', mod=modulo.value)
    #pointc = Nakasendo.BigNum(value='9EDB9EB292D5EFD9FC71AB2D8CA80B693526DE68A61176D1258A8A841F77BCDD', mod=modulo.value)
    #pointb = Nakasendo.BigNum(value='7A64B803656EB3F17E035BE721F71525D5E761510F6F8E0555CAF555C7980C2E', mod=modulo.value)
    pointa = Nakasendo.BigNum(value='2E30623C2128AAD193B884268459B286973156F806D8536BADA725B16F65D92E', mod=modulo.value)
    pointc = Nakasendo.BigNum(value='193876CFFD9FF5CB1998D1520B778CD09E5B000BEE2CAC28F1AAC14D7487E335', mod=modulo.value)
    pointb = Nakasendo.BigNum(value='69542E811BA839E759E838FDB74128B440A28506EF12B786234BCADC35F856FC', mod=modulo.value)
    
    
    calcprivatekey = pointa + pointb + pointc
    print(calcprivatekey)
    
  
    calpubkey = Nakasendo.MultiplyByGenerator(calcprivatekey,compressed=True)
    #print(calpubkey)
    assert(calpubkey.value == pubKeyStr)
    
    ecdsaPubKey = int(calcprivatekey.value,16)*G
    print ('Public Key for ecdsa verification %s' %calpubkey.value)
    pubkeytest = ecdsa.ecdsa.Public_key(G, ecdsaPubKey) 
    privkeytest = ecdsa.ecdsa.Private_key(pubkeytest,int(calcprivatekey.value,16))
    ecdsa_sig = privkeytest.sign(int(Hm.value,16),int(calcprivatekey.value,16))
    print('%s, %s' % (hex(ecdsa_sig.r), hex(ecdsa_sig.s)))
    
    #hard-coded polynomials
    #kPointa = Nakasendo.BigNum(value='2F881CFC58002291B148F4FEEB08FBF37D6029FC64A31D92DD20205CCB835E66', mod=modulo.value)
    #kPointb = Nakasendo.BigNum(value='A9EDD1D17CE3977487F97DF19F85ABA915E4FED827A12DDD7464EB2227FB282F', mod=modulo.value)
    #kPointc = Nakasendo.BigNum(value='D0A48E448025A5E4114B87DC5DD1513A6FD6E1CBF489DE4842CAA71EE78F2AF4', mod=modulo.value)
    kPointa = Nakasendo.BigNum(value='7C954E1C7E41D67A794428CB0844AF17A60FD5E50B3DAEBE3574ADF52B50BBFD', mod=modulo.value)
    kPointb = Nakasendo.BigNum(value='C06E694DB196E23D5B0641346024DA316A6385C0C6BEDB9E9921EA5C50D314E4', mod=modulo.value)
    kPointc = Nakasendo.BigNum(value='0E44A03589DCED73F37F0D09AE5BF8A83E5DEC3B19AE4B87DAA2D4AB04DC07E9', mod=modulo.value)

    kVal = kPointa + kPointb + kPointc
    inv_kVal = kVal.inverse()
    
    ecdsa_sig = privkeytest.sign(int(Hm.value,16),int(kVal.value,16))
    print('%s, %s' % (hex(ecdsa_sig.r), hex(ecdsa_sig.s)))
    if (pubkeytest.verifies (int(Hm.value,16),ecdsa_sig)):
        print ("SUCCESS WITH ECDSA")
    else:
        print ("ERROR WITH ECDSA")
        
    
    hmMessage = '8C31C0E267AFC3FA1AAF7955960874FCDBE9BEE0F798FD9EF3E7B07E95964752'
    #create an ecdsa public key from the program parameter
    ecdsaPubKeyPoint = ecdsa.ellipticcurve.Point(ecdsa.SECP256k1.generator.curve(), int(coords[0],16), int(coords[1],16))
    pubkeytestB = ecdsa.ecdsa.Public_key(G, ecdsaPubKeyPoint)
    
    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(sigRStr,16),int(sigSStr,16))
    if(pubkeytest.verifies(int(hmMessage,16), thresHoldSigLocalCal)):
        print ("SUCESS WITH TS")
    else:
        print("FAILURE WITH TS - Local Calc")
        
    if(pubkeytestB.verifies(int(hmMessage,16), thresHoldSigLocalCal)):
        print ("SUCESS WITH TS .. point constructed from public key parameter")
    else:
        print("FAILURE WITH TS - Local Calc")
        
        
    signBN = Nakasendo.BigNum(value=sigSStr, mod=modulo.value)
    #DER FORMAT (move this into the Nakasendo
    TWO = Nakasendo.BigNum('2',modulo.value,isDec=False)
    modDivByTwo = modulo / TWO  
    canonizedInteropolated_s = signBN
    
    if ( signBN > modDivByTwo):
        canonizedInteropolated_s = modulo - signBN
        
    print('verify with cannonized version')
    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(sigRStr,16),int(canonizedInteropolated_s.value,16))
    if(pubkeytest.verifies(int(Hm.value,16), thresHoldSigLocalCal)):
        print ("SUCESS WITH TS (cannonized)")
    else:
        print("FAILURE WITH TS - Local Calc")
        
        
    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int("F4A3022B9F8EFD541F1534B80F2E7016A836149A9F06B5A78EACB95C28DFF3A2",16),int("84E179BEEF8FBF256A63C34D579C62706D4782341E60E6A2269A3FA805129FD9",16))
    if(pubkeytest.verifies(int(Hm.value,16), thresHoldSigLocalCal)):
        print ("SUCESS WITH Nakasendo generaterd signature")
    else:
        print("FAILURE WITH Nakasendo generaterd signaturec")
        
        
    print ('Nakasendo test')
    pubkeyVal = "0240160EC2459D7606F9F59DF2A6B15F0F16BE3359D5DE896C5A906B423C9E7728"
    ptPub = Nakasendo.ECPoint()
    ptPub.value = pubkeyVal
    print(ptPub.GetAffineCoOrdinates())
    
    coords = ptPub.GetAffineCoOrdinates()
    
    
    ecdsaPubKeyPoint = ecdsa.ellipticcurve.Point(ecdsa.SECP256k1.generator.curve(), int(coords[0],16), int(coords[1],16))
    pubkeytestNak = ecdsa.ecdsa.Public_key(G, ecdsaPubKeyPoint)
    NakSIg =  ecdsa.ecdsa.Signature(int("D2D8D8930F0D696DE66E30C84C063A7FFC2DCFFD9E598C1BA444A62821959CCD",16),int("8AABFF42B12263A79CEDEE13BE268C8B1BDD59FFDEAE6CC8B24E26E830B476BA",16))
    if(pubkeytestNak.verifies(int(Hm.value,16),NakSIg)):
        print("At least this verified")
        
    print ('The final test')
    ecdsaPubKeyNak = int("70971C51C90FE1A7F87B6391712D3D1C5DC20891991FD150841A2076681FA007",16)*G
    finalTestpubKey =  ecdsa.ecdsa.Public_key(G,ecdsaPubKeyNak)
    if(finalTestpubKey.verifies(int(Hm.value,16),NakSIg)):
        print("At least this verified...finally")
    
    
    modulo = Nakasendo.BigNum( value = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
    #priKey = Nakasendo.BigNum()
    #priKey.mod = modulo.value
    
    pointa = Nakasendo.BigNum(value='6B83B8CAE69B7E31232FE1EEB764F2F6AFF22DE408C1002E0F854C32EDF93887', mod=modulo.value)
    pointb = Nakasendo.BigNum(value='B2ED315539E2C2D6C1B45648F9BF0D1125C9C3EFA6B9E4A6DDD9A94C6E809857', mod=modulo.value)
    pointc = Nakasendo.BigNum(value='38AA72B06DD3100A76F1DAA1FBEAED000976B9C57976A672F321ED6F25727DEA', mod=modulo.value)
    
    
    calcprivatekey = pointa + pointb + pointc
    print(calcprivatekey)
    
  
    calpubkey = Nakasendo.MultiplyByGenerator(calcprivatekey,compressed=True)
    #print(calpubkey)
    assert(calpubkey.value == pubKeyStr)
    
    
    ecdsaPubKey = int(calcprivatekey.value,16)*G
    print ('Public Key for ecdsa verification %s' %calpubkey.value)
    pubkeytest = ecdsa.ecdsa.Public_key(G, ecdsaPubKey) 
    privkeytest = ecdsa.ecdsa.Private_key(pubkeytest,int(calcprivatekey.value,16))

   
    
    Hm = HashMsg(message,modulo)
    #print ('private key %s' % secret.value )

    ecdsa_sig = privkeytest.sign(int(Hm.value,16),int(calcprivatekey.value,16))
    print('%s, %s' % (ecdsa_sig.r, ecdsa_sig.s))
    
    #signature = ecdsa.util.sigencode_der_canonize(ecdsa_sig.r, ecdsa_sig.s,int(modulo_n.value,16))

    if (pubkeytest.verifies (int(Hm.value,16),ecdsa_sig)):
        print ("SUCCESS WITH ECDSA")
    else:
        print ("ERROR WITH ECDSA")
    
        
      
    kPointa = Nakasendo.BigNum(value='89D893081A6D9E429D5CD8C44884A034D5583C20E584FA082CADC026660430E7', mod=modulo.value)
    kPointb = Nakasendo.BigNum(value='970CCD99DF65EC437005ADF4092A0B42A3C5F29A9B00774A592C0CC2934CE77B', mod=modulo.value)
    kPointc = Nakasendo.BigNum(value='C7D85487C7A7DF76B2DC5FC30451F6AAE1A5AF579712998C9D0D590749C0DDA2', mod=modulo.value)
    
    
    
    #ord2
    
    sPointa = Nakasendo.BigNum(value='49AEFB047E628116AA54CED304C97B6F53A1582B923555B914DB08D13ADD3021', mod=modulo.value)
    
    #ord3
    sPointb = Nakasendo.BigNum(value='C18237BED6A3A5B58D165B268F142716CEA95EA1611B35DEFE78E03F62315E1F', mod=modulo.value)
    
    #ord1
    sPointc = Nakasendo.BigNum(value='ADE8E4BD1BCEE6B2AFEF581D5EB7495931737108DDF034BAE523076F3F5AC7BD', mod=modulo.value)
    
    xfx = []
    xfx.append((1,sPointc.value))
    xfx.append((2,sPointa.value))
    xfx.append((3,sPointb.value))
    s_interpolator = Nakasendo.LGInterpolator(xfx, modulo.value,decimal=False)
    interpolated_s = s_interpolator ('0')
    
    #DER FORMAT (move this into the Nakasendo
    TWO = Nakasendo.BigNum('2',modulo.value,isDec=False)
    modDivByTwo = modulo / TWO  
    canonizedInteropolated_s = interpolated_s
    
    if ( interpolated_s > modDivByTwo):
        canonizedInteropolated_s = modulo - interpolated_s
    
    print ('interpolated_s %s', canonizedInteropolated_s)
    
    
    kVal = kPointa + kPointb + kPointc
    inv_kVal = kVal.inverse()
    print(inv_kVal)
    
    kPubKey = Nakasendo.MultiplyByGenerator(kVal,compressed=True)
    kPubkeyPoints = kPubKey.GetAffineCoOrdinates()
    print(kPubkeyPoints[0])
    kPubKeyInv = Nakasendo.MultiplyByGenerator(inv_kVal,compressed=True)
    kPubkeyPointsInv = kPubKeyInv.GetAffineCoOrdinates()
    print(kPubkeyPointsInv[0])
    assert(kPubkeyPointsInv[0]== sigRStr)
    
    ecdsa_sig = privkeytest.sign(int(Hm.value,16),int(inv_kVal.value,16))
    print('%s, %s' % (hex(ecdsa_sig.r), hex(ecdsa_sig.s)))
    if (pubkeytest.verifies (int(Hm.value,16),ecdsa_sig)):
        print ("SUCCESS WITH ECDSA")
    else:
        print ("ERROR WITH ECDSA")
    
    
    thresHoldSig = ecdsa.ecdsa.Signature(int(sigRStr,16),int(sigSStr,16))
    if(pubkeytest.verifies(int(Hm.value,16), thresHoldSig)):
        print ("SUCESS WITH TS")
    else:
        print("FAILURE WITH TS")
        
        
    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(sigRStr,16),int(canonizedInteropolated_s.value,16))
    if(pubkeytest.verifies(int(Hm.value,16), thresHoldSigLocalCal)):
        print ("SUCESS WITH TS - Local Calc")
    else:
        print("FAILURE WITH TS - Local Calc")
    



    #player 3

    #2019-11-13 15:02:07+0000 [-] s = 64C00EE9573DBD58A73BBCD69D7349B40DF59A826C37594F1C80A97A789DD1A9
    empKey = Nakasendo.BigNum(value="F52C9B79DF96EA430C63070118C45DBAA14032B4C0F09965039D742515FDA2AD", mod=modulo.value)
    pks = Nakasendo.BigNum(value="B9B19557C4C9735BC0C73718FB6215631F18A833DC122BE26EC9390B4A784DDC", mod=modulo.value)
    rval = Nakasendo.BigNum(value="91D3FBE905B8C1715A8F8136788C9C3CC2A50007BD821F2DFEC8F7D1E1FBF3E4", mod=modulo.value)
    
    s_3 = empKey * (Hm +(pks*rval))
    print('Value of s_3 = %s',s_3.value)
    #s_3_inv = invEmpKey * (Hm +(pks*rval))
    #print(s_3_inv)
    #player 2

    #2019-11-13 15:02:07+0000 [-] s = 9EC0912A59F2426192A40FC84BB2CF4973D5AAA7D815BED8B4A186A8CCAFDD7A
    empKey= Nakasendo.BigNum(value="465CF95F2AE314D64856FC29D7D874892467E442D9351EFCA38E50DBFF78D289", mod=modulo.value)
    pks = Nakasendo.BigNum(value="437F828007F6BD4349CC2B03E1465D458D576B6680D6E0DC6A24DD4E7CD022AA", mod=modulo.value)
    rval = Nakasendo.BigNum(value="91D3FBE905B8C1715A8F8136788C9C3CC2A50007BD821F2DFEC8F7D1E1FBF3E4", mod=modulo.value)

    s_2 = empKey * (Hm +(pks*rval))
    print( print('Value of s_2 = %s',s_2.value))
    #s_2_inv = invEmpKey * (Hm +(pks*rval))
    #print(s_2_inv)
    
     #player 1
     
    #2019-11-13 15:02:07+0000 [-] s = 690DA9EBBDE980CDABA874698C0B4BE743C98250F4722D5626F25E954DC60C12

    empKey= Nakasendo.BigNum(value="978D5744762F3F69844AF15296EC8B56623E72B7A0C244D003518C1FB92A43A6", mod=modulo.value)
    pks = Nakasendo.BigNum(value="CD4D6FA84B24072AD2D11EEEC72AA526B6450B7FD4E436122552E01E7F5E38B9", mod=modulo.value)
    rval = Nakasendo.BigNum(value="91D3FBE905B8C1715A8F8136788C9C3CC2A50007BD821F2DFEC8F7D1E1FBF3E4", mod=modulo.value)

    s_1 = empKey * (Hm +(pks*rval))
    print('Value of s_1 = %s',s_1.value)
    #s_1_inv = invEmpKey * (Hm +(pks*rval))
    #print(s_1_inv)
    
    
    
    xfx = [(1, s_1.value),(2,s_2.value),(3,s_3.value)]
    #xfx.append((1, s_1),(2,s_2),(3,s_3))
    print(xfx)
    s_interpolator = Nakasendo.LGInterpolator(xfx, modulo.value,decimal=False)
    interpolated_s = s_interpolator ('0')
    
    #DER FORMAT (move this into the Nakasendo
    TWO = Nakasendo.BigNum('2',modulo.value,isDec=False)
    modDivByTwo = modulo / TWO  
    canonizedInteropolated_s = interpolated_s
    
    if ( interpolated_s > modDivByTwo):
        canonizedInteropolated_s = modulo - interpolated_s
    
    print ('interpolated_s_from_copied_data %s', canonizedInteropolated_s)
    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(sigRStr,16),int(canonizedInteropolated_s.value,16))
    if(pubkeytest.verifies(int(Hm.value,16), thresHoldSigLocalCal)):
        print ("SUCESS WITH TS - Local Calc")
    else:
        print("FAILURE WITH TS - Local Calc")
        
    
  
    xfx_inv = [(1, s_1_inv.value),(2,s_2_inv.value),(3,s_3_inv.value)]
    #xfx.append((1, s_1),(2,s_2),(3,s_3))
    print(xfx)
    s_interpolator = Nakasendo.LGInterpolator(xfx_inv, modulo.value,decimal=False)
    interpolated_s = s_interpolator ('0')
    
    #DER FORMAT (move this into the Nakasendo
    TWO = Nakasendo.BigNum('2',modulo.value,isDec=False)
    modDivByTwo = modulo / TWO  
    canonizedInteropolated_s = interpolated_s
    
    if ( interpolated_s > modDivByTwo):
        canonizedInteropolated_s = modulo - interpolated_s
    
    print ('interpolated_s (inv k)_from_copied_data %s', canonizedInteropolated_s)
  
   
    # check the key shares 
    keyShareA = Nakasendo.BigNum(value='025020C3285E783A03799C1BD4B7FEEF0BBBCF49243947C1FB7AA1418E08FBCB', mod=modulo.value)
    keyShareB = Nakasendo.BigNum(value='437F828007F6BD4349CC2B03E1465D458D576B6680D6E0DC6A24DD4E7CD022AA', mod=modulo.value)
    keyShareC = Nakasendo.BigNum(value='B9B19557C4C9735BC0C73718FB6215631F18A833DC122BE26EC9390B4A784DDC', mod=modulo.value)
    
    xfx = [(1,keyShareA.value),(2,keyShareB.value),(3,keyShareC.value)]
    privKeyInterpolator = Nakasendo.LGInterpolator(xfx, modulo.value,decimal=False)
    calculatedPriKey = privKeyInterpolator('0')
    #print(calculatedPriKey)
    assert (calculatedPriKey == calcprivatekey)
    
    
    #check the ephermal key shares
        # check the key shares 
    empShareA = Nakasendo.BigNum(value='CD4D6FA84B24072AD2D11EEEC72AA526B6450B7FD4E436122552E01E7F5E38B9', mod=modulo.value)
    empShareB = Nakasendo.BigNum(value='465CF95F2AE314D64856FC29D7D874892467E442D9351EFCA38E50DBFF78D289', mod=modulo.value)
    empShareC = Nakasendo.BigNum(value='F52C9B79DF96EA430C63070118C45DBAA14032B4C0F09965039D742515FDA2AD', mod=modulo.value)
    
    xfx = [(1,empShareA.value),(2,empShareB.value),(3,empShareC.value)]
    empKeyInterpolator = Nakasendo.LGInterpolator(xfx, modulo.value,decimal=False)
    calculatedempKey = empKeyInterpolator('0')
    #print(calculatedempKey)
    assert (calculatedempKey == kVal)
    
    '''
    
    
