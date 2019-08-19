#include <iostream>
#include <sstream>
#include <map>

#include "Polynomial/LGInterpolator.h"

template <typename Iterator>
bool has_duplicates( Iterator, Iterator );

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

int LGInterpolator::Degree () const { 
    return int(m_Points.size() - 1);
}

int LGInterpolator::Length () const {
    return int (m_Points.size());
}

BigNumber LGInterpolator::operator()(const BigNumber& xValue){
    const int n = m_Points.size(); 
    BigNumber k; 
    BigNumber mul;
    BigNumber testZero;
    testZero.Zero(); 
    k.Zero(); 
    mul.One(); 

    for (int i=0;i<n;++i){
        mul = evalLi(i, xValue);
        BigNumber completeTerm ;
        (m_modulo ==  testZero) ? completeTerm = mul * m_Points.at(i).second
        : completeTerm = Mul_mod(mul, m_Points.at(i).second, m_modulo);
        
        (m_modulo == testZero) ? k = k + completeTerm : k = Add_mod (k,completeTerm,m_modulo);       

    }
    return k;
}

BigNumber LGInterpolator::operator()(const int& i, const BigNumber& xValue){
    // this operator is to evaluate the ith basis polynomial at xValue. 
    // sanity check the value of i
    if ( i < 0 || i >= m_Points.size()){
        std::stringstream errMsg;
        errMsg << "Invalid basis polynomial requested in LGInterpolator()\t i " << i << " is not between zero and " << m_Points.size () ;
        std::runtime_error err (errMsg.str());
        throw err; 
    }
    return ( evalLi (i, xValue));
}

BigNumber LGInterpolator::evalLi (const int& i, const BigNumber& xValue){
    BigNumber mul;
    mul.One();
    BigNumber testZero;
    testZero.Zero(); 
    for(int j=0;j<m_Points.size();++j){
            if( j!=i){
                try{
                    BigNumber BNNumeratorTerm1; BNNumeratorTerm1.Zero(); 
                    BigNumber BNDenominatorTerm1; BNDenominatorTerm1.One(); 
                    BigNumber div; div.Zero();  
                    (m_modulo == testZero) ?  BNNumeratorTerm1 = xValue - m_Points.at(j).first 
                                            : BNNumeratorTerm1 = Sub_mod( xValue, m_Points.at(j).first, m_modulo);                
                    (m_modulo == testZero) ?  BNDenominatorTerm1 = m_Points.at(i).first - m_Points.at(j).first
                                            : BNDenominatorTerm1 = Sub_mod(m_Points.at(i).first, m_Points.at(j).first, m_modulo);
                    (m_modulo == testZero) ? div =  BNNumeratorTerm1 / BNDenominatorTerm1
                                            : div = Div_mod(BNNumeratorTerm1,BNDenominatorTerm1,m_modulo);
                    (m_modulo == testZero) ? mul = mul * div 
                                            : mul = Mul_mod(mul,div,m_modulo);
                }
                catch (std::exception& e){
                    std::stringstream errStr ; 
                    errStr << "Exception in interpolation of point: " << xValue.ToHex() << " between " 
                            << m_Points.at(i).first.ToHex () << " and " << m_Points.at(j).first.ToHex() << "\t" << e.what(); 
                    
                    std::runtime_error extraExp ( errStr.str());
                    throw extraExp; 
                }
            }
    }
    return mul;
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
