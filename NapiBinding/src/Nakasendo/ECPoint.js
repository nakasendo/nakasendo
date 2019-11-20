const bindings = require('../../build/Release/nakasendo.node');


class ECPoint{
    #m_Value;
    #m_IsDecimal=false;
    #m_IsCompressed = true;
    #m_Nid = 714
    
    constructor(nid=0,IsDecimal=false,isCompressed=true){
        this.#m_IsDecimal = IsDecimal;
        this.#m_IsCompressed = isCompressed;
        if(nid != 0)
            this.#m_Nid = nid;
            
        this.#m_Value = bindings.ec_pt_generate_random(this.#m_Nid,this.#m_IsDecimal,this.#m_IsCompressed);        
    }
    
    set SetValue(value){
        this.#m_Value = value;
    }
    
    set SetDec(val){
        this.#m_IsDecimal = val;
    }
    
    set IsCompressed (val){
        this.#m_IsCompressed = val;
    }
    
    set nid (val){
        this.#m_Nid = val;
    }
    
    
    // Getter
    get value(){
        return this.#m_Value;
    }
    
    
    get isdec(){
        return this.#m_IsDecimal;
    }
    
    
    get isCompressed(){
        return this.#m_IsCompressed;
    }
    
    get nid(){
       return this.#m_Nid;;
    }
    
    
    
    
    toString(){
        return this.#m_Value;
    }
    
    GetGroupOrder(){
        return bindings.ec_pt_GetGroupOrder(this.#m_Nid,this.#m_IsDecimal);
    }
    
    GetGroupDegree(){
        return bindings.ec_pt_GetGroupDegree(this.#m_Nid, this.#m_IsDecimal);
    }
    
    CheckonCurve(){
        return bindings.ec_pt_CheckonCurve(this.#m_Value, this.#m_Nid, this.#m_IsDecimal);
    }
    
    GetAffineCoordinates(){
        var coords = bindings.ec_pt_GetAffineCoordinates(this.#m_Value,this.#m_Nid,this.#m_IsDecimal);
        return coords;
    }
    
}

function AddECPoint (a, b){
    // check the dec flag & mod of each big num
    if (!(a.isdec == b.isdec) || !(a.nid == b.nid) || !(a.isCompressed == b.isCompressed)){
        throw new Error('EC point properties (decimal or nid) not equal');
    }  
    
    var result = bindings.ec_pt_Add(a.value, b.value, a.nid, a.isdec, a.isCompressed);
    // create a return of type ECPoint
    var retval = new ECPoint(a.nid,a.isdec,a.isCompressed);
    retval.SetValue = result;
    return retval;
}

function EC_Compare(a,b){
    if(!(a.isdec == b.isdec) || !(a.nid == b.nid) || !(a.isCompressed == b.isCompressed)){
        throw new Error('EC point properties (decimal or nid) not equal');
    }
    return (bindings.ec_pt_compare(a.value, b.value, a.nid, a.isdec));
}

function EC_Double(a){
   var result = bindings.ec_pt_double(a.value, a.nid, a.isdec, a.isCompressed);
   var retval = new ECPoint(a.nid,a.isdec,a.isCompressed);
   retval.SetValue = result;
   return retval;
}

function EC_Invert(a){
    var result = bindings.ec_pt_invert(a.value, a.nid, a.isdec, a.isCompressed);
    var retval = new ECPoint(a.nid,a.isdec,a.isCompressed);
    retval.SetValue = result;
    return retval;
}

function EC_ScalerMultiply(ecpt,bignum){
    var result = bindings.ec_pt_multiplyScalar(ecpt.value, bignum, ecpt.nid,ecpt.isdec, ecpt.isCompressed);
    var retVal = new ECPoint(ecpt.nid, ecpt.isdec, ecpt.isCompressed);
    retVal.SetValue = result;
    return retVal;   
}


function EC_MultiplyScalerByGenerator(bignum, nid, decimal, compressed){
    var result = bindings.ec_pt_MultiplyByGenerator(bignum, nid,decimal,compressed);
    var retval = new ECPoint(nid,decimal,compressed);
    retval.SetValue = result;
    return retval;
}

 module.exports = {
    ECPoint,
    AddECPoint,
    EC_Compare,
    EC_Double,
    EC_Invert,
    EC_ScalerMultiply,
    EC_MultiplyScalerByGenerator
 }
 






