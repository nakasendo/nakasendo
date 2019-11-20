const bindings = require('../../build/Release/nakasendo.node');


class ECLGInterpolator{
    #m_xfx = [];
    #m_IsDecimal=false;
    #m_modulo;
    
    
    constructor(xfx, modulo, IsDecimal=false){
        this.#m_xfx = xfx;
        this.#m_modulo = modulo;
        this.#m_IsDecimal = IsDecimal;      
    }
    
    get xfx(){
        return this.#m_xfx;
    }
    
    get mod(){
        return this.#m_modulo;
    }
    
    get isdec(){
        return this.#m_IsDecimal;
    }
    
    evaluate(xval){
        return bindings.ECLGInterpolatorFull(this.#m_xfx, xval, this.#m_modulo, this.#m_IsDecimal);
    }
    
    
    
}

module.exports = {
    ECLGInterpolator
 }
