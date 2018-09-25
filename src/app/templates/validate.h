#ifndef ASN1SCC_FUZZER_VALIDATE_H
#define ASN1SCC_FUZZER_VALIDATE_H

#include <stdbool.h>

#include "asn1crt.h"

bool validate(const BitStream *stream);

#endif
