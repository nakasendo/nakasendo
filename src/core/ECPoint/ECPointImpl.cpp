#include <ECPoint/ECPointImpl.h>
#include <BigNumbers/BigNumbersImpl.h>
#include <openssl/ec.h>

std::unique_ptr<ECPointImpl> Add(const ECPointImpl *obj1, const ECPointImpl *obj2)
{
    // check if the given ECPoints are not null
    if (obj1 == nullptr || obj2 == nullptr)
    {
        throw std::runtime_error("error : Given object being null");
    }

    // make sure that the EC_METHOD of the given ECPoints are same
    if (EC_METHOD_get_field_type(obj1->getMethod()) != EC_METHOD_get_field_type(obj2->getMethod()))
    {
        throw std::runtime_error("error : methods mismatched in the given objects");
    }

    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 

    // allocate & get result group and EC struct
    ECGROUP_ptr resGroup = EC_GROUP_new(obj1->getMethod());
    ECPOINT_ptr resEC = EC_POINT_new(resGroup);

    if (! EC_POINT_add(resGroup, resEC, obj1->ec(), obj2->ec(), ctxptr))
    {
        // free CTX object
        BN_CTX_free(ctxptr);
	
        // free group and EC structs
        EC_POINT_free(resEC);
        EC_GROUP_free(resGroup);

        throw std::runtime_error("error : Failed to add EC POINTs");
    }

    // free CTX object
    BN_CTX_free(ctxptr);

    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(resEC, resGroup, EC_POINT_method_of(resEC))); 

    // free group and EC structs
    EC_POINT_free(resEC);
    EC_GROUP_free(resGroup);

    return std::move(ResImpl);
}

// bn_obj_n + ec_obj_q * bn_obj_m, where bn_obj_n can be nullptr
std::unique_ptr<ECPointImpl> ECPointImpl::MultiplyWithHexBigNum (const std::string& bn_obj_m, const std::string& bn_obj_n)
{
    // check if the given ECPoint, and Big number are not null
    if (bn_obj_m.empty())
    {
        throw std::runtime_error("error : Given objector big number being null");
    }

    BN_ptr bn_obj_m_uptr {BN_new(), ::BN_free};
    BN_ptr bn_obj_n_uptr {BN_new(), ::BN_free};

    BIGNUM *m_ptr = bn_obj_m_uptr.get();
    BIGNUM *n_ptr = nullptr;

    // Get BN from Hex String
    ::BN_hex2bn(&m_ptr, bn_obj_m.c_str());

    if (!bn_obj_n.empty())
    {
        n_ptr = bn_obj_n_uptr.get();
         
        // Get BN from Hex String
        ::BN_hex2bn(&n_ptr, bn_obj_n.c_str());
    }
    return Multiply(m_ptr, n_ptr);
}

// bn_obj_n + ec_obj_q * bn_obj_m, where bn_obj_n can be nullptr
std::unique_ptr<ECPointImpl> ECPointImpl::MultiplyWithDecBigNum (const std::string& bn_obj_m, const std::string& bn_obj_n)
{
    // check if the given ECPoint, and Big number are not null
    if (bn_obj_m.empty()) 
    {
        throw std::runtime_error("error : Given objector big number being null");
    }

    BN_ptr bn_obj_m_uptr {BN_new(), ::BN_free};
    BN_ptr bn_obj_n_uptr {BN_new(), ::BN_free};

    BIGNUM *m_ptr = bn_obj_m_uptr.get();
    BIGNUM *n_ptr = nullptr;

    // Get BN from Hex String
    ::BN_dec2bn(&m_ptr, bn_obj_m.c_str());

    if (!bn_obj_n.empty())
    {
        n_ptr = bn_obj_n_uptr.get();
         
        // Get BN from Hex String
        ::BN_dec2bn(&n_ptr, bn_obj_n.c_str());
    }
    return Multiply(m_ptr, n_ptr);
}

std::unique_ptr<ECPointImpl> ECPointImpl::Multiply(BIGNUM *m_ptr, BIGNUM *n_ptr = nullptr)
{

    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 

    // allocate & get result group and EC struct
    ECGROUP_ptr resGroup = EC_GROUP_new(this->getMethod());
    ECPOINT_ptr resEC = EC_POINT_new(resGroup);

    int res = EC_POINT_mul(resGroup, 
                           resEC, 
                           n_ptr == nullptr ? nullptr : n_ptr,
                           this->ec(),
                           m_ptr,
                           ctxptr);

    // free CTX object
    BN_CTX_free(ctxptr);

    if (!res)
    {
        // free group and EC structs
        EC_POINT_free(resEC);
        EC_GROUP_free(resGroup);
      
        throw std::runtime_error("error : Failed to multiply EC POINT with the BIGNUM");
    }

    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(resEC, resGroup, EC_POINT_method_of(resEC))); 

    // free group and EC structs
    EC_POINT_free(resEC);
    EC_GROUP_free(resGroup);

    return std::move(ResImpl);
}


bool Compare(const ECPointImpl *obj1, const ECPointImpl *obj2)
{
    // check if the given ECPoints are not null
    if (obj1 == nullptr || obj2 == nullptr)
    {
        throw std::runtime_error("error : Given object being null");
    }

    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 
    
    // allocate & get result group and EC struct
    ECGROUP_ptr resGroup = EC_GROUP_new(obj1->getMethod());

    int res = EC_POINT_cmp(resGroup, obj1->ec(), obj2->ec(), ctxptr); 

    // free CTX object
    BN_CTX_free(ctxptr);

    // free group struct
    EC_GROUP_free(resGroup);

    if (res != 0)
    {
        return false;
    }

    return true;
}

std::unique_ptr<ECPointImpl> ECPointImpl::Double()
{

    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 

    // allocate & get result group and EC struct
    ECGROUP_ptr resGroup = EC_GROUP_new(getMethod());
    ECPOINT_ptr resEC = EC_POINT_new(resGroup);

    if (! EC_POINT_dbl(resGroup, resEC, ec(), ctxptr))
    {
        // free CTX object
        BN_CTX_free(ctxptr);
	
        // free group and EC structs
        EC_POINT_free(resEC);
        EC_GROUP_free(resGroup);

        throw std::runtime_error("error : Failed to double EC POINT");
    }

    // free CTX object
    BN_CTX_free(ctxptr);

    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(resEC, resGroup, EC_POINT_method_of(resEC))); 

    // free group and EC structs
    EC_POINT_free(resEC);
    EC_GROUP_free(resGroup);

    return std::move(ResImpl);
}

/* Inverts the EC */
void ECPointImpl::Invert()
{
    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 

    int res = EC_POINT_invert(this->gp(), this->ec(), ctxptr);
    
    // free CTX object
    BN_CTX_free(ctxptr);

    if (!res)
    {
        throw std::runtime_error("error : Failed to invert EC POINT");
    }

}

bool ECPointImpl::CheckInfinity()
{
    if (! EC_POINT_is_at_infinity(this->gp(), this->ec()))
    {
        return false;
    }

    return true;
}

bool ECPointImpl::CheckOnCurve()
{
    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 

    int res = EC_POINT_is_on_curve(this->gp(), this->ec(), ctxptr);

    // free CTX object
    BN_CTX_free(ctxptr);

    if (res == -1)
    {
        throw std::runtime_error("error : Failed to check if the EC POINT is on the curve");
    }

    return res == 0 ? false : true;
}


std::string ECPointImpl::ToHex()
{
    char *ecChar = nullptr; 
    
    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 

    ecChar  = EC_POINT_point2hex(gp(), ec(), POINT_CONVERSION_COMPRESSED, ctxptr);
    if ( ecChar == nullptr)
    {
        // free CTX object
        BN_CTX_free(ctxptr);
    }

    std::string ecStr(ecChar) ;

    // free 
    OPENSSL_free(ecChar);
  
    // free CTX object
    BN_CTX_free(ctxptr);

    return ecStr;
}

bool ECPointImpl::FromHex(const std::string& hexStr, int nid)
{
    ECGROUP_ptr _gp = EC_GROUP_new_by_curve_name(nid);
    ECPOINT_ptr _ec = nullptr;
    if (_gp == nullptr)
        return false;
   
    
    // Allocate for CTX
    BN_CTX* ctxptr = BN_CTX_new(); 
    if (ctxptr == nullptr) 
    {
        EC_GROUP_free(_gp);
        return false;
    }

    _ec = EC_POINT_hex2point(_gp, hexStr.c_str(), nullptr, nullptr);
    if (_ec == nullptr)
    {
        EC_GROUP_free(_gp);
        return false;
    }

    // free CTX object
    BN_CTX_free(ctxptr);

    // free up the existing memory of group and ec
    if (m_ec)
        EC_POINT_free(m_ec);

    if (m_gp)
        EC_GROUP_free(m_gp);

    m_ec = nullptr;
    m_gp = nullptr;

    // Update the existing
    this->nid = nid;
    m_gp  =  _gp;
    m_ec = _ec;

    return true; 
}

CurveList _getCurveList()
{
    /* Get a list of all internal curves */
    auto crvLen = EC_get_builtin_curves(NULL, 0);

    EC_builtin_curve *curves = (EC_builtin_curve *) OPENSSL_malloc(sizeof(EC_builtin_curve) * crvLen);

    if (curves == nullptr)
    {
        throw std::runtime_error("error : Failed to allocate memory for internal curves");
    }

    if (!EC_get_builtin_curves(curves, crvLen)) 
    {
        throw std::runtime_error("error : Failed to EC_get_builtin_curves to get internal curve list");
    }

    CurveList _curveList;
    for (int i = 0; i < crvLen; i++)
    {
        _curveList.push_back(make_pair(curves[i].nid, curves[i].comment)); 
    }
  
    /* NOTE : curves has an internal pointer, comment, which shouldn't freed as its just a pointer to a constant curve_list->comment*/
    if (curves)
        ::OPENSSL_free(curves);

    return _curveList;
}
