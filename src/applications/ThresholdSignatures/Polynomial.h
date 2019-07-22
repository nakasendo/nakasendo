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
    Polynomial( std::vector< BigNumber >& coefficients, const BigNumber& modulo ) ;

    // construct from String vector
    Polynomial( std::vector< std::string >& coefficients, const BigNumber& modulo ) ;

    // construct from random numbers
    Polynomial( int degree, const BigNumber& modulo ) ;

    // construct from random numbers, with fixed a_0 term
    Polynomial( int degree, const BigNumber& modulo, const BigNumber& a_0 ) ;

    // Default destructor
    ~Polynomial() = default ;


    /*
    * Get the degree of the polynomial.
    * @return The degree (order) of the polynomial
    */
    int getDegree() const { return m_coefficients.size() -1 ; }


    /*
     * Function 
     */
    BigNumber operator( )( const BigNumber& ) const ;

       


    /*
     * Array access
     */
    BigNumber& operator [] (unsigned int index)                 { return m_coefficients[ index] ; }
    BigNumber const& operator [] (unsigned int index) const     { return m_coefficients[ index] ; }



  private:

    /* Push BigNumber to end of coefficients, using the modulo (if defined)
     */
    void push_back( BigNumber big ) ;

    /* Evaluate the polynomial at value x
    */
    BigNumber evaluate( const BigNumber& ) const ;

    /* Helper 
    */
    void setup( int degree ) ;

    /* Generate random numbers
     */
    std::vector< BigNumber > randomBigGenerator
    ( 
        int degree, 
        const BigNumber& min, 
        const BigNumber& max 
    ) ;
   

    /// coefficients array
    std::vector< BigNumber > m_coefficients ;    

   
    /// modulo 
    BigNumber m_modulo ;

    BigNumber m_zero = GenerateZero( ) ;


    friend std::ostream& operator<<( std::ostream &os, const Polynomial& p ) ; 
};




#endif 