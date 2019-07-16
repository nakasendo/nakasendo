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
    for ( auto & big : coefficients )
    {
        // apply modulo
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
    for ( auto & element : strCoefficients )
    {
        BigNumber big ;
	    big.FromDec( element ) ;
        
        // apply modulo
        this->push_back( big ) ;
    }
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

/*
 */
BigNumber Polynomial::operator( ) (  const BigNumber& x )
{
    if ( m_coefficients.empty() )
    {
        // <todo> how do we handle errors?  
        std::cout << "Polynomial is empty, returning" << std::endl ;
        throw ;
    }

    if ( m_coefficients.back( ) == GenerateZero( ) )
    {
        // <todo> how do we handle errors?  
        std::cout << "Polynomial has zero coefficient at the highest degree, returning" << std::endl ;
        throw ;
    }

    return ( this->evaluate( x ) );

}

    /**
    * Evaluate the polynomial at value x
    * @param x The value of x 
    * @return  The evaluated result 
    */
    BigNumber Polynomial::evaluate(  const BigNumber& x )  
    {
        BigNumber res = GenerateZero( ) ;

        // Horners method for polynomial evaluation
        for( auto coef = m_coefficients.cbegin(); coef != m_coefficients.cend(); ++coef )
        {   
            res = ( res * x ) + *coef ;
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
                os << " + " << poly.m_coefficients[ i ].ToDec( ) << "^" << i ;
            }
        }
    }
    return os ;
}
