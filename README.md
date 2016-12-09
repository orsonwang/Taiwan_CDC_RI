# Taiwan_CDC_RI
Reference implementation of Taiwan's citizen digital certificate

This is a clean room implementation of functions communicate with CDC chip card.
Currently we release two functions
1. CDCCheck: Check if this card is a CDC card.
2. CDCGetCert: Read the certification file and return in DER format.

We will put PIN verify later.

2016/12/09 Note
After communicate with management authority,
It is not possible to release PIN verify function as password is protected by SHA256.
But we might be able to support more CDC card at the week of 2016/12/25.
Hope we can get that as Christmas gift.
