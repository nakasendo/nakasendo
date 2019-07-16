#include "Polynomial.h"



/*
    Construct a polynomial from a vector of BigNumber
 */
Polynomial::Polynomial( std::vector< BigNumber >& coefficients, int groupModulo )    
    : m_modulo( groupModulo ) 
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
        int groupModulo
    )
    : m_modulo ( groupModulo )
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
    BigNumber modulo ;
    modulo.FromDec( std::to_string( m_modulo ) ) ;

    if ( m_modulo )
    {
        coeff = big % modulo ;
    }   

    m_coefficients.push_back( std::move( coeff ) ) ;       
}


/* Friend function operato<<
 * for writing out polynomial in human-friendly form
 */
std::ostream& operator<<( std::ostream &os, const Polynomial& poly )
{

    os << "modulo = " << poly.m_modulo << ";\t" ;
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
