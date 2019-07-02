Taiwan CDC RI
---
Reference implementation of Taiwan's citizen digital certificate

This is a clean room implementation of functions communicate with CDC chip card.
Currently we release two functions
1. CDCCheck: Check if this card is a CDC card.
2. CDCGetCert2: Read the certification file(user CA2) and return in DER format.

2016/12/09 Note
After communicate with management authority,
It is not possible to release PIN verify function as password was encrypted. (It's easy to trace the algorithm and key to encrypt, but this will infring CHT's copyright)

2017/02/07 Note
There are four CAs that we can read. user CA1 is for data encryption, user CA2 is for signature. 
These two CAs are the CAs developer need to use,check it out.
