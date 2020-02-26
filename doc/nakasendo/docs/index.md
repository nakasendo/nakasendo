**Welcome to Nakasendo**<sup>TM</sup>

Nakasendo<sup>TM</sup> provides a mix of high-level cryptographic functions underpinned by the nChain IP patent portfolio as well as by the cryptographic primitives required to implement your new big idea.

## Installation

Download the [Nakasendo<sup>TM</sup>](https://github.com/nakasendo/nakasendo) Installer for your preferred platform.

The best way to run the example code is to define the environment variable **SDKLIBRARIES_ROOT** pointing to the location where Nakasendo<sup>TM</sup> is installed.

## Layout

Nakasendo<sup>TM</sup> is broadly split into 2 sections. One is [high-level functions](HighLevelFunctions.md) that require little to no configuration. The other is cryptographic [primitives](Primitives.md). These are often mission-critical and can be used incorrectly. They require making decisions and having an in-depth knowledge of the cryptographic concepts.

In depth [mathematical definitions](Primitives.md) are given, along with [code samples in JavaScript, Python and C++](Primitives.md). Full [developer setup guides](DeveloperSetup.md) are provided as well as a working example of a [generic Threshold Signature Scheme](HighLevelFunctions.md).

**High-level Functions**

- [Threshold Signatures](HighLevelFunctions.md#ThresholdSignatures)

**Primitives**

- [Big Numbers](Primitives.md#BigNumbers)
- [Asym Keys](Primitives.md#AsymKey)
- [BSV Address](Primitives.md#BSVAddress)
- [Elliptic Curve Points](Primitives.md#ECPoint)
- [Message Hash](Primitives.md#MessageHash)
- [Polynomials](Primitives.md#Polynomial)
- [Secret Split](Primitives.md#SecretSplit)
- [Symmetric Encryption and Decryption](Primitives.md#SymEncDec)
