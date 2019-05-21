#include <ECPoint/ECPointImpl.h>
#include <BigNumbers/BigNumbersImpl.h>
#include <openssl/ec.h>

std::unique_ptr<ECPointImpl> Add(const ECPointImpl *obj1, const ECPointImpl *obj2)
{
    // check if the given ECPoints are not null
    if (obj1 == nullptr || obj2 == nullptr){
        throw std::runtime_error("error : Given object being null");
    }
    
       
    if (obj1->getNid() != obj2->getNid()){
        throw std::runtime_error("error : methods mismatched in the given objects");
    }

    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 
    ECGROUP_ptr resGroup = EC_GROUP_new_by_curve_name (obj1->getNid()); 
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

    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(resEC, resGroup,obj1->getNid())); 

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

    BIGNUM *mPtr = bn_obj_m_uptr.get() ; 
    BN_hex2bn(&mPtr, bn_obj_m.c_str());

    if (!bn_obj_n.empty())
    {        
        // Get BN from Hex String
        BIGNUM *nPtr  = bn_obj_n_uptr.get(); 
        BN_hex2bn(&nPtr, bn_obj_n.c_str());
    }else{
        bn_obj_n_uptr = nullptr; 
    }
    
    return Multiply(bn_obj_m_uptr.get(), bn_obj_n_uptr.get());
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


    // Get BN from Hex String
    BIGNUM *mPtr = bn_obj_m_uptr.get();
    BN_dec2bn(&mPtr, bn_obj_m.c_str());

    if (!bn_obj_n.empty())
    {         
        // Get BN from Hex String
        BIGNUM *nPtr = bn_obj_n_uptr.get();
        BN_dec2bn(&nPtr, bn_obj_n.c_str());
    }else{
        bn_obj_n_uptr = nullptr;
    }
    return Multiply(bn_obj_m_uptr.get(), bn_obj_n_uptr.get());
}

std::unique_ptr<ECPointImpl> ECPointImpl::Multiply(BIGNUM *mPtr, BIGNUM *nPtr)
{

    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 

    // allocate & get result group and EC struct
    ECGROUP_ptr resGroup =  EC_GROUP_new_by_curve_name(m_nid); 
    ECPOINT_ptr resEC = EC_POINT_new(resGroup);

    int res = EC_POINT_mul(resGroup, 
                           resEC, 
                           nPtr ,
                           m_ec,
                           mPtr,
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

    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(resEC, resGroup, m_nid)); 

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

    if ( obj1->getNid() != obj2->getNid()){
        return false; 
    }
    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 
    
    // allocate & get result group and EC struct
    ECGROUP_ptr resGroup = EC_GROUP_new_by_curve_name(obj1->getNid());

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

    ECPOINT_ptr resEC = EC_POINT_new(m_gp);

    if (! EC_POINT_dbl(m_gp, resEC, ec(), ctxptr))
    {
        // free CTX object
        BN_CTX_free(ctxptr);
	
        // free group and EC structs
        EC_POINT_free(resEC);

        throw std::runtime_error("error : Failed to double EC POINT");
    }

    // free CTX object
    BN_CTX_free(ctxptr);

    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(resEC, m_gp, m_nid)); 

    // free group and EC structs
    EC_POINT_free(resEC);
    return std::move(ResImpl);
}

/* Inverts the EC */
void ECPointImpl::Invert()
{
    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 

    int res = EC_POINT_invert(m_gp, m_ec, ctxptr);
    
    // free CTX object
    BN_CTX_free(ctxptr);

    if (!res)
    {
        throw std::runtime_error("error : Failed to invert EC POINT");
    }

}

bool ECPointImpl::CheckInfinity()
{
    if (! EC_POINT_is_at_infinity(m_gp, m_ec))
    {
        return false;
    }

    return true;
}

bool ECPointImpl::CheckOnCurve()
{
    // Allocate for CTX 
    BN_CTX* ctxptr = BN_CTX_new(); 

    int res = EC_POINT_is_on_curve(m_gp, m_ec, ctxptr);

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

    ecChar  = EC_POINT_point2hex(m_gp, m_ec, POINT_CONVERSION_COMPRESSED, ctxptr);
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
    if(nid != -1){
        m_nid = nid; 
        if (m_ec != nullptr)
            EC_POINT_free (m_ec);
        if(m_gp != nullptr)
            EC_GROUP_free(m_gp); 
        
        m_gp = EC_GROUP_new_by_curve_name(nid);
        m_ec = EC_POINT_new(m_gp); 
    }
      
    
    // Allocate for CTX
    BN_CTX* ctxptr = BN_CTX_new(); 
    if (ctxptr == nullptr) {
        return false;
    }

    m_ec = EC_POINT_hex2point(m_gp, hexStr.c_str(), m_ec, ctxptr);
    if (m_ec == nullptr){
        return false;
    }

    // free CTX object
    BN_CTX_free(ctxptr);

    return true; 
}

void ECPointImpl::SetRandom(){
    /* I believe this is correct but a real scientist might have to tell me the truth
        1) Initial a EC_POINT P as generator.
        2) Find a random bignum k such that 0 < k < order (of the group).
        3) Do scalar multiplication to get random point R = kP
    */


    BN_ptr k ( BN_new(), ::BN_free ); 
    BN_CTX *ctx = BN_CTX_new () ; 

    if ( !EC_GROUP_get_order(m_gp,k.get(),ctx)){
        BN_CTX_free(ctx); 
        throw std::runtime_error ("Invalid group order on set randon" ); 
    }

    if (!BN_rand(k.get(), BN_num_bits(k.get()), 0, 0)){
        BN_CTX_free(ctx); 
        throw std::runtime_error ("Unable to generate a random number" ); 
    }

    if (!EC_POINT_mul(m_gp,m_ec,k.get(),NULL,NULL,ctx)){
        BN_CTX_free(ctx); 
        throw std::runtime_error ("Unable to generate a random number" ); 
    }
    return ; 
}

std::pair<std::string, std::string> ECPointImpl::GetAffineCoords_GFp (){
    std::string xVal, yVal ; 

    BN_CTX* ctx = BN_CTX_new();
    if (!EC_POINT_is_on_curve(m_gp,m_ec,ctx)){
        BN_CTX_free(ctx); 
        std::cout << "Point not on the required curve.." << std::endl; 
        return std::make_pair(xVal, yVal) ; 
    }

    BN_ptr x ( BN_new(), ::BN_free ); 
    BN_ptr y ( BN_new(), ::BN_free ); 

    if (!EC_POINT_get_affine_coordinates_GFp(m_gp, m_ec, x.get(), y.get(), ctx)){
        BN_CTX_free(ctx); 
         std::cout << "Unable to get affine coordinates.." << std::endl; 
        return std::make_pair(xVal, yVal); 
    }

    xVal = BN_bn2hex(x.get());
    yVal = BN_bn2hex(y.get()); 
    BN_CTX_free(ctx); 
    return std::make_pair(xVal, yVal);     
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
