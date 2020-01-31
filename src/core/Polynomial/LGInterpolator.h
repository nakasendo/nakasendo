#ifndef __LGINTERPOLATOR_H__
#define __LGINTERPOLATOR_H__

#include <DYNAMIC_LIBRARY_API.hpp>
#include <Polynomial/PolynomialConfig.h>


#include <iostream>
#include <vector>
#include <BigNumbers/BigNumbers.h>
#include <ECPoint/ECPoint.h>

using PointsList = std::vector<std::pair<BigNumber, BigNumber> > ;  

using ECPointsList = std::vector<std::pair<BigNumber, ECPoint> > ; 

/*
The LGInterpolator class performs interpolation on a curve made up of (x,y) points
where x & y are big integers (or in our case BigNumbers)
*/
class Polynomial_API LGInterpolator
{
    Polynomial_API friend std::ostream& operator<<(std::ostream&, const LGInterpolator&);
    public:
        LGInterpolator (const PointsList&, const BigNumber& ) ; 
        ~LGInterpolator () = default;

        BigNumber operator()( const BigNumber&);
        BigNumber operator()(const int&, const BigNumber&);


        int Degree () const ; 
        int Length () const ;            
    private:
        PointsList m_Points ; 
        BigNumber m_modulo;
};

/*
The LGECInterpolator class performs interpolation on a curve made up of (x,y) points
where x is a BigNumber & y is a curve point (in our case an ECPoint type)
*/

class Polynomial_API LGECInterpolator
{
    Polynomial_API friend std::ostream& operator<<(std::ostream&, const LGECInterpolator&);
    public:
        LGECInterpolator (const ECPointsList&, const BigNumber& ) ; 
        ~LGECInterpolator () = default;

        ECPoint operator()( const BigNumber&);
        BigNumber operator()(const int&, const BigNumber&);


        int Degree () const ; 
        int Length () const ; 
    private:
        ECPointsList m_Points ; 
        BigNumber m_modulo;
};

#endif //#ifndef __LGINTERPOLATOR_H__