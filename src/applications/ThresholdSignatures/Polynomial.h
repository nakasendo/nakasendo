/*
 * Polynomical implementation
 */

#ifndef _NCHAIN_SDK_POLYNOMIAL_H_
#define _NCHAIN_SDK_POLYNOMIAL_H_


#include <vector>
#include <string>
#include <iostream>
#include <BigNumbers/BigNumbers.h>


/// 
class Polynomial
{
  public:
    // How do we want to constuct polynomials ?
    // From string?  From a BigNumber Vector ?

    // construct empty
    Polynomial( ) = default ;

    // construct from BigNumber vector
    Polynomial( std::vector< BigNumber >& coefficients, int groupModulo = 0 ) 
        : m_coefficients( coefficients )
        , m_modulo( groupModulo ) { } 

    // construct from String vector
    Polynomial( std::vector< std::string >& coefficients, int groupModulo = 0 ) ;

    /// Default destructor
    ~Polynomial() = default ;

    /**
    * Get the degree of the polynomial.
    * @return The degree (order) of the polynomial
    */
    int getDegree() { return m_coefficients.size() -1 ; }

        /**
    * Evaluate the polynomial at value x
    * @param x The value of x 
    * @return  The evaluated result 
    */
    BigNumber evaluate( const BigNumber& x ) 
    {
        BigNumber res ;
	    res.FromDec( std::to_string( 0 ) ) ;

        // Horners method for polynomial evaluation
        for( auto coef = m_coefficients.cbegin(); coef != m_coefficients.cend(); ++coef )
        {   
            res = ( res * x ) + *coef ;
        }

        return res;
    }    


    /**
     * Push_back a coefficients to the vector
     * Equivalent to append_coeff in Python 
     * <todo> to we want to rename for consistency?
     * <todo> do we need this ?  Remove ?
     * @return 
     
 
    void push_back( BigNumber big )
    {
        std::cout << "do we get here ?" << std::endl ;
        BigNumber coeff ( big ) ; 
        BigNumber modulo ;
        //modulo.FromDec( modulo ) ;

        if ( m_modulo )
        {
            coeff = big % modulo ;
        }   

        std::cout << "pushing_back the coeff" << coeff.ToDec( ) << std::endl ;
        m_coefficients.push_back( std::move( coeff ) ) ;       
    }
*/
    /*
     * Array access
     */
    BigNumber& operator [] (unsigned int index)
    { return m_coefficients[ index] ; }

    BigNumber const& operator [] (unsigned int index) const
    { return m_coefficients[ index] ; }



  private:

    void push_back( BigNumber big ) ;

    /// coefficients array
    std::vector< BigNumber > m_coefficients ;    

   
    /// modulo 
    //<todo> do we want to store BigNumber not int ?
    int m_modulo ;


    friend std::ostream& operator<<( std::ostream &os, const Polynomial& p ) ; 
};




#endif 