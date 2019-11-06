const bindings = require('../../build/Release/nakasendo.node');


class BSVAddress{
    #m_key ;
    #m_version;
    #m_address;
    #m_valid;
    #m_decodedAddress;
    #m_prefix;
    #m_NetworkType;
    
    
    constructor(key, version, addressInfo=[]){
        this.#m_key = key;
        this.#m_version = version;
        
        var addr = []
        if(addressInfo.length != 5){
            addr = bindings.createBSVAddress(this.#m_key, this.#m_version);
        }else{
            addr = addressInfo;
        }
              
       this.#m_address = addr[0];
       this.#m_valid = addr[1];
       this.#m_decodedAddress = addr[2];
       this.#m_prefix = addr[3];
       this.#m_NetworkType = addr[4];
    }
    
    
    get address(){
        return this.#m_address;
    }
    
    get decodedAddress(){
        return this.#m_decodedAddress;
    }
    
    get valid(){
        return this.#m_valid;
    }
    
    get prefix(){
        return this.#m_prefix;
    }
    
    get NetworkType(){
        return this.#m_NetworkType;
    }

}

module.exports = {
    BSVAddress
 }
