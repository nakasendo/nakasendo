import json

class PolynomialVals:

    def __init__(self):
        self.polynomialFunctions = [] # list of tuples, where each tuple is of (byUrl, forUrl, polyFuncVal)

    def addPolynomialFunction(self, byUrl, forUrl, polyVal):

        # if the byUrl and forUrl found, remove and add
        for polyTuple in self.polynomialFunctions:
            if (byUrl == polyTuple[0] and forUrl == polyTuple[1]):
                self.polynomialFunctions.remove(polyTuple)
                break

        self.polynomialFunctions.append((byUrl, forUrl, polyVal))

    def removePolynomialFunction(self, byUrl, forUrl, polyVal=None):

        for polyTuple in self.polynomialFunctions:
            if (polyVal != None):
              if (byUrl == polyTuple[0] and forUrl == polyTuple[1] and polyVal == polyTuple[2]):
                self.polynomialFunctions.remove(polyTuple)
                break
            else:
              if (byUrl == polyTuple[0] and forUrl == polyTuple[1]):
                self.polynomialFunctions.remove(polyTuple)
                break

    def clearPolynomialFunction(self):
        self.polynomialFunctions.clear()

    def getPolynomialFunction(self):
        return self.polynomialFunctions

    def __str__(self):
        val = ""
        for polyTuple in self.polynomialFunctions:
            val += str(polyTuple[0]) + ", " + str(polyTuple[1]) + ", " + str(polyTuple[2]) + "\n\n"
        return val

    def to_json(self):
        polynomialsJson = []
        for polyTuple in self.polynomialFunctions:
            polynomialsJson.append(json.dumps(polyTuple))
        return json.dumps(polynomialsJson)

    def from_json(self, polynomialsJson):
        polynomials = json.loads(polynomialsJson)
        for polyVal in polynomials:
          # get into List
          polyValList = json.loads(polyVal)
          self.addPolynomialFunction(polyValList[0], polyValList[1], int(polyValList[2])) 


class GroupSecretPolynomialVals:

    def __init__(self, url=None):
        self.myUrl = url
        self.polynomialFunctionsForTheGivenUrl = PolynomialVals()

    def addPolynomialFunction(self, byUrl, forUrl, polyVal):
        if (self.myUrl == forUrl):
            self.polynomialFunctionsForTheGivenUrl.addPolynomialFunction(byUrl, forUrl, polyVal)

    def removePolynomialFunction(self, byUrl, forUrl, polyVal=None):
            self.polynomialFunctionsForTheGivenUrl.removePolynomialFunction(byUrl, forUrl, polyVal)

    def clearPolynomialFunction(self):
        self.polynomialFunctionsForTheGivenUrl.clearPolynomialFunction()

    def getPolynomialFunction(self):
        return self.polynomialFunctionsForTheGivenUrl.getPolynomialFunction()

    def __str__(self):
        val = ""
        val += str(self.myUrl)  + "\n"
        val += str(self.polynomialFunctionsForTheGivenUrl) 
        return val

    def to_json(self):
        grpSrtUrlPolys = {}
        grpSrtUrlPolys['toPlayer'] = self.myUrl
        grpSrtUrlPolys['polynomials'] = self.polynomialFunctionsForTheGivenUrl.to_json()
        return json.dumps(grpSrtUrlPolys)

    def from_json(self, urlPolynomialsJsonStr):
        urlPolynomials = json.loads(urlPolynomialsJsonStr)
        self.myUrl = urlPolynomials['toPlayer']
        self.polynomialFunctionsForTheGivenUrl.from_json(urlPolynomials['polynomials'])

