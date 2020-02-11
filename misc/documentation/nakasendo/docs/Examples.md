## Examples in C++
**BigNumber**
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

TODO : pick examaples in examples/cpp, quote here and explain them

## Examples in Python

**Key split and recover**
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

TODO : pick examaples in examples/python, quote here and explain them

## Examples in Javascript
**Random big number generator**
```javascript
    const rand_hex_str = JSBigNumbers.RandHex(128);
```

TODO : pick examaples in NapiBinding/test, quote here and explain them