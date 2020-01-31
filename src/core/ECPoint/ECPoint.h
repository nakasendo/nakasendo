#ifndef _EC_POINT__H__
#define _EC_POINT__H__

#include <ECPoint/ECPointConfig.h>

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <tuple>
#include <BigNumbers/BigNumbers.h>



class ECPointImpl; 

class ECPoint_API ECPoint
{
    ECPoint_API friend ECPoint operator+ (const ECPoint&, const ECPoint&);
    ECPoint_API friend bool operator == (const ECPoint&, const ECPoint&);
    ECPoint_API friend bool operator != (const ECPoint&, const ECPoint&);

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

        ECPoint MulHex(const std::string& objm, const std::string& objn) const ;
        ECPoint MulDec(const std::string& objm, const std::string& objn) const ;
        ECPoint Double() const;

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


ECPoint_API std::vector<std::tuple<int, std::string, std::string>> getCurveList();
ECPoint_API int getNidForString(const std::string& NIDstr);
//ECPoint_API BigNumber MultiplyByGenerator( const BigNumber& bigNum, int curveID ) ; 
ECPoint_API ECPoint MultiplyByGeneratorPt(const BigNumber&, int curveID=714);

#endif //ifndef _EC_POINT__H__
