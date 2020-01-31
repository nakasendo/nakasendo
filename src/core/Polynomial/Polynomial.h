#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <Polynomial/PolynomialConfig.h>
#include <vector>
#include <string>
#include <iostream>
#include <BigNumbers/BigNumbers.h>
class ECPoint; 

/// 
class Polynomial_API Polynomial
{
  public:

    //construct an empty polynomial
    Polynomial()=default;
    // construct from BigNumber vector
    Polynomial( std::vector< BigNumber >& coefficients, const BigNumber& modulo );

    // construct from String vector
    Polynomial( std::vector< std::string >& coefficients, const BigNumber& modulo );

    // construct from random numbers
    Polynomial( int degree, const BigNumber& modulo ) ;

    // construct from random numbers, with fixed a_0 term
    Polynomial( int degree, const BigNumber& modulo, const BigNumber& a_0 );
    
    // construct from random numbers, with minimum and maximum range
    Polynomial
      (
          int degree, 
          const BigNumber& modulo,
          const BigNumber& min,
          const BigNumber& max
      );

    // Default destructor
    ~Polynomial() = default ;

    // copy constructor
    Polynomial(const Polynomial&);
    //  assignment operator
    Polynomial& operator=(const Polynomial&);
    /*
    * Get the degree of the polynomial.
    * @return The degree (order) of the polynomial
    */
    int getDegree() const;
    
    /*
     * Array access
     */
    BigNumber const& operator [] (unsigned int index) const;


    /*
     * Get the coefficients of the polynomial.
     * @return coefficients
     */
    const std::vector<BigNumber>& getCoefficients() const;
    
    /*
     * Get the coefficients of the polynomial.
     * @return coefficients
     */
    size_t length() const;

    /*
     * Function
     * Hide the coefficients using generator point
     */
    std::vector< BigNumber > hide( int curveID = 714 ) const;
    std::vector<ECPoint> hideAsPoint( int curveID = 714 ) const;


    /*
     * Function
     * Evaluate the polynomial at value x
     */
    BigNumber operator( )(const BigNumber&) const;

  private:

    /* Check the validity during construction */
    void checkValid( ) ;

    /* Push  BigNumber to end of coefficients, using the modulo (if defined) */
    void pushBack( const BigNumber& ) ;

    /* Generate random numbers */
    void randomBigGenerator
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

public:
    Polynomial_API friend std::ostream& operator<<( std::ostream &os, const Polynomial& p ) ;
};

#endif /* POLYNOMIAL_H */