#include "Polynomial.h"



/*
    Construct a polynomial from a vector of BigNumber
 */
Polynomial::Polynomial
    ( 
        std::vector< BigNumber >& coefficients, 
        const BigNumber& modulo 
    )    
    : m_modulo( modulo ) 
{ 
    // apply modulo and push_back to vector
    for ( auto & big : coefficients )
    {
        this->push_back( big ) ;
    }
} 

/*
    Construct a polynomial from a vector of strings
 */
Polynomial::Polynomial
    ( 
        std::vector< std::string >& strCoefficients, 
        const BigNumber& modulo
    )
    : m_modulo ( modulo )
{
    // convert to BigNumber, apply modulo and push_back to vector
    for ( auto & element : strCoefficients )
    {
        BigNumber big ;
	    big.FromDec( element ) ;
        this->push_back( big ) ;
    }
}

/*
 *  Construct with random numbers from the degree and modulo
 *  Constraints: 
 *       - degree must be 1 or more
 *       - m_coefficienst[0] must be non-zero and positive
 *       - m_coefficients[size-1] must be non-zero and positive
 */
Polynomial::Polynomial( int degree, const BigNumber& modulo ) 
    : m_modulo ( modulo )
{
    setup( degree ) ;
}

/*
 *  Construct with random numbers from the degree and modulo
 *  @param degree - the order of the polynomial to be generated
 *         modulo - the modulo to be applied to the polynomial
 *         a_0 - fix the free coefficient
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
    if ( a_0 == m_zero )
    {
        throw std::range_error( "a_0 coefficient should be strictly positive" ) ;
    }

    if ( a_0 > m_modulo )
    {
        throw std::range_error
            ( "a_0 coefficient should be lower than modulo, but " 
                + a_0.ToDec( ) 
                + ">=" 
                + m_modulo.ToDec( ) 
            ) ;     
    }
    
    setup( degree ) ;

    m_coefficients[ 0 ] = a_0 ;
}

/**
 * Push_back a coefficients to the vector
 * Equivalent to append_coeff in Python 
 * <todo> to we want to rename this to append_coeff for consistency?
 * @param BigNumber append to coefficients vector
 */
void Polynomial::push_back( BigNumber big )
{
    BigNumber coeff ( big ) ; 

    if ( !( m_modulo == GenerateZero( ) ) )
    {
        coeff = big % m_modulo ;
    }   

    m_coefficients.push_back( std::move( coeff ) ) ;       
}

void Polynomial::setup( int degree )
{   
    if ( degree < 1 )
    {
        throw std::range_error( "Unble to generate polynomial with degree less than 1" ) ;
    }

    // default the min and max numbers
    BigNumber min, max ;
    min.FromDec( "0" ) ;

    if ( m_modulo == m_zero ) 
        max.FromDec( "100" ) ;        
    else
        max = m_modulo - GenerateOne( ) ;
    
    std::vector< BigNumber > tempCoefficients ;
    tempCoefficients = randomBigGenerator( degree, min, max );

    // apply modulo and push_back to vector
    for ( auto & big : tempCoefficients )
    {
        this->push_back( big ) ;
    }    
}

/* RandomBigGenerator
 *   Generates random BigNumber's
 * @Params: degree (size of vector is degree+1 )
 *          min (minimum range for random number generator)
 *          max (maximum range for random number generator)
 * @Returns vector of BigNumbers
 */
std::vector< BigNumber > Polynomial::randomBigGenerator
    ( 
        int degree, 
        const BigNumber& min, 
        const BigNumber& max 
    )
{

    std::vector< BigNumber > tempCoeffs ;
    int i = 0 ;

    while ( i < degree )
    {
        BigNumber randomBig ;

        randomBig = GenerateRandRange( min, max );

        if ( ( i == 0 ) || ( i == degree-1 ) )
        {
            while ( randomBig == m_zero )
            {
                randomBig = GenerateRandRange( min, max );
            }   
        }

        tempCoeffs.push_back( std::move( randomBig ) ) ;
        ++i ;
    }
    
    return( tempCoeffs ) ;
}

/*
 */
BigNumber Polynomial::operator( ) ( const BigNumber& x ) const
{
    if ( m_coefficients.empty() )
    { 
        throw std::runtime_error( "Polynomial is empty, returning" ) ;
    }

    if ( m_coefficients.back( ) == m_zero )
    {
        throw std::runtime_error( "Polynomial has zero coefficient at the highest degree, returning" ) ;
    }

    return ( this->evaluate( x ) );

}

    /**
    * Evaluate the polynomial at value x
    * @param x The value of x 
    * @return  The evaluated result 
    */
    BigNumber Polynomial::evaluate(  const BigNumber& x ) const
    {
        BigNumber res  = GenerateZero( ) ;

        // Horners method for polynomial evaluation
        for ( 
                auto itr = m_coefficients.cend( ) - 1  ;  
                itr >= m_coefficients.begin();  
                --itr
            ) 
        {
            res = *itr + ( res *  x ) ;
        }

       // if ( !( m_modulo == GenerateZero( ) ) )
        if ( !( m_modulo == m_zero ) )
        {
            res = res % m_modulo ;
        } 
        return res;
    }     



/* Friend function operator<<
 * for writing out polynomial in human-friendly form
 */
std::ostream& operator<<( std::ostream &os, const Polynomial& poly )
{

    os << "modulo = " << poly.m_modulo.ToDec( ) << ";\t" ;
    os << "polynomial = " ;
    
    int numberCoefficients = poly.m_coefficients.size( ) ;

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
