#ifndef _EC_POINT__H__
#define _EC_POINT__H__

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_ECPoint
#    define ECPoint_API EXPORT_DYNAMIC_LIBRARY
#else
#    define ECPoint_API IMPORT_DYNAMIC_LIBRARY
#endif

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <tuple>
#include <BigNumbers/BigNumbers.h>



class ECPointImpl; 

class ECPoint_API ECPoint
{
    friend ECPoint ECPoint_API operator+ (const ECPoint&, const ECPoint&);
    friend bool ECPoint_API operator == (const ECPoint&, const ECPoint&);
    friend bool ECPoint_API operator != (const ECPoint&, const ECPoint&);

    public:
        explicit ECPoint();
	    ECPoint(const BigNumber& x, const BigNumber& y);
        ~ECPoint();
        ECPoint(const int& nid);
        ECPoint(const std::string& NIDstring);

        //moveable
        ECPoint(ECPoint&& obj) noexcept;
        ECPoint& operator=(ECPoint&& obj) noexcept;

        //copyable
        ECPoint(const ECPoint& obj);
        ECPoint& operator=(const ECPoint& obj);

        ECPoint MulHex(const std::string& objm, const std::string& objn);
        ECPoint MulDec(const std::string& objm, const std::string& objn);
        ECPoint Double();

        void SetRandom() ; 
        void Invert();
        bool CheckInfinity();
        bool CheckOnCurve();

        std::string ToHex(const bool& compressed = true) const ;
        std::string ToDec(const bool& compressed = true) const ;

        int GroupNid()const;

        bool FromHex(const std::string& hexStr, int nid=-1) ;
        bool FromDec(const std::string& decStr, int nid=-1) ;

        std::pair<std::string, std::string> GetAffineCoords_GFp () const ;
        std::pair<std::string, std::string> GetAffineCoords_GFp_Dec () const ;

        BigNumber getECGroupOrder() const;
        int getECGroupDegree() const;
        ECPoint getGenerator() const;

    private:
        const ECPointImpl* pImpl() const;
        ECPointImpl* pImpl();

        std::unique_ptr<ECPointImpl> m_pImpl ; 
};


std::vector<std::tuple<int, std::string, std::string>> ECPoint_API getCurveList();
int ECPoint_API getNidForString(const std::string& NIDstr);
BigNumber MultiplyByGenerator( const BigNumber& bigNum, int curveID ) ; 

#endif //ifndef _EC_POINT__H__
