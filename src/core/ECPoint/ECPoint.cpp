#include <ECPoint/ECPoint.h>
#include <ECPoint/ECPointImpl.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <BigNumbers/BigNumbers.h>
#include <cassert>
#define ASSERT assert

ECPoint::ECPoint() : m_pImpl(new ECPointImpl){}

ECPoint::ECPoint(const int& nid) : m_pImpl(new ECPointImpl(nid)) {}

ECPoint::ECPoint(const std::string& NIDstring) 
{
    int nid = getNidForString(NIDstring);
    if (nid == -1)
    {
        throw std::runtime_error("Invalid NID string provided");
    }
    m_pImpl.reset( new ECPointImpl(nid));
}


ECPoint::~ECPoint()=default;

ECPoint::ECPoint(ECPoint&& obj) noexcept = default;

ECPoint& ECPoint::operator=(ECPoint&& obj) noexcept = default;

ECPoint::ECPoint(const ECPoint& obj) : m_pImpl(new ECPointImpl (*obj.m_pImpl)) { }

ECPoint::ECPoint(const BigNumber& x, const BigNumber& y) : m_pImpl(new ECPointImpl(x, y)) {}

const ECPointImpl* ECPoint::pImpl() const
{
    return const_cast <const ECPointImpl *>(m_pImpl.get());
}
ECPointImpl* ECPoint::pImpl()
{
    return m_pImpl.get();
}

ECPoint& ECPoint::operator=(const ECPoint& obj)
{
    if (this != &obj)
    {
        m_pImpl.reset( new ECPointImpl (*obj.m_pImpl));
    }
    return *this;
}

ECPoint operator+ (const ECPoint& obj1, const ECPoint& obj2)
{
    std::unique_ptr<ECPointImpl> res1 = Add(obj1.pImpl(),obj2.pImpl()); 
    ECPoint res;
    res.m_pImpl.reset(new ECPointImpl(*res1));
    return res;
}

bool operator == (const ECPoint& obj1, const ECPoint& obj2)
{
    return Compare(obj1.pImpl(), obj2.pImpl());
}

bool operator != (const ECPoint& obj1, const ECPoint& obj2)
{
    return !Compare(obj1.pImpl(), obj2.pImpl());
}

ECPoint ECPoint::MulHex(const std::string& objm, const std::string& objn)
{
    std::unique_ptr<ECPointImpl> res1 = this->pImpl()->MultiplyWithHexBigNum(objm, objn);
    ECPoint res;
    res.m_pImpl.reset(new ECPointImpl(*res1));
    return res;
}

ECPoint ECPoint::MulDec(const std::string& objm, const std::string& objn)
{
    std::unique_ptr<ECPointImpl> res1 = this->pImpl()->MultiplyWithDecBigNum(objm, objn);
    ECPoint res;
    res.m_pImpl.reset(new ECPointImpl(*res1));
    return res;
}

ECPoint ECPoint::Double()
{
    std::unique_ptr<ECPointImpl> res1 = this->pImpl()->Double();
    ECPoint res;
    res.m_pImpl.reset(new ECPointImpl(*res1));
    return res;
}

void ECPoint::SetRandom()
{
    pImpl()->SetRandom () ;
}

void ECPoint::Invert()
{
    return this->pImpl()->Invert();
}

bool ECPoint::CheckInfinity()
{
    return  this->pImpl()->CheckInfinity();
}

bool ECPoint::CheckOnCurve()
{
    return  this->pImpl()->CheckOnCurve();
}

std::pair<std::string, std::string> ECPoint::GetAffineCoords_GFp (){
    return pImpl()->GetAffineCoords_GFp () ;
}

std::pair<std::string, std::string> ECPoint::GetAffineCoords_GFp_Dec (){
    return pImpl()->GetAffineCoords_GFp_Dec () ;
}

std::string ECPoint::ToHex(const bool& compressed)
{
    return  this->pImpl()->ToHex(compressed);
}

std::string ECPoint::ToDec()
{
    return  this->pImpl()->ToDec();
}

int ECPoint::GroupNid() const
{
    return  this->pImpl()->getNid();
}

bool ECPoint::FromHex(const std::string& hexStr, int nid)
{
    return this->pImpl()->FromHex(hexStr, nid);
}

bool ECPoint::FromDec(const std::string& decStr, int nid){
    return this->pImpl()->FromDec(decStr, nid);
}
std::vector<std::tuple<int, std::string, std::string>> getCurveList()
{
    return  _getCurveList(); 
}

int ECPoint_API getNidForString(const std::string& NIDstr)
{
    // get the curve vec list
    std::vector<std::tuple<int, std::string, std::string>> nidVec = getCurveList();

    // iterate over the vec list and look for the matched one
    for(auto& nidTuple : nidVec)
    {
        if (std::get<1>(nidTuple) == NIDstr)
        {
            return std::get<0>(nidTuple);
        }
    }
    return -1;
}

BigNumber ECPoint::getECGroupOrder() const
{
    BigNumber bnVal;
    bnVal.FromHex(this->pImpl()->getGroupOrder());
    return std::move(bnVal);
}

int ECPoint::getECGroupDegree() const
{
    return this->pImpl()->getGroupDegree();
}

ECPoint ECPoint::getGenerator() const
{
    std::unique_ptr<ECPointImpl> res1 = this->pImpl()->getGenerator();
    ECPoint res;
    res.m_pImpl.reset(new ECPointImpl(*res1));
    return std::move(res);
}
