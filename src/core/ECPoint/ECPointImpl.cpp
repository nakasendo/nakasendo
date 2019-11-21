#include <ECPoint/ECPointImpl.h>
#include <BigNumbers/BigNumbers.h>
#include <BigNumbers/BigNumbersImpl.h>
#include <openssl/ec.h>
#include <openssl/objects.h>

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
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    ECGROUP_ptr resGroup = EC_GROUP_new_by_curve_name (obj1->getNid()); 
    ECPOINT_ptr resEC = EC_POINT_new(resGroup);

    if (! EC_POINT_add(resGroup, resEC, obj1->ec(), obj2->ec(), ctxptr.get()))
    {
        // free group and EC structs
        EC_POINT_free(resEC);
        EC_GROUP_free(resGroup);

        throw std::runtime_error("error : Failed to add EC POINTs");
    }

    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(resEC, obj1->getNid()));

    // free group and EC structs
    EC_POINT_free(resEC);
    EC_GROUP_free(resGroup);

    return ResImpl;
}

ECPointImpl::ECPointImpl(const BigNumber& bn_obj_x, const BigNumber& bn_obj_y, const int& nid)
{
    // Construct a builtin curve 
    if((m_gp = EC_GROUP_new_by_curve_name(nid)) == nullptr)
    {
        throw std::runtime_error("error : Failed to allocate curve");
    }
    m_ec = EC_POINT_new(m_gp);

    m_nid = nid;
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );

    // Get BN from Hex String
    BIGNUM *xPtr = nullptr;
    BN_hex2bn(&xPtr, bn_obj_x.ToHex().c_str());
    BN_ptr bnXPtr{xPtr, ::BN_free};

    // Get BN from Hex String
    BIGNUM *yPtr = nullptr;
    BN_hex2bn(&yPtr, bn_obj_y.ToHex().c_str());
    BN_ptr bnYPtr{yPtr, ::BN_free};

    if (!EC_POINT_set_affine_coordinates_GFp(m_gp, m_ec, bnXPtr.get(), bnYPtr.get(), ctxptr.get()))
    {
        throw std::runtime_error("Failed to set coordinates.");
    }

}

// bn_obj_n + ec_obj_q * bn_obj_m, where bn_obj_n can be nullptr
std::unique_ptr<ECPointImpl> ECPointImpl::MultiplyWithHexBigNum (const std::string& bn_obj_m, const std::string& bn_obj_n) const
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
std::unique_ptr<ECPointImpl> ECPointImpl::MultiplyWithDecBigNum (const std::string& bn_obj_m, const std::string& bn_obj_n) const
{
    // check if the given ECPoint, and Big number are not null
    if (bn_obj_m.empty()) 
    {
        throw std::runtime_error("error : Given objector big number being null");
    }



    // Get BN from Dec String
    BIGNUM *mPtr = nullptr;
    BN_dec2bn(&mPtr, bn_obj_m.c_str());
    BN_ptr bn_obj_m_uptr {mPtr, ::BN_free};

    BIGNUM *nPtr = nullptr;

    if (!bn_obj_n.empty())
    {         
        // Get BN from Hex String
        BN_dec2bn(&nPtr, bn_obj_n.c_str());
    }
    BN_ptr bn_obj_n_uptr {nPtr, ::BN_free};
   
    return Multiply(bn_obj_m_uptr.get(), bn_obj_n_uptr.get());
}

std::unique_ptr<ECPointImpl> ECPointImpl::Multiply(BIGNUM *mPtr, BIGNUM *nPtr) const
{

    // Allocate for CTX 
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );

    // allocate & get result group and EC struct
    ECGROUP_ptr resGroup = EC_GROUP_new_by_curve_name(m_nid); 
    ECPOINT_ptr resEC = EC_POINT_new(resGroup);

    int res = EC_POINT_mul(resGroup, 
                           resEC, 
                           nPtr,
                           m_ec,
                           mPtr,
                           ctxptr.get());

    if (!res)
    {
        // free group and EC structs
        EC_POINT_free(resEC);
        EC_GROUP_free(resGroup);
      
        throw std::runtime_error("error : Failed to multiply EC POINT with the BIGNUM");
    }

    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(resEC, m_nid));

    // free group and EC structs
    EC_POINT_free(resEC);
    EC_GROUP_free(resGroup);

    return ResImpl;
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
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    
    // allocate & get result group and EC struct
    ECGROUP_ptr resGroup = EC_GROUP_new_by_curve_name(obj1->getNid());

    int res = EC_POINT_cmp(resGroup, obj1->ec(), obj2->ec(), ctxptr.get()); 

    // free group struct
    EC_GROUP_free(resGroup);

    if (res != 0)
    {
        return false;
    }

    return true;
}

std::unique_ptr<ECPointImpl> ECPointImpl::Double() const
{

    // Allocate for CTX 
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );

    // allocate & get result group and EC struct

    ECPOINT_ptr resEC = EC_POINT_new(m_gp);

    if (! EC_POINT_dbl(m_gp, resEC, ec(), ctxptr.get()))
    {
        // free group and EC structs
        EC_POINT_free(resEC);

        throw std::runtime_error("error : Failed to double EC POINT");
    }

    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(resEC, m_nid));

    // free group and EC structs
    EC_POINT_free(resEC);
    return ResImpl;
}

/* Inverts the EC */
void ECPointImpl::Invert()
{
    // Allocate for CTX 
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );

    int res = EC_POINT_invert(m_gp, m_ec, ctxptr.get());
    
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
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );

    int res = EC_POINT_is_on_curve(m_gp, m_ec, ctxptr.get());

    if (res == -1)
    {
        throw std::runtime_error("error : Failed to check if the EC POINT is on the curve");
    }

    return res == 0 ? false : true;
}

/*
An EC Point is a point (X, Y)
Its serialization is 04+X+Y as uncompressed, and (02+X as compressed if Y is even), and (03+X as compressed if Y is odd). X and Y are here the corresponding 64-character hexadecimal string
*/
std::string ECPointImpl::ToHex(const bool& compressed) const
{
    char *ecChar = nullptr; 
    
    // Allocate for CTX 
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    if(compressed)
        ecChar  = EC_POINT_point2hex(m_gp, m_ec, POINT_CONVERSION_COMPRESSED, ctxptr.get());
    else
        ecChar  = EC_POINT_point2hex(m_gp, m_ec, POINT_CONVERSION_UNCOMPRESSED, ctxptr.get());

    if ( ecChar == nullptr)
    {
	    std::runtime_error("Failed to convert EC Point to Hex");
    }

    std::string ecStr(ecChar) ;

    // free 
    OPENSSL_free(ecChar);

    return ecStr;
}

std::string ECPointImpl::ToDec(const bool& compressed) const
{
    char *ecChar = nullptr; 
    
    // Allocate for CTX 
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    BN_ptr bn_obj_n_uptr {BN_new(), ::BN_free};
    if(compressed)
        EC_POINT_point2bn(m_gp, m_ec, POINT_CONVERSION_COMPRESSED, bn_obj_n_uptr.get(), ctxptr.get());
    else
        EC_POINT_point2bn(m_gp, m_ec, POINT_CONVERSION_UNCOMPRESSED, bn_obj_n_uptr.get(), ctxptr.get());
        
    ecChar = BN_bn2dec(bn_obj_n_uptr.get());
    if ( ecChar == nullptr){
	    std::runtime_error("Failed to convert EC Point to Hex");
    }
    std::string ecStr(ecChar) ;
    // free 
    OPENSSL_free(ecChar);
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
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    if (ctxptr == nullptr) {
        return false;
    }

    m_ec = EC_POINT_hex2point(m_gp, hexStr.c_str(), m_ec, ctxptr.get());
    if (m_ec == nullptr){
        return false;
    }

    return true;
}

bool ECPointImpl::FromDec(const std::string& decStr, int nid)
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
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    if (ctxptr == nullptr) {
        return false;
    }

    BIGNUM * bnptr = BN_new();

    //BN_ptr bn_obj_n_uptr {BN_new(), ::BN_free};

    //BIGNUM * bnptr = bn_obj_n_uptr.get();
    BN_dec2bn(&bnptr, decStr.c_str());
    m_ec = EC_POINT_bn2point(m_gp, bnptr, m_ec, ctxptr.get());
    if (m_ec == nullptr){
        BN_free(bnptr);
        return false;
    }

    BN_free(bnptr);

    return true;
}
void ECPointImpl::SetRandom(){
    /* I believe this is correct but a real scientist might have to tell me the truth
        1) Initial a EC_POINT P as generator.
        2) Find a random bignum k such that 0 < k < order (of the group).
        3) Do scalar multiplication to get random point R = kP
    */


    BN_ptr k ( BN_new(), ::BN_free );
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );

    if ( !EC_GROUP_get_order(m_gp,k.get(),ctxptr.get())){
        throw std::runtime_error ("Invalid group order on set random" );
    }

    if (!BN_rand(k.get(), BN_num_bits(k.get()), 0, 0)){
        throw std::runtime_error ("Unable to generate a random number" );
    }

    if (!EC_POINT_mul(m_gp,m_ec,k.get(),NULL,NULL,ctxptr.get())){
        throw std::runtime_error ("Unable to generate a random number" );
    }
    return ;
}

std::pair<std::string, std::string> ECPointImpl::GetAffineCoords_GFp ()const 
{
    std::string xVal, yVal ;

    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    if (!EC_POINT_is_on_curve(m_gp,m_ec, ctxptr.get())){
        return std::make_pair(xVal, yVal) ;
    }

    BN_ptr x ( BN_new(), ::BN_free );
    BN_ptr y ( BN_new(), ::BN_free );

    if (!EC_POINT_get_affine_coordinates_GFp(m_gp, m_ec, x.get(), y.get(), ctxptr.get())){
        return std::make_pair(xVal, yVal);
    }

    char *xCharVal = BN_bn2hex(x.get());
    char *yCharVal = BN_bn2hex(y.get());
    xVal = xCharVal;
    yVal = yCharVal;
    OPENSSL_free(xCharVal);
    OPENSSL_free(yCharVal);
    return std::make_pair(xVal, yVal);
}

std::pair<std::string, std::string> ECPointImpl::GetAffineCoords_GFp_Dec () const
{
    std::string xVal, yVal ;

    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    
    if (!EC_POINT_is_on_curve(m_gp,m_ec, ctxptr.get())){
        return std::make_pair(xVal, yVal) ;
    }

    BN_ptr x ( BN_new(), ::BN_free );
    BN_ptr y ( BN_new(), ::BN_free );
   
    if (!EC_POINT_get_affine_coordinates_GFp(m_gp, m_ec, x.get(), y.get(), ctxptr.get())){
        return std::make_pair(xVal, yVal);
    }

    char *xCharVal = BN_bn2dec(x.get());
    char *yCharVal = BN_bn2dec(y.get());
    xVal = xCharVal;
    yVal = yCharVal;
    OPENSSL_free(xCharVal);
    OPENSSL_free(yCharVal);
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
        const char *sname = nullptr;

        sname =  OBJ_nid2sn(curves[i].nid);
        if (sname== nullptr)
            sname = "";
        _curveList.push_back(make_tuple(curves[i].nid, sname, curves[i].comment)); 
    }
  
    /* NOTE : curves has an internal pointer, comment, which shouldn't freed as its just a pointer to a constant curve_list->comment*/
    if (curves)
        ::OPENSSL_free(curves);

    return _curveList;
}

std::string ECPointImpl::getGroupOrder() const
{
    BN_ptr x(BN_new(), ::BN_free );

    // Allocate for CTX
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );

    if(!EC_GROUP_get_order(m_gp, x.get(), ctxptr.get()))
    {
        return {};
    }

    char *xCharVal = BN_bn2hex(x.get());
    std::string xVal = xCharVal;
    OPENSSL_free(xCharVal);
    return xVal;
}

int ECPointImpl::getGroupDegree() const
{
    return EC_GROUP_get_degree(m_gp);
}

std::unique_ptr<ECPointImpl> ECPointImpl::getGenerator() const
{
    std::unique_ptr<ECPointImpl> ResImpl (new ECPointImpl(EC_GROUP_get0_generator(m_gp), m_nid));
    return ResImpl;
}
