import PyPolynomial
from random import randint

def polynomialEvaluation(coeffeicients, xValue, mod):

    # evaluate polynomial for x
    polynomialFX = PyPolynomial.evaluate(coeffeicients, xValue, mod)
    assert type(polynomialFX) == str, "Test failed"

def test_InitFromList():
    for x in range(10, 20):

        fx = str(x)
        modulo = str(x + 1)
        listCoefficients = []

        # Generate random coefficients for the polynomial
        for i in range(x):
            listCoefficients.append(str(randint(1, 10)))

        # create a Polynomial from a list of coefficients
        allCoeffeicient = PyPolynomial.initFromList(listCoefficients)
        assert len(allCoeffeicient) == x, "Test failed"

        # Calling evaluate polynomial function
        polynomialEvaluation(listCoefficients, fx, modulo)

def test_InitFromListModulo():  # Function verifed using https://www.alpertron.com.ar/POLFACT.HTM

    for x in range(10, 30):

        fx = str(x)
        modulo = str(x + 3)
        degree = x

        # Generate random coefficients with degree and modulo for the polynomial
        listCoefficients = PyPolynomial.randomPolynomial(degree, modulo)
        assert len(listCoefficients) == (degree + 1), "Test failed"

        # create a Polynomial from a list of coefficient with modulo
        coefficientsWithModulo = PyPolynomial.initFromListModulo(listCoefficients, modulo)
        assert len(coefficientsWithModulo) == x + 1, "Test failed"

        # calling evaluate polynomial function
        polynomialEvaluation(coefficientsWithModulo, fx, modulo)

def test_RandomPolynomial():

    for x in range(100, 200):
        fx = str(x)
        degree = x
        modulo = str(x + 1)

        # create a random polynomial
        listCoefficients = PyPolynomial.randomPolynomial(degree, modulo)
        assert len(listCoefficients) == (degree + 1), "Test failed"

        # calling evaluate polynomial function
        polynomialEvaluation(listCoefficients, fx, modulo)

def test_RandomPolynomialFixed_a_0():

    for x in range(100, 200):
        fx = str(x - 5)
        degree = x
        modulo = str(x + 2)
        a_0 = str(x)

        # create a random polynomial with fixed a_0
        listCoefficients = PyPolynomial.randomPolynomialFixed_a_0(degree, modulo, a_0)
        assert listCoefficients[0] == a_0, "Test failed"
        assert len(listCoefficients) == (degree + 1), "Test failed"

        # calling evaluate polynomial function
        polynomialEvaluation(listCoefficients, fx, modulo)

def test_RandomPolynomialMinMax():

    for x in range(10, 30):
        fx = str(x + 5)
        degree = x
        modulo = str(x + 2)
        min = str(randint(1, 30))
        max = str(randint(31, 60))

        # create a random polynomial with range of min..max
        listCoefficients = PyPolynomial.randomPolynomialMinMax(degree, modulo, min, max)
        assert len(listCoefficients) == (degree + 1), "Test failed"

        # calling evaluate polynomial function
        polynomialEvaluation(listCoefficients, fx, modulo)

def test_LGInterpolatorFull():

    listTupleObj = [(2, "10"), (3, "15")]
    modulo = "0"

    for x in range(100, 200):

        xValue = str(x)
        # LGInterpolator, full evaluation of the polynomial at xValue
        lgInterpolatorX = PyPolynomial.LGInterpolatorFull(listTupleObj, modulo, xValue)
        assert lgInterpolatorX == str(x * 5), "Test failed"

def test_LGInterpolatorSingle():

    listTupleObj = [(1, "13"), (2, "4"), (3, "2"), (4, "5"), (5, "11"), (6, "1")]
    modulo = "17"
    xValue = "400"

    for x in range(1, 6):
        xPoint = str(x)
        # LGInterpolator, evaluate the ith basis polynomial at xValue
        lgInterpolatorX = PyPolynomial.LGInterpolatorSingle(listTupleObj, modulo, xValue, xPoint)
        # print(type(lgInterpolatorX))
        assert type(lgInterpolatorX) == str, "Test failed"