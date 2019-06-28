/*
 * Base class for polynomials.
 */

#ifndef _NCHAIN_SDK_POLYNOMIAL_H_
#define _NCHAIN_SDK_POLYNOMIAL_H_

#include <BigNumbers/BigNumbers.h>


/// Forward declaration
class Polynomial;

/// Pointer type
using PolynomialSPtr = std::shared_ptr<Polynomial>;

/// Base class for polynomials.
class Polynomial
{
  public:

    /// Default constructor.
    Polynomial() = default;

    /// Default destructor
    virtual ~Polynomial() = default;

    /**
    * Get the degree of the polynomial.
    * @return The degree (order) of the polynomial, or -1 in the special case of
    *         the zero polynomial.
    */
    virtual long getDegree() const = 0;

    /**
    * Evaluate for x.
    * @param x The value to plug in to the polynomial for x.
    * @return The evaluated result of the polynomial.
    */
    virtual BigNumber evaluate(const BigNumber& x) const = 0;

};



#endif
