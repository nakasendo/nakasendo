#ifndef __BIG_NUMBERS_H__
#define __BIG_NUMBERS_H__

#include <BigNumAPI.h>

#include <memory>
#include <string>

class BigNumberImpl; 

class BigNumbers_API BigNumber
{
    friend BigNumber BigNumbers_API operator+ ( const BigNumber&, const BigNumber&);
    friend BigNumber BigNumbers_API operator+ ( const BigNumber&, const int&);
    friend BigNumber BigNumbers_API operator- (const BigNumber&, const BigNumber&);
    friend BigNumber BigNumbers_API operator- (const BigNumber&, const int&);

    friend BigNumber BigNumbers_API operator% (const BigNumber&, const BigNumber&);
    friend bool BigNumbers_API operator> (const BigNumber&, const BigNumber& );
    friend bool BigNumbers_API operator< (const BigNumber&, const BigNumber& );
    friend bool BigNumbers_API operator== (const BigNumber&, const BigNumber& );

    public:
        explicit BigNumber() ; 
        ~BigNumber();
        //moveable
        BigNumber(BigNumber&& obj) noexcept; 
        BigNumber& operator=(BigNumber&& obj) noexcept;
        //copyable
        BigNumber(const BigNumber& obj);
        BigNumber& operator=(const BigNumber& obj);

        // Set the value to one/zero/negative/positive
        void One(); 
        void Zero(); 
        void Negative () ; 
        void Positive ();

        // Pre/post inc/dec
        BigNumber operator++ (int);
        BigNumber& operator++ ();
        BigNumber operator-- (int);
        BigNumber& operator-- ();

        // public interface
        std::string ToHex () const ; 
        std::string ToDec () const ;

        int FromHex (const std::string& );
        int FromDec (const std::string& );

        // Generate & return string Representation
        std::string generateRandHex (const int& nsize=512) ; 
        std::string generateRandDec (const int& nsize=512) ; 
        std::string generateNegRandHex (const int& nsize=512);
        std::string generateNegRandDec (const int& nsize=512);
        std::string generateRandRange (const BigNumber&);

        
        void seedRNG (const std::string& ) ; 
        std::string generateRandHexWithSeed(const std::string&, const int& nsize=512); 
        std::string generateRandDecWithSeed(const std::string&, const int& nsize=512);
        std::string generateNegRandHexWithSeed (const std::string&, const int& nsize=512);
        std::string generateNegRandDecWithSeed (const std::string&, const int& nsize=512);
        std::string generateRangRandHexWithSeed (const std::string&, const BigNumber&);
        std::string generateRangRandDecWithSeed (const std::string&, const BigNumber&);


        
    private:
        const BigNumberImpl* pImpl() const { return m_pImpl.get(); }
        BigNumberImpl* pImpl() {return m_pImpl.get();}       

        std::unique_ptr<BigNumberImpl> m_pImpl ; 
};

BigNumber BigNumbers_API GenerateOne ();
BigNumber BigNumbers_API GnerateZero () ;

BigNumber BigNumbers_API GenerateRand ( const int& )  ;
BigNumber BigNumbers_API GenerateRandNegative (const int&);
BigNumber BigNumbers_API GenerateRandWithSeed(const std::string&, const int&);
// Generate in a range 
BigNumber BigNumbers_API GenerateRandRange(const BigNumber& min, const BigNumber& max,const int& nsize=512);



#endif //ifndef __BIG_NUMBERS_H__


