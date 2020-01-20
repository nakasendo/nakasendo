const bindings = require('../../build/Release/nakasendo.node');


class polynomial{
    #m_coeffs = [];
    #m_IsDecimal=false;
    #m_modulo;
    #m_degree;
   
    
    constructor(degree, modulo, IsDecimal=false){
        this.#m_IsDecimal = IsDecimal;
        this.#m_degree = degree;
        this.#m_modulo = modulo;
        
            
        this.#m_coeffs = bindings.randomPolynomial(this.#m_degree, this.#m_modulo, this.#m_IsDecimal);        
    }
    
    
    set poly(inputpoly){this.#m_coeffs = inputpoly;}
    get poly(){return this.#m_coeffs;}
    
    set isdec(val){this.#m_IsDecimal = val;}
    get isdec(){return this.#m_IsDecimal;}
    
    set degree (val){
        this.#m_degree = val;
    }
    
    get degree(){
        return this.#m_degree;
    }
    
    set modulo (val){
        this.#m_modulo = val;
    }
    
    
    get modulo(){
       return this.#m_modulo;
    }
    
    
    toString(){
        return this.#m_coeffs;
    }
    
    evaluate(a){
        return bindings.evalulatePolynomial(this.#m_coeffs, a, this.#m_modulo, this.#m_IsDecimal);
    }
    
}

module.exports = {
    polynomial
 }
