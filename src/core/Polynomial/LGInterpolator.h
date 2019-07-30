#ifndef __LGINTERPOLATOR_H__
#define __LGINTERPOLATOR_H__

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_LGInterpolator
#    define LGInterpolator_API EXPORT_DYNAMIC_LIBRARY
#else
#    define LGInterpolator_API IMPORT_DYNAMIC_LIBRARY
#endif

#include <iostream>
#include <vector>
#include <BigNumbers/BigNumbers.h>

using PointsList = std::vector<std::pair<BigNumber, BigNumber> > ;  
class LGInterpolator_API LGInterpolator
{
    friend LGInterpolator_API std::ostream& operator<<(std::ostream&, const LGInterpolator&);
    public:
        LGInterpolator (const PointsList&, const BigNumber& ) ; 
        ~LGInterpolator () = default;

        BigNumber operator()( const BigNumber&);

        const int Degree () const ; 
        const int Length () const ; 
    private:
        PointsList m_Points ; 
        BigNumber m_modulo;

        void CheckForDuplicates(); 


};

template <typename Iterator>
bool LGInterpolator_API has_duplicates( Iterator, Iterator);
#endif //#ifndef __LGINTERPOLATOR_H__