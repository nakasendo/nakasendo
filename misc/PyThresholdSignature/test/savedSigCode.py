if __name__ == "__main__":
    print("....starting.....")
    modulo = Nakasendo.BigNum()
    #modulo.value = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141'
    modulo.value = '17'
    
    #tt = random.randint(1, 4)
    #margin = random.randint(0,5)
    #nn = 2 * tt + 1 + margin
    tt = 3;
    nn = 4;
    msg='hello, I am a string, sign me now'
    #__test_ThresholdSignature(msg, 1, 3, q)
    #__test_ThresholdSignature(msg, tt,nn,q)
    
    start_time = time.time()
    
    Players = []
    for i in range (1,nn):
        label = "p" + str(i)
        new_player = PyPlayer.Player(label, i, False, False)
        new_player.CreatePolynomial(nn, str(modulo));
        Players.append(new_player)
   
   
   
    for i in range(len(Players)):
        print(Players[i].to_json()) 
        print(Players[i].polynomial)
        
    
    #create key share
    keyShares = CalculateSecret(Players)
    assert len(keyShares) == len(Players)
    
    print ("The Calculated key shares are")
    print (keyShares)
    #Create a public key
    SharedPubKey = DerivePubKey(Players)
    points = SharedPubKey.GetAffineCoOrdinates()
    print('\nThe Shared Public key is %s,%s\n' % (points[0], points[1]))
    #create empheral keys
    # reset the polys
    
    
    exit(0)
    for i in range(len(Players)):
        new_player.CreatePolynomial(nn, str(modulo));
    
    empKeys = CalculateSecret(Players)
    print ('empheral keys')
    print(empKeys)
    #create blinding values
    # reset the polys
    for i in range(len(Players)):
        new_player.CreatePolynomial(nn, str(modulo));
    blindingVals = CalculateSecret(Players)
    
    print ('alphas')
    print (blindingVals)
    
    
    #note the index of each of the arrays coresponds to the players
    
    
    #create v = K(alpha)
    v = __calc_mul_vector(empKeys,blindingVals)
    print ("v*alpha")
    print(v)
    
    
    #create w = alpha * G
    W = []
    for i in range(len(blindingVals)):
        w = Nakasendo.MultiplyByGenerator(blindingVals[i])
        W.append(w)
    
    print(W)
    
    
    
    #interpolate v = (v1,v2)
    
    #create xfx with player labels & v 
    assert (len(v) == len(Players))
    xfx = []
    for i in range(len(v)):
        xfx.append((Players[i].getOrdinal(), v[i].value))
    
    BigNumInterp = Nakasendo.LGInterpolator(xfx, modulo.value,False)
    zeroPoint = Nakasendo.BigNum('0',modulo)
    interpolated_v = BigNumInterp(zeroPoint.value)
    print(interpolated_v)
    
    #create xfx with player labels & w
    assert(len(W) == len(Players))
    xfx_cp = []
    
    for i in range(len(v)):
        points = W[i].GetAffineCoOrdinates()
        xfx_cp.append((Players[i].getOrdinal(), points[0],points[1]))
        
    ECPointInterp = Nakasendo.LGECInterpolator(xfx_cp, modulo.value)
    interpolated_w = ECPointInterp('0')
    print(interpolated_w)
    
    
    invV = interpolated_v.inverse();
    rKey = interpolated_w.multipleScalar(invV)
    
    print(rKey)
    
    
    print ('Hash a message')
    
    message = 'The quick brown fox jumped over the lazy dog'
    Hm = Nakasendo.hash256(message)
    print(Hm)
    Hm.value = '7D38B5CD25A2BAF85AD3BB5B9311383E671A8A142EB302B324D4A5FBA8748C69'
    print(Hm)
    #Hm, keyshares, empKeys, interpolated_r
    
    assert(len(keyShares) == len(empKeys))
    sigShares = CalculateSignatureShares(Hm, keyShares, empKeys,rKey)
    
    print(sigShares)
    assert (len(sigShares) == len(Players))
    xfx_sig = []
    for i in range(len(Players)):
        xfx_sig.append((Players[i].getOrdinal(), sigShares[i].value))
    
    print(xfx_sig)
    #interpolate
    
    BigNumInterp = Nakasendo.LGInterpolator(xfx_sig,modulo.value,False)
    sig = BigNumInterp('0')
    
    print ('Signature should be (%s, %s)' % (rKey, sig))
    
    points = SharedPubKey.GetAffineCoOrdinates()
    print('\nThe Shared Public key is %s,%s\n' % (points[0], points[1]))
    
 '''
    
    #to use this the puiblic key needs to be in PEM form! (BUMHOLES)
    #verifyIt = Nakasendo.verify(msg, SharedPubKey.value, rKey.value, sig.value)
    #if (verifyIt == True):
    #    print ('msg verified')
        
    #set up the polynomials:
    #Polynoms = []
    #labels = []
   # for i in range(len(Players)):
  #      Polynoms.append(Players[i].polynomial)
  #      labels.append(Players[i].getOrdinal())
        
  #  matrix_d = __calc_matrix(Polynoms, labels)
    
  #  print(matrix_d)
    
 #   vect_d = __calc_matrix_sum_col(matrix_d, modulo.value)
 #   for i in range(len(vect_d)):
 #       print(vect_d[i])
 #   
    
 #   print ('Calculating Shared Secret...')
 #   sharedSec = __calc_shared_secret(Polynoms,modulo.value)
 #   print (sharedSec)
    
    
    #calculate the individual pieces of a shared secret
    
    #player 
    
    #a quick mess with inversion & ECPoint
    
    #invK = sharedSec.inverse()

    #print (invK)
    
    #kG = Nakasendo.MultiplyByGenerator(invK)
    #print(kG.x())
    
    #order = kG.GetOrder()
    #print(order)
    #deg = kG.GetDegree()
    #print(deg)
    
    
    
    #kG = k*G
    #kGx = kG.x()
    
    #print(new_player.polynomial);
    
    #data=new_player.to_json()
    #p  = PyPlayer.Player()
    #print(data)
    #p.from_json(data)
    #print(p)
