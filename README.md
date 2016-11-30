# Taiwan_CDC_RI
Reference implementation of Taiwan's citizen digital certificate

This is a clean room implementation of functions communicate with CDC chip card.
Currently we release two functions
1. CDCCheck: Check if this card is a CDC card.
2. CDCGetCert: Read the certification file and return in DER format.

We will put PIN verify later. 
