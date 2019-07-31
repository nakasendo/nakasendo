#ifndef __LGINTERPOLATOR_H__
#define __LGINTERPOLATOR_H__

#include <DYNAMIC_LIBRARY_API.hpp>
#include <Polynomial/Polynomial_Api.h>


#include <iostream>
#include <vector>
#include <BigNumbers/BigNumbers.h>

using PointsList = std::vector<std::pair<BigNumber, BigNumber> > ;  
class Polynomial_API LGInterpolator
{
    friend Polynomial_API std::ostream& operator<<(std::ostream&, const LGInterpolator&);
    public:
        LGInterpolator (const PointsList&, const BigNumber& ) ; 
        ~LGInterpolator () = default;

        BigNumber operator()( const BigNumber&);

        int Degree () const ; 
        int Length () const ; 
    private:
        PointsList m_Points ; 
        BigNumber m_modulo;

};

#endif //#ifndef __LGINTERPOLATOR_H__