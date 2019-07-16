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
 
    // construct empty
    Polynomial( ) = default ;

    // <todo> Do we want to use move semantics here?
    //Polynomial( stsd::vector< BigNumber >&& coefficients, BigNumber groupModulo )


    // construct from BigNumber vector
    Polynomial( std::vector< BigNumber >&, const BigNumber& ) ;

    // construct from String vector
    Polynomial( std::vector< std::string >&, const BigNumber& ) ;

    // Default destructor
    ~Polynomial() = default ;


    /*
    * Get the degree of the polynomial.
    * @return The degree (order) of the polynomial
    */
    int getDegree() { return m_coefficients.size() -1 ; }


    /*
     * Function 
     */
    BigNumber operator( )( const BigNumber& ) ;

       


    /*
     * Array access
     */
    BigNumber& operator [] (unsigned int index)                 { return m_coefficients[ index] ; }
    BigNumber const& operator [] (unsigned int index) const     { return m_coefficients[ index] ; }



  private:

    /* Push BigNumber to end of coefficients, using the modulo (if defined)
     */
    void push_back( BigNumber big ) ;

    /*
    * Evaluate the polynomial at value x
    */
    BigNumber evaluate( const BigNumber& )  ;
   

    /// coefficients array
    std::vector< BigNumber > m_coefficients ;    

   
    /// modulo 
    BigNumber m_modulo ;


    friend std::ostream& operator<<( std::ostream &os, const Polynomial& p ) ; 
};




#endif 