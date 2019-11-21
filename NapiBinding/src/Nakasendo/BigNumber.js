
const bindings = require('../../build/Release/nakasendo.node');

class BigNumber{
    #m_Value;
    #m_IsDecimal;
    #m_Mod;
    
    constructor(IsDecimal=false,mod=0,size=1024){
        this.#m_Value = bindings.generate_random(size, IsDecimal);
        this.#m_IsDecimal = IsDecimal;
        this.#m_Mod = mod;
        
        
    }
    
    set SetValue(value){
        this.#m_Value = value;
    }
    
    set SetDec(val){
        this.#m_IsDecimal = val;
    }
    
    set SetMod(val){
        this.#m_Mod = val;
    }
    
    // Getter
    get value(){
        return this.#m_Value;
    }
    
    
    get isdec(){
        return this.#m_IsDecimal;
    }
    
    get mod(){
        return this.#m_Mod;
    }
    
    
    toString(){
        return this.#m_Value;
    }
    
}

function AddBigNumber (a, b){
    // check the dec flag & mod of each big num
    if (!(a.isdec == b.isdec) || !(a.mod == b.mod)){
        throw new Error('Big number properties (decimal or mod) not equal');
    }  
    
    var result='0';
    if( a.mod == 0){
        result = bindings.bn_add(a.value.toString(), b.value.toString(), a.isdec);
    }else{
        result =  bindings.bn_add_mod(a.value, b.value, a.mod, a.isdec);
    }
    
    var retVal = new BigNumber (a.isdec, b.mod);
    retVal.SetValue = result;
    return retVal;
    

}

function SubBigNumber (a, b){
    // check the dec flag & mod of each big num
    if (!(a.isdec == b.isdec) || !(a.mod == b.mod)){
        throw new Error('Big number properties (decimal or mod) not equal');
    }  
    
    var result='0';
    if( a.mod == 0){
        result = bindings.bn_sub(a.value.toString(), b.value.toString(), a.isdec);
    }else{
        result = bindings.bn_sub_mod(a.value, b.value, a.mod, a.isdec);
    }
    
    var retVal = new BigNumber (a.isdec, b.mod);
    retVal.SetValue = result;
    return retVal;
    
}

function MulBigNumber (a, b){
    // check the dec flag & mod of each big num
    if (!(a.isdec == b.isdec) || !(a.mod == b.mod)){
        throw new Error('Big number properties (decimal or mod) not equal');
    }  
    
    var result='0';
    if( a.mod == 0){
        result =  bindings.bn_mul(a.value.toString(), b.value.toString(), a.isdec);
    }else{
        result =  bindings.bn_mul_mod(a.value, b.value, a.mod, a.isdec);
    }
    
    var retVal = new BigNumber (a.isdec, b.mod);
    retVal.SetValue = result;
    return retVal;
}


function DivBigNumber (a, b){
    // check the dec flag & mod of each big num
    if (!(a.isdec == b.isdec) || !(a.mod == b.mod)){
        throw new Error('Big number properties (decimal or mod) not equal');
    }  
    
    var result='0';
    
    if( a.mod == 0){
        result = bindings.bn_div(a.value.toString(), b.value.toString(), a.isdec);
    }else{
        result = bindings.bn_div_mod(a.value, b.value, a.mod, a.isdec);
    }
    var retVal = new BigNumber (a.isdec, b.mod);
    retVal.SetValue = result;
    return retVal;
}

function InvBigNumber(a){
    var result = 0 ; 
    if(a.mod == 0){
        var BigNumOne = new BigNumber(a.dec,a.mod);
        BigNumOne.SetValue('1');
        result =  bindings.bn_div(BigNumOne.value,a.value,a.isdec);
        
    }else{
        result = bindings.bn_inv_mod(a.value, a.mod,a.isdec);
    }
    var retVal = new BigNumber (a.isdec, a.mod);
    retVal.SetValue = result;
    return retVal;
}

module.exports = { 
    BigNumber,
    AddBigNumber,
    SubBigNumber,
    MulBigNumber,
    DivBigNumber,
    InvBigNumber
}






