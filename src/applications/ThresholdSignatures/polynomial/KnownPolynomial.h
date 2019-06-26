
/*
 * Implementation of a polynomial created from known coefficients.
 */

#ifndef _NCHAIN_SDK_KNOWN_POLYNOMIAL_H_
#define _NCHAIN_SDK_KNOWN_POLYNOMIAL_H_

#include "Polynomial.h"
#include <vector>
#include <string>
#include <BigNumbers/BigNumbers.h>


/// A class to represent a known polynomial equation.
class KnownPolynomial : public Polynomial
{
    using Base = Polynomial;

  public:

    /// Default constructor.
    KnownPolynomial() = default;

    /**
    * Construct from random coefficients.
    * @param rng A random range generator to use.
    * @param degree Degree (order) of the polynomial to generate.
    * @param min Lower bound for random coefficients.
    * @param max Upper bound for random coefficients.
    */
    KnownPolynomial(size_t degree, const BigNumber& min, const BigNumber& max)
    : Base{}
    {
        // Create random coefficients.
        for(auto i = 0; i <= degree; ++i)
        {   
	    BigNumber bigVal = GenerateRandRange(min, max);
            mCoefficients.push_back(std::move(bigVal));
        }
    }

    /**
    * Construct from supplied coefficients.
    * @param coefficients A list of coefficients with the highest degree term
    *                     first and the free term last. Ie; For the equation
    *                     y = ax^2 + bx + c the coefficents should be in the
    *                     order { a, b, c }.
    */
    KnownPolynomial(const std::vector<BigNumber>& coefficients)
    : Base{}, mCoefficients{coefficients}
    {}

    /**
    * Move construct from supplied coefficients.
    * @param coefficients A list of coefficients with the highest degree term
    *                     first and the free term last. Ie; For the equation
    *                     y = ax^2 + bx + c the coefficents should be in the
    *                     order { a, b, c }.
    */
    KnownPolynomial(std::vector<BigNumber>&& coefficients)
    : Base{}, mCoefficients{std::move(coefficients)}
    {}

    /**
    * Get the coefficients.
    * @return A list of coefficients with the highest degree term first and
    *         the free term last.
    */
    const std::vector<BigNumber>& getCoefficients() const { return mCoefficients; }

    /**
    * Get the degree of the polynomial.
    * @return The degree (order) of the polynomial, or -1 in the special case of
    *         the zero polynomial.
    */
    long getDegree() const override
    {
        return { static_cast<long>(mCoefficients.size()) - 1 };
    }

    /**
    * Evaluate for x.
    * @param x The value to plug in to the polynomial for x.
    * @return The evaluated result of the polynomial.
    */
    BigNumber evaluate(const BigNumber& x) const override
    {
        BigNumber res ;
	res.FromDec(std::to_string(0));

        // Horners method for polynomial evaluation
        for(auto coef = mCoefficients.cbegin(); coef != mCoefficients.cend(); ++coef)
        {   
            res = (res * x) + *coef;
        }

        return res;
    }

  private:

    /// Our list of coefficients.
    std::vector<BigNumber> mCoefficients {};

};

#endif
