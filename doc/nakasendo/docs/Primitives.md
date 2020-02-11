## BigNumbers
**Description**

BigNumbers are integers of arbitrary size, that require more than 4 bytes to store in memory. Our BigNumbers interface performs operations on numbers of arbitrary precision in decimal and hex-decimal number, including addition, subtraction, multiplication, division, modulo, inverse modulo and rightshift and leftshift bitwise operations.

**Code Examples**

BigNumber operators in JavaScript:
```javascript
    var modN = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
    var bnA = new BigNumber(false, modN);
    var bnB = new BigNumber(false, modN);
    var addres = AddBigNumber(bnA, bnB);
    var subres = SubBigNumber(bnA,bnB);
    var mulres = MulBigNumber(bnA,bnB);
    var divres = DivBigNumber(bnA,bnB);
```

BigNumber operators in C++:
```c++
    BigNumber val;

    std::string SumA("");
    std::string AVal = "c590e57ee64fced18aff6e2f0c6ac05625b1e94f394f42470cae14d12cadea4f5ab6b9d77225fe3b4903825966c78752ae51b6a0a2caca555fd0ffcbd9704b01";
    std::string BVal("219639ed8afc21e052422fff0ae5583231ebca2999404b099628093e6540b1dbc20b9c495aa7229b5965b19a5fcd653b3fa0eccab567c5b5e8");

    BigNumber BNValA, BNValB;
    BNValA.FromHex(AVal);
    BNValB.FromHex(BVal);


    std::cout << BNValA.ToHex() << "\n" << BNValB.ToHex() << std::endl;

    BigNumber Sum = BNValA + BNValB;
    std::cout << Sum.ToHex() << std::endl;
```

BigNumber operators in Python:
```python
    # Add 2 big numbers of arbitrary precision in decimal
    for x in collectionOfBigNumbersInDecimal:
        decNumber = x.split()
        actual_Value = PyBigNumbers.addFromDec(decNumber[0], decNumber[1])
        # Verify the actual value with expected value
        assert actual_Value == decNumber[2], "Test failed"
        
    # Subtract 2 big numbers of arbitrary precision in hex
    for x in collectionOfBigNumbersInHex:
        hexNumber = x.split()
        actual_Value = PyBigNumbers.subFromHex(hexNumber[0], hexNumber[1])
        # Verify the actual value with expected value
        assert actual_Value.upper() == hexNumber[2].upper(), "Test failed"
```
___

## AsymKey
**Description**

Asymmetric keys form the foundations of Public Key Infrastructure (PKI). They are a cryptographic scheme requiring two different keys, one to encrypt the text, and one to decrypt.

**Code Examples**

Splitting a key in JavaScript:
```javascript
    var myKey = new AsymKey();
    console.log(myKey.toString());

    var sharesOfKey = myKey.SplitKey(3,6);
    console.log(sharesOfKey);

    var recoveredKey = new AsymKey();
    recoveredKey.RecoverKey(sharesOfKey);
    console.log(recoveredKey.toString());
```

Splitting a key in C++:
```c++
    AsymKey randomKey;
    int t=5;
    int k=7;
    std::vector<KeyShare> shares = randomKey.split(t,k); 
```

Splitting a key in Python:
```python
    for x in range(10):
        # Randomly generate two digit number that ranges from 3 to 100
        threshold = randint(3, 100)

        # Generate pair of keys in pem format
        pubKey, privKey = PyAsymKey.GenerateKeyPairPEM()
        assert privKeyPEMHeader in privKey, "Test failed"

        # Split a private key into a given number of shares
        splitKeyList = PyAsymKey.SplitKey(privKey, threshold, maxshares)
        assert len(splitKeyList) == maxshares, "Test failed"
```

___

## BSVAddress
**Description**

Most Bitcoin addresses are 34 characters. They consist of random digits and uppercase and lowercase letters, with the exception that the uppercase letter "O", uppercase letter "I", lowercase letter "l", and the number "0" are never used - to prevent visual ambiguity.

**Code Examples**

Creating BSVAddresses in JavaScript:
```javascript
    //derive a public key from a msg (WP-42 example)
    var derivedPubK = recoveredKey.derivePublicKey(msg);
    console.log(derivedPubK);

    var derivedPubKasHex = pubKeyAsHexPt(derivedPubK,true);
    console.log(derivedPubKasHex);
    
    var bsvaddr = new BSVAddress(derivedPubKasHex, 0);
    if(bsvaddr.valid)
        console.log(bsvaddr.address);
```

Creating BSVAddresses in C++:
```c++
    BSVAddress addressOne 
        ( 
            key.exportPublicHEXStr( ), 
            VersionPrefix::PUBLIC_KEY_ADDRESS  
        ) ;

    BSVAddress addressTwo 
        ( 
            "023cba1f4d12d1ce0bced724612769b2262c6daa97be6a0588cfec8ce1a5f0bd09", 
            VersionPrefix::TESTNET_PUBLIC_KEY_ADDRESS 
        ) ;
```

Creating BSVAddresses in Python:
```python
    key = "023cba1f4d12d1ce0bced725373769b2225ba6daa97be6a0588cfec8ce1a5f0bd09"
    version = 0
    address = Nakasendo.BSVAddress(key, version )
    
    print( "BSVAddress from key %s is %s\n" % (key, address.address ) )
    print("Address information\n%s" % address )
```

___

## ECPoint
**Description**

ECPoints represent points on an elliptic curve. Below, please find details of Elliptic Curves, how they are used in Bitcoin via Digital Signatures and finally a method of exchanging them between 2 parties.

**Code Examples**

Create an ECPoint in JavaScript:
```javascript
    ar ecptA = new ECPoint();
    var ecptB = new ECPoint();

    console.log(ecptA.toString());
    console.log(ecptB.toString());

    var sumPts = AddECPoint(ecptA, ecptB);
    console.log(sumPts.toString());
```

Create an ECPoint in C++:
```c++
    namespace PY_ECPOINT
    {
        // make an ECPoint according to curveID
        std::unique_ptr< ECPoint > makeECPoint( int curveID )
        {
            std::unique_ptr< ECPoint > pointPtr ;
            if ( curveID == 0 ) 
            {
                // use defualt constructor
                pointPtr =  std::unique_ptr< ECPoint> ( new ECPoint( ) ) ;
            }
            else
            {
                pointPtr =  std::unique_ptr< ECPoint> ( new ECPoint( curveID ) ) ; 
            }
            return pointPtr ;
        }
    }
```

Create an ECPoint in Python:
```python
    # Generate Random EC Points
    for x in range(100):
        # Generate a Random EC Point with default NID ==> NID_secp256k1
        actual_value = PyECPoint.GenerateRandomEC( 0, hex, True )

        # Verify the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"
```

**Definition**

_Elliptic Curves_

In Bitcoin the calculation of private and public keys is based on elliptic curves groups. An elliptic curve $E$ over the field $\mathbb{Z}_p$ satisfies the equation

\begin{equation}
y^2 = x^3 + ax + b \bmod p
\end{equation}

where $a, b \in \mathbb{Z}_p$, satisfying $4a^3 + 27b^2 \neq 0$. The group over this elliptic curve is defined to be $E(\mathbb{Z}_p) := E \cup \{ {\mathcal{O}} \}$ where $\{ {\mathcal{O}} \}$ is the identity element called the point at infinity, and the elements of $E$ are of the form $(x,y)$ satisfying the equation above. The group operation is point addition $'+'$ and the group has order $n$. 

For a point $G\in E(\mathbb{Z}_p)$ and a scalar $d \in \mathbb{Z}_n$, point scalar multiplicaion denoted $d\cdot G$ is defined to be the point $G$ added to itself $d$ times. In Bitcoin, private keys are chosen to be this value $d$, and the public keys are then $d\cdot G$. The elliptic curve is chosen to be the secp256k1 curve, which has the following parameters in hexadecimal:

$$a=00$$

$$b=07$$

$$p=\mathrm{FFFFFFFF \; FFFFFFFF \; FFFFFFFF \; FFFFFFFF \; FFFFFFFF \; FFFFFFFF \; FFFFFFFE \; FFFFFC2F} $$

$$n=\mathrm{FFFFFFFF \; FFFFFFFF \; FFFFFFFF \; FFFFFFFE \; BAAEDCE6 \; AF48A03B \; BFD25E8C \; D0364141}$$

$$G=\mathrm{ (79BE667E \; F9DCBBAC \; 55A06295 \; CE870B07 \; 029BFCDB \; 2DCE28D9 \; 59F2815B \; 16F81798, \\ \; 483ADA77 \; 26A3C465 \; 5DA4FBFC \; 0E1108A8 \; FD17B448 \; A6855419 \; 9C47D08F \; FB10D4B8) }$$

Note that both $p$ and $n$ are primes. Given a public key $d\cdot G$ with these parameters, it is computationally infeasible to calculate the private key $d$, which is one of the reasons why Bitcoin is secure. A private key can be used to create a signature on a message, that can be verified against the corresponding public key. Signatures in Bitcoin are calculated with the Elliptic Curve Digital Signature Algorithm.

_Elliptic Curve Digital Signature Algorithm_

In order to create a signature on a message $msg$, with the private key $d$, the following steps are taken. 

1. Calculate the message digest $e = \mathrm{SHA256}\left(\mathrm{SHA256}(msg)\right)$, where $\mathrm{SHA256}(\bullet)$ is the SHA256 hash function. Note that in the usual ECDSA, the message is only hashed once. However, the algorithm used in Bitcoin hashes the message twice, as we calculate here.  
2. Chose a random integer $k \in \{1,\ldots, n-1\}$, where $n$ is the order of the secp256k1 curve. We will refer to $k$ as the ephemeral key.
3. Calculate the ephemeral public key corresponding to this ephemeral private key 
$$ k \cdot G = (R_x,R_y) \; . $$
4. Calculate $r=R_x \bmod n$. If $r=0$, return to step 2.
5. Calculate the multiplicative inverse of the ephemeral key $k^{-1} \bmod $n$.
6. Calculate $s=k^{-1}(e+dr) \bmod n$. If $s=0$, return to step 2.
7. The signature on the message $msg$ is $(r,s)$.

The ephemeral key must be kept secret, otherwise the private key can be calculated, given a message and signature. Additionally, each time a signature is generated, a different ephemeral key must be used. If this is not the case, it is possible to derive the private key $d$ given two different signatures and their corresponding messages.

Given a message $msg$, a public key $d\cdot G$, and corresponding signature $(r,s)$, then one can verify the signature by completing the following steps.

1. Calculate the message digest $e=\mathrm{SHA256}(\mathrm{SHA256}(msg))$.
2. Calculate the multiplicative inverse $s^{-1}$ of $s$ modulo $n$.
3. Calculate $j_1=es^{-1} \bmod n$ and $j_2=rs^{-1} \bmod n$.
4. Calculate the point $Q=j_1\cdot G+j_2 (d\cdot G)$.
5. If $Q=\mathcal{O}$, the point at infinity, the signature is invalid. 
6. If $Q\neq \mathcal{O}$, then let $Q:=(Q_x,Q_y)$, and calculate $u=Q_x \bmod n$. If $u=r$, the signature is valid.

It is possible to split the private key $d$ into key shares, that can be distributed among parties, and then set a threshold number of parties who can come together to sign a message in the way described above. This process of splitting a private key, and calculating a signature uses concepts of polynomials. 

_Elliptic Curve Diffie-Hellman key exchange_

This is a method for two entities to create a common secret across an unsecure network, by calculating the secret independently at each entity. 

Assume that Alice and Bob want to share a secret across an unsecure communication channel. Alice and Bob agree on an elliptic curve scheme. The setup is as follows.

1. They each randomly generate their own private key:
	
	i. Alice generates a private key $d_A \in_R \{1,\ldots, n-1\}$ and calculates her corresponding public key $D_A = d_A\cdot G$. Recall that $n$ is the order of the elliptic curve group. 
	
	ii. Bob generates a private key $D_B = d_B \in_R \{1,\ldots, n-1\}$, and calculates his corresponding public key $d_B\cdot G$.
	
2. They share their public keys with each other. 

3. They each use their own private key and the other's public to calculate a common secret. 
	
	i. Alice calculates the common secret using $D_{AB}= d_A D_B = d_A(d_B\cdot G)$.
	
	ii. Bob calculates the common secret using $D_{AB}= d_B D_A = d_B(d_A\cdot G)$.
	
	
They now both know the value $D_{AB}$ and no one else can calculate this, hence it is a common secret. This method can be shown in the diagram below.

Note that the Diffie-Hellman protocol that is used in practice contains some steps where each participant provides a certicate of a public key proving their identity, and signs a message with this certified public key, preventing man-in-the-middle attacks. Additionally, the common secret is changed for each communication round so that if a common secret is compromised, the other communication rounds are still encrypted. This method is called ephemeral Diffie-Hellman key exchange.

___

## MessageHash
**Description**

Cryptographic hash functions are mathematical operations run on digital data. By comparing the computed "hash" (the output from execution of the algorithm) to a known and expected hash value, the data's integrity can be assured. A one-way hash can be generated from any piece of data, but the data cannot be generated from the hash.

**Code Examples**

Call the default SHA256 message hash function in JavaScript:
```javascript
    var valToHash = 'The brown fox jumped over the lazy dog';
    console.log ('Hashing value %s', valToHash);
    console.log('SHA256 %s', nakesendoBindings.SHA256(valToHash));
```

Call the default SHA256 message hash function in C++:
```c++
    MessageHash hashMessage; 
    hashMessage.HashSha256 ("message to hash");
    std::cout << "Calling printable hash" << std::endl ; 
    hashMessage.printHash () ; 
    std::cout << std::endl ;
    std::cout << hashMessage.HashHex () << std::endl ; 
```

Call the default SHA256 message hash function in Python:
```python
    for x in msgHashFile_txt.readlines():
        msg_Hash_value = x.split(",")
        # Generate SHA256 hash on a given input
        actual_Value = PyMessageHash.HashMsgSHA256(msg_Hash_value[0])

        # Verify the actual value with expected value
        assert actual_Value == msg_Hash_value[1].rstrip("\n"), "Test failed"
        assert len(actual_Value) == 64, "Test Failed"
```

___

## Polynomial
**Description**

Polynomial means _many terms_. Polynomials can have constants, variables and exponents, but never division by a variable.

**Code Examples**

Create a Polynomial in JavaScript:
```javascript
    var mod = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141'
    var myPoly = new polynomial(4,mod);
    console.log(myPoly.toString());
```

Create a Polynomial in C++:
```c++
    Polynomial poly( 3, GenerateZero( ), min, max ) ;
```

Create a Polynomial in Python:
```python
    # evaluate polynomial for x
    polynomialFX = PyPolynomial.evaluate(coefficients, xValue, mod, decHex)
    assert type(polynomialFX) == str, "Test failed"
```

**Definition**

A polynomial of order $t$ has the form 

\begin{equation}
f(x) = a_0 + a_1x + \ldots + a_{t-1}x^{t-1} + a_t x^t \bmod n 
\end{equation}

where $a_0, a_1, \ldots, a_t \in \mathbb{Z}_n$, and points on this polynomial will be denoted by 

\begin{equation}
(x_i, y_i) 
\end{equation}

where $y_i = f(x_i)$. Given $t+1$ of these points, there is a unique polynomial of order $t$ that passes through these points, which is necessarily the $f(x)$ given above. In the following, we will assume that all arithmetic operations on integers are done modulo $n$ unless otherwise stated. 

The zeroth order of the polynomial will correspond to a shared secret $d$ in the cases below. Each participant's secret share is a point on this polynomial $(x_i,y_i)$ where $i=1,\ldots ,M$, and $M$ is the number of participants.
It is possible to use $(t+1)$ of these points to recalculate the polynomial. This is done using Lagrange interpolation.

**Lagrange Interpolation**

In order to calculate an order-$t$ polynomial, given at least $(t+1)$ points labelled

\begin{equation}
(x_1, y_1), \ldots, (x_{t+1}, y_{t+1}) 
\end{equation}

on the polynomial $f(x)$, one can use the following equation for interpolation

\begin{equation}
f(x) = \sum_{i=1}^{t+1} y_i \prod_{\substack{ 1\leq j \leq (t+1), \\ j \neq i}}(x-x_j)(x_i-x_j)^{-1} \; .
\end{equation}

If this expression is evaluated at 0, i.e. $f(0)$ is calculated, then the result of this will be exactly the zeroth order of the polynomial $a_0$. As mentioned, this zeroth order coefficient will be the shared secret. Additionally, the $x$-coordinates $x_i$ are chosen to be defined as $x_i=i$. Therefore, the shared secret can immediately be calculated using the equation

\begin{equation}
interpolate(y_1,\ldots, y_{t+1}) := \sum_{i=1}^{t+1} y_i \prod_{\substack{ 1\leq j \leq (t+1), \\ j \neq i}}(-j)(i-j)^{-1} \; .
\end{equation}

where $interpolate(y_1,\ldots, y_{t+1})$ denotes the interpolation at 0 with the $x$-coordinate being given by the index on the $y$-coordinate. Note that if the interpolation is over less than $(t+1)$ points, the polynomial and therefore shared secret $a_0$ cannot be calculated. 

___

## SecretSplit
**Description**

Secret key sharing may refer to the concept of splitting a private key into multiple shares which can be stored independently such that a single entity doesn't know the full key, or it may refer to the process of two parties establishing a secret in a secure way so both parties know a common secret key. Each of these methods are used in this implementation and so are described below. 

**Code Examples**

Split a secret in JavaScript:
```javascript
    var myKey = new AsymKey();
    console.log(myKey.toString());

    var sharesOfKey = myKey.SplitKey(3,6);
    console.log(sharesOfKey);
```

Split a secret in C++:
```c++
    Polynomial poly(degree, mod, theSecret);
    
    std::vector<KeyShare> shares = make_shared_secret (poly, threshold, maxshares);
    for (int i=0; i < numberShareCombinations; ++i){
        std::vector<KeyShare> shareSample;
        std::set<int> chosenNums; 
        
        while (shareSample.size () < threshold ){
            int index = rand() % (shares.size()-1) ; 
            if ( chosenNums.find(index) == chosenNums.end()){
                chosenNums.insert(index);
                shareSample.push_back(shares.at(index)); 
            }
        }
        
        // try to recover the secret
        
        BigNumber recovered_secret; 
        recovered_secret.Zero(); 
        recovered_secret = RecoverSecret(shareSample, mod); 
        
        BOOST_TEST (recovered_secret.ToHex () == theSecret.ToHex()); 
        
    }
```

Split a secret in Python:
```python
    mykey = Nakasendo.ECKey256K1();
    print (mykey)

    shares = mykey.SplitKey(5,15);

    print(shares)

    #At some point later....
    print ("a random collection of shares")

    secure_random = random.SystemRandom()

    subset = []
    sharesCopy = shares
    random.shuffle (sharesCopy)
    for i in range(5):
        subset.append(sharesCopy[i])

    print(subset)

    recoveredKey = Nakasendo.ECKey256K1()
    recoveredKey.RecoverKey(subset)
    
    print(recoveredKey)
```

**Definition**

The first is achieved using Joint Verifiable Random Secret Sharing, and the second is Diffie-Hellman key exchange. 

_Joint Verifiable Random Secret Sharing_

A group of $N$ participants can create a shared secret using the concepts described above, such that $(t+1)$ of the group are required to calculate the shared secret. The shares of this shared secret can be calculated without ever calculating the shared secret itself, and this can be doing in a verifiable way. This method is called 'Joint Verifiable Random Secret Sharing', commonly referred to as JVRSS.

This method JVRSS is split into two parts: creating the shares of the shared secret and verifying the shares of the shared secret.

_Creating the shares_

In order to create shared secret $d$ between $N$ participants, without any single party knowing the secret,  and where $(t+1)$ of the participants are required to calculate the secret, the following steps are taken. Note that each participant has a unique label $i=1,\ldots N$ that they all agree on. 

1. Each participant $i$ randomly generates $(t+1)$ integers $d_{i0},\ldots, d_{it} \in_R \mathbb{Z}_n$, where the notation $\in_R$ means randomly generated elements of $\mathbb{Z}_n$. These integers are used to define a private polynomial for each participant $i$ 
$$ f_i(x) = d_{i0}+ d_{i1}x+\ldots + d_{it} x^t $$
These $N$ polynomials will be used to create shares on a shared polynomial which itself is never calculated explicitly, and in turn will define a shared secret, which similarly is never calculated explicitly. Note that since the aim is to establish a shared private key which is in the group $\mathbb{Z}_n$ where $n$ is the order of the elliptic curve group, the polynomials are calculated modulo $n$. 
2. Each participant $i$ sends the value $f_i(j)$ to participant $j$ only using a secure communication channel with participant $j$. It is crucial that this is shared only betweeen $i$ and $j$ or there may be enough information shared between participants for any of them to calculate the shared secret, contradicting the aim of the JVRSS process. 
3. Each participant $i$ calculates their own secret share of the shared secret by calculating 
$$ d_i = \sum_{j=1}^N f_j(i) \; .$$

By choosing the secret shares to be defined in this way, the shared secret $d$ is essentially
$$ d := \sum_{i=1}^N d_{i0}  \; , $$
although this will never be calculated itself. 
These $d_i$ calculated in the final step along with a participants label $i$ are the secret shares, that is, a participant $i$ has a secret share $d_i$, and $(t+1)$ of these shares can be used to calculate the shared secret.
The formula for Langrange interpolation to calculate the shared secret $d$ using the secret shares $(i,d_i)$ for $i=1,\ldots, (t+1)$ is
\begin{equation}
interpolate(d_1,\ldots d_{t+1}) = \sum_{i=1}^{t+1} d_i \prod_{\substack{ 1\leq j \leq (t+1), \\ j \neq i}}(-j)(i-j)^{-1} \; .
\end{equation}
As mentioned, this interpolation will never be calculated, and is given for completeness. 

_Verifying the shares_

The participants can now verify that the other participants have sent the correct point on their own private polynomials by following the steps below. 

1. Each participant $i$ broadcasts the following information 
$$ d_{ik}\cdot G $$
for $k=0,\ldots t$. Here 'broadcast' means that each participant knows that all other participants received the same information.
2. Each participant $i$ can check that each participant $j$ has calculated this polynomial point $f_j(i)$ from the coefficients $d_{j0}, \ldots, d_{jt}$ using the values $$ d_{jk}\cdot G $$ that are shared in step 1 of the verification. This is done by participant $i$ calculating 
$$ f_j(i) \cdot G \stackrel{?}{=} \sum_{k=0}^t i^k(d_{jk}\cdot G) $$
for $j=1,\ldots, N$, where $f_j(i) \cdot G$ is calculated from the value shared in the second step of the setup. If these checks hold, then participant $i$ knows that participant $j$ has shared the correct private polynomial point. If all participants find this equation holds, then the group collectively knows that they share the same secret. 

_Calculating the shared public key_

Finally, all pariticpants can calculate the shared public key. Each participant calculates 

$$ D := d\cdot G = \sum_{i=0}^{N} (d_{i0}\cdot G) $$

where the right-hand side can be calculated from the values $(d_{i0}\cdot G)$ shared in step 1 of the verification, and the public key is now calculated without ever knowing $d$ explicitly. 

In the following, the calculation and verification of a secret share $d_i$ for a particpant labelled by $i$ will be referred to as $d_i=JVRSS(i)$.

We saw that in step 2, each pair of participants needs to establish a secure communication channel to share the points on the private polynomials. This requires a method for each pair to set up a common secret, which can be used to encrypt the points on the private polynomials, and then they can be shared with the other party securely. This is the second interpretation of the phrase 'secret key sharing', and one method for this which has been implemented is Elliptic Curve Diffie-Hellman key exchange.

___

## SymEncDec
**Description**

Symmetric encryption refers to a type of encryption where the same secret key is used to encrypt and decrypt a message. Therefore, if two parties can create a shared secret such as using Diffie-Hellman described above, they can use symmetric encryption and share ciphertexts over unsecure communication channels securely. Some common symmetric encryption schemes include the Data Encryption Standard (DES), Rivest Cipher 4 (RC4), or Advanced Encryption Standard (AES). In practice, DES and RC4 are no longer used.  

The most secure symmetric encryption standard is the authenticated encrpytion with AES 256-bit and HMAC and been chosen as the standard in the SDK software for this reason. This encryption includes the HMAC of the plaintext message in the encryption of the plaintext, such that after decryption, the reciever can verify that the message was not compromised in transmission.

**Code Examples**

Encrypt and Decrypt a message in JavaScript:
```javascript
```

Encrypt and Decrypt a message in C++:
```c++
    const std::string msg_text{ "I am a message to encrypt" };
    const std::string secret_key{ "MyKey" };
    const std::string iv{ "IV" };
    std::unique_ptr<unsigned char[]> myPass (new unsigned char [secret_key.length() + 1 ]);
    std::fill_n(myPass.get(), secret_key.length()+1, 0x00);
    int index(0);
    for(std::string::const_iterator iter = secret_key.begin(); iter != secret_key.end(); ++ iter, ++index){
        myPass.get()[index]=*iter;
    }

    std::unique_ptr<unsigned char[]> mySalt (new unsigned char [iv.length() + 1 ]);
    std::fill_n(mySalt.get(), iv.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = iv.begin(); iter != iv.end(); ++ iter, ++index){
        mySalt.get()[index]=*iter;
    }
        
    uint64_t keylen(32);
    int iterCount(10000);
    std::unique_ptr<unsigned char[]> mykey = KeyGen(myPass,secret_key.length(),mySalt, iv.length(), iterCount,keylen);
    std::cout << "Set the paramerters:" << std::endl; 
    SymEncDec symencdec;

    
    symencdec.SetParams(mykey,mySalt, 32, 16); 

    std::cout << "Calling aes_encrypt..." << std::endl; 
    std::unique_ptr<unsigned char> encodedData = symencdec.aes_encrypt(sharedSecret);
    std::unique_ptr<unsigned char[]> encodedData;
    int encodedLen = symencdec.aes_encrypt(msg_text, encodedData);
    std::cout << "Encoded length: " << encodedLen << std::endl; 

    std::string outputData;
    int decodedLen = symencdec.aes_decrypt(encodedData,encodedLen, outputData);


    std::cout << "Encoded: " << outputData << "\n" << "Decoded: " << decode << std::endl; 
```

Encrypt and Decrypt a message in Python:
```python
    nonce_value = "c280f8553e93485876e423b1d3f2e3a7"

    # Generate a key from random string of fixed length
    genKey = PySymEncDec.GenerateKey("ghmgpipbmz", nonce_value)
    assert len(genKey) == 64, "Test failed"

    # Read test data from a txt file
    for x in strEncode_txt:
        strEncode = x.split(",")
        encode_value = PySymEncDec.encodeAES(strEncode[0], genKey, nonce_value)
        actual_value = PySymEncDec.decodeAES(encode_value, genKey, nonce_value)
        assert actual_value == strEncode[0], "Test failed"
```
___

