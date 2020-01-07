## Elliptic Curve Maths

TODO : fill in content for elliptic curve math, ecdsa

## Polynomial Maths

TODO : fill in content for polynomial definitions and interpolation

## Key Management

TODO : fill in content for key management

## Secret Key Sharing

TODO : fill in content for key sharing

## Symmetric Encryption

TODO : fill in content encrypt/decrypt

## Threshold Signatures

TODO : fill in content for math and protocol part of the Threshold signature

#### Sequence diagram round 1
```plantuml
@startuml
!pragma teoz true

header TS Messaging Framework
title Round 1 Communications
hide footbox
actor Player_1
participant PlayerClient
control Orchestrator
participant PlayerClient2
actor Player_2

activate Orchestrator
Player_1->PlayerClient : register
& PlayerClient->Orchestrator : register
Player_2->PlayerClient2 : register
& PlayerClient2->Orchestrator : register

Player_1 -> PlayerClient : createGroup
activate PlayerClient
PlayerClient -> Orchestrator : createGroup 
deactivate PlayerClient

Orchestrator -> PlayerClient : << invite >>
& Orchestrator -> PlayerClient2 : << invite >>
activate PlayerClient
activate PlayerClient2

PlayerClient --> Orchestrator : response
deactivate PlayerClient
PlayerClient2 --> Orchestrator : response
deactivate PlayerClient2

... wait for number of players (n - 1) ...

Orchestrator -> PlayerClient : << groupIsSet >> (distribute all ordinals)
& Orchestrator -> PlayerClient2 :  << groupIsSet >>
activate PlayerClient
activate PlayerClient2
PlayerClient -> PlayerClient : label polynomial calculation \n evaluate labels
PlayerClient2 -> PlayerClient2 : label polynomial calculation \n evaluate labels
deactivate PlayerClient
deactivate PlayerClient2

... wait until Actor triggers next part ...

Player_1 -> PlayerClient : createSharePublicKey

activate PlayerClient

PlayerClient -> Orchestrator : createSharePublicKey
deactivate PlayerClient

Orchestrator -> PlayerClient : << requestData >>
& Orchestrator -> PlayerClient2 : << requestData >>
activate PlayerClient
activate PlayerClient2

note right PlayerClient2
    - JVRSS (part 1)
end note

PlayerClient --> Orchestrator : response <<data>>
deactivate PlayerClient
PlayerClient2 --> Orchestrator : response <<data>>
deactivate PlayerClient2

... wait for number of responses (n) ...

Orchestrator -> Orchestrator : combine public information
Orchestrator -> PlayerClient : << listCoefficients >>
& Orchestrator -> PlayerClient2 : << listCoefficients >>
activate PlayerClient
activate PlayerClient2

note right PlayerClient2
    - JVRSS (part 2)
end note

PlayerClient -> PlayerClient : calculate private key share \n verify key share
PlayerClient2 -> PlayerClient2 : calculate private key share \n verify key share

PlayerClient --> Orchestrator : verified secret complete
deactivate PlayerClient
PlayerClient2 --> Orchestrator : verified secret complete
deactivate PlayerClient2

... wait for number of responses (n) ...

Orchestrator -> PlayerClient : << groupIsVerified >>
& Orchestrator -> PlayerClient2 : << groupIsVerified >>

note right Player_1
    - go to round2 sequence diagram -
end note

deactivate Orchestrator

@enduml
```

#### Sequence diagram round 2
```plantuml
@startuml
!pragma teoz true

header TS Messaging Framework
title Round 2 Communications
hide footbox
actor Player_1
participant PlayerClient
control Orchestrator
participant PlayerClient2
actor Player_2

activate Orchestrator

Player_1 -> PlayerClient : presign
activate PlayerClient
PlayerClient -> Orchestrator : presign 
deactivate PlayerClient

group JVRSS
loop "k", "alpha"
Orchestrator -> PlayerClient : << requestData >>
& Orchestrator -> PlayerClient2 : << requestData >>
activate PlayerClient
activate PlayerClient2

PlayerClient -> PlayerClient : create new polynomial \n evaluate labels
PlayerClient2 -> PlayerClient2 : create new polynomial \n evaluate labels


PlayerClient --> Orchestrator : response
deactivate PlayerClient
PlayerClient2 --> Orchestrator : response
deactivate PlayerClient2

... wait for number of responses (n) ...

Orchestrator -> Orchestrator : combine public information
Orchestrator -> PlayerClient : << listCoefficients >>
& Orchestrator -> PlayerClient2 : << listCoefficients >>
activate PlayerClient
activate PlayerClient2


PlayerClient -> PlayerClient :  createSecret \n verification of honesty
PlayerClient2 -> PlayerClient2 : createSecret \n verification of honesty

PlayerClient --> Orchestrator : verified secret complete
deactivate PlayerClient
PlayerClient2 --> Orchestrator : verified secret complete
deactivate PlayerClient2

... wait for number of responses (n) ...

Orchestrator -> PlayerClient : << groupIsVerified >>
& Orchestrator -> PlayerClient2 : << groupIsVerified >>

end
end

PlayerClient -> Orchestrator : collateVW
PlayerClient2 -> Orchestrator : collateVW

... wait for number of responses (n) ...

Orchestrator -> PlayerClient : << sharedVWData >>
& Orchestrator -> PlayerClient2 : << sharedVWData >>
activate PlayerClient
activate PlayerClient2

PlayerClient -> PlayerClient : create ephemeral key
PlayerClient2 -> PlayerClient2 : create ephemeral key
PlayerClient -> Orchestrator : ephemeral key complete
PlayerClient2 -> Orchestrator : ephemeral key complete

... wait for number of responses (n) ...

Orchestrator -> PlayerClient : << All completed >>
& Orchestrator -> PlayerClient2 : << All completed >>

deactivate PlayerClient2

PlayerClient -> Orchestrator : presign (repeat x number times )
deactivate PlayerClient

@enduml
```

#### Sequence diagram round 3
```plantuml
@startuml
!pragma teoz true

header TS Messaging Framework
title Round 3 Communications
hide footbox
actor Player_1
participant PlayerClient
control Orchestrator
participant PlayerClient2
actor Player_2

activate Orchestrator

Player_1 -> PlayerClient : sign <msg> 
activate PlayerClient
PlayerClient -> Orchestrator : sign <msg> 
deactivate PlayerClient

Orchestrator -> PlayerClient : << please sign >>
& Orchestrator -> PlayerClient2 : << please sign >>
activate PlayerClient
activate PlayerClient2

PlayerClient -> PlayerClient : signing, calculate s \npop ephemeral key from list
PlayerClient2 -> PlayerClient2 : signing, calculate s \npop ephemeral key from list

PlayerClient --> Orchestrator : response with s
deactivate PlayerClient
PlayerClient2 --> Orchestrator : response with s
deactivate PlayerClient2

... wait for number of responses (2t+1) ...

Orchestrator -> Orchestrator : combine s information
Orchestrator -> PlayerClient : << send signing information >>
activate PlayerClient

PlayerClient -> PlayerClient : interpolate to create signature

PlayerClient --> Orchestrator : finished
deactivate PlayerClient
@enduml
```