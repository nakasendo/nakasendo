function GenerateNounceAPI (){
  var res = Module['GenerateNounce'](16);

  var str1 = res;

  return str1;
}

function GenerateKey256API (keyInput, IV){

    var str1 = keyInput.toString();
    var str2 = IV.toString();
    //var res = 'WTF';

    var res = Module['GenerateKey256Test'](str1,str2,32,16);
    return res;

}

function EncryptMessage(message, keyInput, IV){
    var str1 = message.toString();
    var str2 = keyInput.toString();
    var str3 = IV.toString();
    
    var res = Module['EncodeFromWASM'](str1,str2,str3,32,16);
    return res;
}

function DecryptMessage(message, keyInput, IV){
    var str1 = message.toString();
    var str2 = keyInput.toString();
    var str3 = IV.toString();
    
    var res = Module['DecodeFromWASM'](str1,str2,str3,32,16);
    return res;
}

class Car {
  constructor(brand) {
    this.carname = brand;
  }
  present() {
    return "I have a " + this.carname;
  }
}





