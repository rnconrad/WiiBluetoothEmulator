#ifndef WM_CRYPTO_H
#define	WM_CRYPTO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

uint8_t ft[8];
uint8_t sb[8];

void generate_tables();

#ifdef	__cplusplus
}
#endif

#endif	/* WM_CRYPTO_H */
