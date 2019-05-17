#ifndef __ECPOINT_IMPL_H__
#define __ECPOINT_IMPL_H__

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cctype>
#include <openssl/bn.h>
#include <openssl/objects.h>
#include <openssl/ec.h>

using namespace std;

//using ECPOINT_ptr = std::unique_ptr<EC_POINT, decltype(&EC_POINT_free)>; 
//using //ECGROUP_ptr = std::unique_ptr<EC_GROUP, decltype(&EC_GROUP_free)>;
typedef EC_POINT*  ECPOINT_ptr;
typedef EC_GROUP*  ECGROUP_ptr;
typedef const EC_METHOD* ECMETHOD_ptr;
typedef EC_builtin_curve* EC_builtin_curve_ptr;

using CurveList = std::vector<std::pair<int, std::string>>;

using CTX_ptr = std::unique_ptr<BN_CTX, decltype(&::BN_CTX_free)>;

inline  bool caseInSensitiveStringCompare(const std::string& s1, const std::string& s2)
{
    return(s1.size() == s2.size() && std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), 
                                                    [](const char& c1, const char& c2){
                                                       return (c1 == c2 || std::toupper(c1) == std::toupper(c2));
                                                    } ));
}

class ECPointImpl
{
    friend std::unique_ptr<ECPointImpl> Add(const ECPointImpl *obj1, const ECPointImpl *obj2);
    friend bool Compare(const ECPointImpl *obj1, const ECPointImpl *obj2);

    public:

        ECPointImpl() 
        { 
            m_gp = EC_GROUP_new_by_curve_name(NID_secp256k1);
            m_ec = EC_POINT_new(m_gp);
            this->nid = NID_secp256k1;
        }

        ECPointImpl(const int& nid) 
        {
            // Construct a builtin curve 
            if((m_gp = EC_GROUP_new_by_curve_name(nid)) == nullptr)
            {
                throw std::runtime_error("error : Failed to allocate curve");
            }
            m_ec = EC_POINT_new(m_gp);
            this->nid = nid;
        }

        ~ECPointImpl()
        {
            if (m_ec)
                EC_POINT_free(m_ec);

            if (m_gp)
                EC_GROUP_free(m_gp);

        }

        //
        ECPointImpl (const ECPointImpl& obj) 
        {
            method = obj.getMethod();
            m_gp = EC_GROUP_new(method);
            m_ec = EC_POINT_new(m_gp);
            EC_GROUP_copy (m_gp, obj.gp());
            EC_POINT_copy (m_ec, obj.ec());
            nid = obj.getNid();
        }

        ECPointImpl& operator= (const ECPointImpl& obj)
        {
            if (this != &obj)
            {
                method = obj.getMethod();
                m_gp = EC_GROUP_new(method);
                m_ec = EC_POINT_new(m_gp);
                EC_GROUP_copy (m_gp, obj.gp());
                EC_POINT_copy (m_ec, obj.ec());
		nid = obj.getNid();
            }
            return *this;
        }

        const ECPOINT_ptr ec () const { return m_ec;}
        const ECGROUP_ptr gp () const { return m_gp;}
        ECMETHOD_ptr getMethod() const {return method;}
        const int getNid() const { return nid;};

        // Invert the given object
        void Invert();

        // Check if the EC is at Infinity
        bool CheckInfinity();

        // Check if the EC is on the curve
        bool CheckOnCurve();

        // Multiply ECPoint with Hex number
        std::unique_ptr<ECPointImpl> MultiplyWithHexBigNum (const std::string& bn_obj_m, const std::string& bn_obj_n); 

        // Multiply ECPoint with dec number
        std::unique_ptr<ECPointImpl> MultiplyWithDecBigNum (const std::string& bn_obj_m, const std::string& bn_obj_n); 

	// Double the ECPointImpl
        std::unique_ptr<ECPointImpl> Double();

        static CurveList  getCurveList();

        std::string ToHex();
        bool FromHex(const std::string& hexStr, int nid);

    private:

        ECPointImpl(const ECPOINT_ptr& ec, const ECGROUP_ptr& gp, ECMETHOD_ptr ec_method) 
        {
            method = ec_method;
            m_gp = EC_GROUP_new(ec_method);
            m_ec = EC_POINT_new(m_gp);
            EC_GROUP_copy (m_gp, gp);
            EC_POINT_copy (m_ec, ec);
        }

        std::unique_ptr<ECPointImpl> Multiply(BIGNUM *m_ptr, BIGNUM *n_ptr);

        ECPOINT_ptr m_ec = nullptr; 
        ECGROUP_ptr m_gp = nullptr; 
        ECMETHOD_ptr method = nullptr;
        int nid = 0;
};

#endif // __ECPOINT_IMPL_H__
