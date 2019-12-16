#include <Polynomial/Polynomial.h>
#include <ECPoint/ECPoint.h>

/*
 *  Construct a polynomial from a vector of BigNumber
 *  @param coefficients - coeffiecients as vector of BigNumbers
 *  @param modulo - the modulo to be applied to the polynomial
 */
Polynomial::Polynomial
    ( 
        std::vector< BigNumber >& coefficients, 
        const BigNumber& modulo 
    )    
    : m_modulo( modulo ) 
{ 

    // apply modulo and push back to vector
    for ( auto & big : coefficients )
    {
        this->pushBack( big ) ;
    }

    checkValid( ) ;
} 


/*
 *  Construct Polynomial with random numbers
 *  @param degree - the order of the polynomial to be generated
 *  @param modulo - the modulo to be applied to the polynomial
 */
Polynomial::Polynomial( int degree, const BigNumber& modulo ) 
    : m_modulo ( modulo )
{
    // provide defaults for minimum and maximum ranges
    BigNumber min ;
    BigNumber max ;
    min = GenerateZero( ) ;
    max = m_modulo ;

    if ( ( min == m_zero ) && ( max == m_zero ) )
    {
        throw std::range_error( "Invalid range, min == max == 0 " ) ;
    }

    randomBigGenerator( degree, min, max ) ;

    checkValid( ) ;
}

/*
 *  Construct Polynomial with random numbers
 *  @param degree - the order of the polynomial to be generated
 *  @param modulo - the modulo to be applied to the polynomial
 *  @param a_0 - fix the free coefficient
 *  @return void
 */
Polynomial::Polynomial
    ( 
        int degree, 
        const BigNumber& modulo, 
        const BigNumber& a_0 
    ) 
    : m_modulo ( modulo )
{
    if ( !( a_0 > m_zero ) )
    {
        throw std::range_error
            (   "a_0 coefficient should be strictly positive, a_0 = " 
                + a_0.ToDec( ) 
            ) ;
    }

    if ( a_0 > m_modulo )
    {
        throw std::range_error
            ( "a_0 coefficient should be lower than modulo, but a_0=" 
                + a_0.ToDec( ) 
                + ", modulo=" 
                + m_modulo.ToDec( ) 
            ) ;     
    }
    
    // provide defaults for minimum and maximum ranges
    BigNumber min ;
    BigNumber max ;
    min = GenerateZero( ) ;
    max = m_modulo ;

    if ( ( min == m_zero ) && ( max == m_zero ) )
    {
        throw std::range_error( "Invalid range, min == max == 0 " ) ;
    }

    randomBigGenerator( degree, min, max ) ;

    m_coefficients[ 0 ] = a_0 ;

    checkValid( ) ;
}

/*
 *  Construct Polynomial with random numbers
 *  @param degree - the order of the polynomial to be generated
 *  @param modulo - the modulo to be applied to the polynomial
 *  @param a_0 - fix the free coefficient
 *  @return void
 */
Polynomial::Polynomial
    ( 
        int degree, 
        const BigNumber& modulo, 
        const BigNumber& min,    
        const BigNumber& max     
    ) 
    : m_modulo ( modulo )
{
    if ( min < m_zero )
    {
        throw std::range_error
            (   "min should be zero or greater, min=" 
                + min.ToDec( ) 
            ) ;
    }
    if ( max < m_modulo )
    {
        throw std::range_error
            ( "max should greater than or equal to modulo, max=" 
                + max.ToDec( ) 
                + ", modulo=" 
                + m_modulo.ToDec( ) 
            ) ;     
    }

    if ( ( min == m_zero ) && ( max == m_zero ) )
    {
        throw std::range_error( "Invalid range, min == max == 0 " ) ;
    }

    randomBigGenerator( degree, min, max ) ;
    checkValid( ) ;
}


Polynomial::Polynomial(const Polynomial& obj)
    : m_coefficients(obj.m_coefficients),m_modulo(obj.m_modulo)
{ return ; }

Polynomial& Polynomial::operator= (const Polynomial& obj){
    if(this != &obj){
        m_coefficients = obj.m_coefficients;
        m_modulo = obj.m_modulo;
    }
    return *this;
}

int Polynomial::getDegree() const
{
    return (int)m_coefficients.size() - 1;
}

BigNumber const& Polynomial::operator[](unsigned int index)const
{
    return m_coefficients[index];
}

const std::vector<BigNumber>& Polynomial::getCoefficients() const
{
    return m_coefficients;
}

size_t Polynomial::length() const
{
    return m_coefficients.size();
}


/* Check Valid
 * Ensure the coefficients array conforms to the non-zero rules
 *       - m_coefficients must not be empty
 *       - m_coefficiens[0] must be non-zero and positive
 *       - m_coefficients[size-1] must be non-zero
 */
void Polynomial::checkValid( )
{
    if ( m_coefficients.empty() )
    { 
        throw std::runtime_error( "Polynomial is empty, returning" ) ;
    }

    if ( m_coefficients.back( ) == m_zero )
    {
        throw std::range_error( "Polynomial has zero coefficient at the highest degree, returning" ) ;
    }

    if ( m_coefficients.front( ) < GenerateOne( ) )
    {
        throw std::range_error( "a_0 coefficient should be strictly positive, returning" ) ;
    }     
}

/*
 * Push back a coefficient to the vector
 * @param BigNumber append to coefficients vector
 */
void Polynomial::pushBack( const BigNumber& big )
{
    BigNumber coeff( big ) ;
    if ( ! ( m_modulo == m_zero ) ) 
    {
        coeff = big % m_modulo ;  
    }

    m_coefficients.push_back( std::move( coeff ) ) ;       
}


/* RandomBigGenerator
 * Pushes random BigNumbers to back of coefficients vector
 * @param degree (size of vector is degree+1 )
 * @param min (minimum range for random number generator)
 * @param max (maximum range for random number generator)
 */
void Polynomial::randomBigGenerator
    ( 
        int degree,             
        const BigNumber& min,   
        const BigNumber& max    
    )
{
    if ( degree < 1 )
    {
        throw std::range_error( "Unble to generate polynomial with degree less than 1" ) ;
    }

    std::vector< BigNumber > tempCoefficients ;
    int i = 0 ;

    while ( i <= degree )
    {
        BigNumber randomBig ;
        randomBig = GenerateRandRange( min, max );

        if ( ( i == 0 ) || ( i == degree ) ) 
        {
            if ( m_modulo == m_zero )
            {
                while ( randomBig == m_zero ) randomBig = GenerateRandRange( min, max ) ;
            }
            else
            {
                while ( ( randomBig % m_modulo ) == m_zero ) randomBig = GenerateRandRange( min, max ) ;
            }   
        }

        tempCoefficients.push_back( std::move( randomBig ) ) ;        
        ++i ;
    }

    // apply modulo and push back to vector
    for ( auto & big : tempCoefficients )
    {
        this->pushBack( big ) ;
    } 
}



/* Evaluate the polynomial at value x
*  @param x The value of x 
*  @return  The evaluated result 
*/
BigNumber Polynomial::operator( ) ( const BigNumber& x ) const
{
    BigNumber res  = GenerateZero( ) ;

    // Horners method for polynomial evaluation
    for (auto ritr = m_coefficients.rbegin(); ritr != m_coefficients.rend(); ++ritr) 
    {
        res = *ritr + ( res *  x ) ;
    }

    if ( !( m_modulo == m_zero ) )
    {
        res = res % m_modulo ;
    } 
    return res ;
}


/*
 * Hide using the generator point
 * @param curveID   The ID of the cureve (defaulted to NID secp256k1)
 * @return          Hidden Coefficients, as a vector of BigNumbers
 */

std::vector< BigNumber >  Polynomial::hide( int curveID ) const
{
    std::vector< BigNumber > hiddenCoefficients ;

    for ( auto itr = m_coefficients.begin(); itr != m_coefficients.end(); ++itr )
    {
        // call function in ECPoint
        ECPoint pt = MultiplyByGeneratorPt( *itr, curveID );
        BigNumber bn;
        bn.FromHex(pt.ToHex());
        hiddenCoefficients.push_back (bn) ;
    }

    return hiddenCoefficients ;
}

std::vector<ECPoint> Polynomial::hideAsPoint( int curveID ) const{
    std::vector<ECPoint> hiddenCoefficients;
    for( auto itr = m_coefficients.begin(); itr != m_coefficients.end(); ++itr )
    {
        // call function in ECPoint
        hiddenCoefficients.push_back ( MultiplyByGeneratorPt( *itr, curveID ) ) ;
    }
    return hiddenCoefficients ;
}

/* Friend function operator<<
 * for writing out polynomial in human-friendly form
 */
std::ostream& operator<<( std::ostream &os, const Polynomial& poly )
{

    os << "modulo = " << poly.m_modulo.ToDec( ) << ";    " ;
    os << "polynomial = " ;
    
    const size_t numberCoefficients = poly.m_coefficients.size( ) ;

    if ( numberCoefficients > 0 )
    {
        os << poly.m_coefficients[ 0 ].ToDec( )  ;

        if ( numberCoefficients > 1 ) 
        {
            os << " + " << poly.m_coefficients[ 1 ].ToDec( ) << "x" ;

            if ( numberCoefficients > 2 )
            for ( int i = 2; i < poly.m_coefficients.size( ) ; ++i )         
            {
                os << " + " << poly.m_coefficients[ i ].ToDec( ) << "x^" << i ;
            }
        }
    }
    return os ;
}
