#ifndef __BIGNUMBERS_IMPL_H__
#define __BIGNUMBERS_IMPL_H__

#include <memory>
#include <iostream>
#include <string>
#include <openssl/bn.h>
#include <openssl/objects.h>

using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)> ; 
using CTX_ptr = std::unique_ptr<BN_CTX, decltype(&::BN_CTX_free)>;

class BigNumberImpl
{
    friend std::unique_ptr<BigNumberImpl> Add (const BigNumberImpl*  obj1, const BigNumberImpl*  obj2);  
    friend std::unique_ptr<BigNumberImpl> Sub (const BigNumberImpl* obj1, const BigNumberImpl* obj2);
    friend std::unique_ptr<BigNumberImpl> Mod (const BigNumberImpl* obj1, const BigNumberImpl* obj2); 

    friend bool CMPGreater (const BigNumberImpl* obj1, const BigNumberImpl* obj2);
    friend bool CMPLess (const BigNumberImpl* obj1, const BigNumberImpl* obj2);
    friend bool CMPEqual ( const BigNumberImpl* obj1, const BigNumberImpl* obj2);

    public:
        BigNumberImpl() : m_bn(BN_new(), ::BN_free) 
        { 
            BN_ptr bn(BN_new(), ::BN_free);
        }


        ~BigNumberImpl() = default;

        //
        BigNumberImpl (const BigNumberImpl& obj) : m_bn(BN_new(), ::BN_free)
        { BN_copy (m_bn.get(), obj.m_bn.get());}

        BigNumberImpl& operator= (const BigNumberImpl& obj)
        { 
            if (this != &obj){
                BN_copy(m_bn.get(), obj.m_bn.get()); 
            }
            return *this;
        }


        void One(); 
        void Zero(); 
        void Negative() ; 

        BigNumberImpl operator++(int);
        BigNumberImpl& operator++();
        BigNumberImpl operator--(int);
        BigNumberImpl& operator--();

        std::string ToHex () const ; 
        std::string ToDec () const ; 

        int FromHex (const std::string& );
        int FromDec (const std::string& );

        void generate (const int& nsize); 
        void generateNeg (const int& nsize);
        void generateRange (const BigNumberImpl*);

        void seedRNG (const std::string& );

        const BN_ptr& bn () const { return m_bn;}
    private:

        BigNumberImpl(const BN_ptr& obj) : m_bn(BN_new(), ::BN_free){ 
            BN_copy(m_bn.get(), obj.get()) ;           
        }
        BN_ptr m_bn; 

};

#endif