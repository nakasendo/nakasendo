#include <string>
#include <vector>

#include <emscripten/bind.h>

#include <Polynomial/Polynomial.h>
#include <BigNumbers/BigNumbers.h>

using namespace emscripten;

std::vector<std::string> wasm_RandPolynomial(const int degree)
{
    BigNumber modulo ;
    Polynomial p = Polynomial( degree, modulo );
    std::vector<std::string> coeffs;
    const size_t nbCoeffs = p.length();
    for(size_t i=0;i<nbCoeffs;++i)
    {
        coeffs.push_back(p[i].ToHex());
    }
    return coeffs;
}

EMSCRIPTEN_BINDINGS(JsPolynomial) {
    function("RandPolynomial", &wasm_RandPolynomial);

    // register bindings for std::vector<std::string>
    register_vector<std::string>("std::vector<std::string>");
}