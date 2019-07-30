#include <iostream>
#include <sstream>
#include <map>

#include "Polynomial/LGInterpolator.h"


LGInterpolator::LGInterpolator(const PointsList& points, const BigNumber& modulo)
    : m_Points (points)
    , m_modulo(modulo)
{
    // Sanity check the points.
    if ( m_Points.empty () || m_Points.size () < 2){
        std::stringstream excepStr; 
        excepStr << "Curve points empty or less than 2 is not allowed in the LGInterpolator constructor" ; 
        throw std::runtime_error ( excepStr.str());
    }

    if(has_duplicates(m_Points.begin(), m_Points.end ())){
        std::stringstream excepStr;
        excepStr << "Curve has duplicated points ... please verify the curve" << std::endl;
        throw std::runtime_error (excepStr.str());
    }

    return ;
}

const int LGInterpolator::Degree () const { 
    return int(m_Points.size() - 1);
}

const int LGInterpolator::Length () const {
    return int (m_Points.size());
}

BigNumber LGInterpolator::operator()(const BigNumber& xValue){
    const int n = m_Points.size(); 
    BigNumber k; 
    BigNumber mul;
    k.Zero(); 
    
    for (int i=0;i<n;++i){
        mul.One();
        for(int j=0;j<n;++j){
            if( j!=i){
                try{
                    BigNumber BNNumeratorTerm1 = Sub_mod( xValue, m_Points.at(j).first, m_modulo);                
                    BigNumber BNDenominatorTerm1 = Sub_mod(m_Points.at(i).first, m_Points.at(j).first, m_modulo);
                    BigNumber div = Div_mod(BNNumeratorTerm1,BNDenominatorTerm1,m_modulo);
                    mul = Mul_mod(mul,div,m_modulo);
                }
                catch (std::exception& e){
                    std::cout << e.what() << std::endl;
                    throw;  
                }

            }
        }
        BigNumber completeTerm = Mul_mod(mul, m_Points.at(i).second, m_modulo);
        k = Add_mod (k,completeTerm,m_modulo);       
    }
    return k;
}



std::ostream& operator<<(std::ostream& output, const LGInterpolator& obj){
    for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator testIter = obj.m_Points.begin(); testIter != obj.m_Points.end(); ++ testIter){
        output << "x= " << testIter->first.ToDec() << "\tf(x)= " << testIter->second.ToDec() << std::endl;
    }
    return output;
}

template <typename Iterator>
bool has_duplicates( Iterator first, Iterator last )
{
  std::map <typename std::iterator_traits <Iterator> ::value_type, std::size_t> histogram;

  while (first != last)
    if (++histogram[ *first++ ] > 1) 
      return true;

  return false;
}
