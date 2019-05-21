#include <ECPoint/ECPoint.h>
#include <ECPoint/ECPointImpl.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <ECPoint/ECPointImpl.h>

#include <cassert>
#define ASSERT assert

ECPoint::ECPoint() : m_pImpl(new ECPointImpl){}

ECPoint::ECPoint(const int& nid) : m_pImpl(new ECPointImpl(nid)) {}

ECPoint::~ECPoint()=default;

ECPoint::ECPoint(ECPoint&& obj) noexcept = default;

ECPoint& ECPoint::operator=(ECPoint&& obj) noexcept = default;

ECPoint::ECPoint(const ECPoint& obj) : m_pImpl(new ECPointImpl (*obj.m_pImpl)) { }

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

std::string ECPoint::ToHex()
{
    return  this->pImpl()->ToHex();
}

int ECPoint::GroupNid() const
{
    return  this->pImpl()->getNid();
}

bool ECPoint::FromHex(const std::string& hexStr, int nid)
{
    return this->pImpl()->FromHex(hexStr, nid);
}

std::vector<std::pair<int, std::string>> getCurveList()
{
    return  _getCurveList(); 
}

int getNidForString(std::string& nidStr)
{
    // get the curve vec list
    std::vector<std::pair<int, std::string>> nidVec = getCurveList();

    // iterate over the vec list and look for the matched one
    for(auto& nidPair : nidVec)
    {
        if (nidPair.second == nidStr)
        {
            return nidPair.first;
        }
    }
    return -1;
}

