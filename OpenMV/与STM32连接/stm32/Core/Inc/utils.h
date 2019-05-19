/*
 * utils.h
 *
 *  Created on: 13.07.2018
 *      Author: johae
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <stdint.h>

#ifndef min
# define min(a,b) ( ((a) < (b)) ? (a) : (b) )
#endif

#ifndef max
# define max(a,b) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef clamp
#  define clamp(x, min, max) ( ((x) < (min)) ? (min) : ( ((x) > (max)) ? (max) : (x) ) )
#endif

#ifndef constrain
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif

#ifndef testBits
# define testBits(x, bits) ( ((x) & (bits)) == (bits) )
#endif

#ifndef SWAP_UINT16
#  define SWAP_UINT16(x) (((x) >> 8) | ((x) << 8))
#endif

#ifndef SWAP_UINT32
#  define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
#endif

#endif /* INC_UTILS_H_ */
