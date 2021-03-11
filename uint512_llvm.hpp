#pragma once
#include <cstdint>
#include <iostream>

/*

Comparison of standard types with uint512_t

[                  64 bytes                                    ]        (1x uint512_t)
[              32              ][              32              ]        (2x uint256_t)
[     16       ][     16       ][     16       ][     16       ]        (4x uint128_t)
[   8  ][   8  ][   8  ][   8  ][   8  ][   8  ][   8  ][   8  ]        (8x uint64_t)
1111111111111111111111111111111111111111111111111111111111111111        (64 uint8_t / char)

*/

extern "C"{

//define uint512_t with llvm inbuilt fixed-width integer support
//see also: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2472.pdf
//see also: https://blog.llvm.org/2020/04/the-new-clang-extint-feature-provides.html
typedef unsigned _ExtInt(512) uint512_t;

//internal macros for simplifying defines
#define _PROMOTE512(a) ((uint512_t)a)
#define _LSHIFT512(a,s) _PROMOTE512(_PROMOTE512(a)<<s)
#define _RSHIFT512(a,s) _PROMOTE512(_PROMOTE512(a)>>s)

//macro for constructing a 512-bit value from 8x uint64_t values
#define _UINT64_TO_512(a,b,c,d, e,f,g,h) ( \
_LSHIFT512(a, 64*7) | \
_LSHIFT512(b, 64*6) | \
_LSHIFT512(c, 64*5) | \
_LSHIFT512(d, 64*4) | \
_LSHIFT512(e, 64*3) | \
_LSHIFT512(f, 64*2) | \
_LSHIFT512(g, 64*1) | \
_LSHIFT512(h, 64*0) )

#if defined(_MSC_VER)
#define U512_INLINE __inline
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
#define U512_INLINE inline
#elif defined(__GNUC__)
#define U512_INLINE __inline__
#else
#define U512_INLINE
#endif

//construct a 512-bit integer with an explicit function call (from 8x uint64_t's)
static constexpr uint512_t uint512(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h){
	return _UINT64_TO_512(a,b,c,d,e,f,g,h);
}

//construct a 512-bit integer with an explicit function call (widen 1x uint64_t)
static constexpr uint512_t uint512(uint64_t n){
	return _PROMOTE512(n);
}

//union for accessing divisions of 512-bit integers (64-bit and 8-bit divisions)
//WARNING: the order of these divisions (u64,u8) is undefined and is determined by the system's endianness.
//if you need a reliable [one-way] conversion to parts, use the uint512_u_be(...) function
typedef union uint512_u{
	uint512_t u512;
	uint64_t u64[8];
	uint8_t u8[64];
} uint512_u;

//utility method for detecting little-endian systems. This is a run-time check for endianness if not optimized-out.
//the return value is cached after internal globals are initialized
bool uint512_littleendian();

//construct 'uint512_u' union from a 512-bit integer, but divisions (u64,u8) will be forced to be
// Big-Endian order.
//WARNING: the order of the 512-bit value (u512) is undefined and is determined by the system's endianness.
// this can only be used as a one-way conversion to parts
// if you need to convert back to uint512, consider using the uint512(...) function.
U512_INLINE uint512_u uint512_u_be(uint512_t u512){
	uint512_u value_be;
	if(uint512_littleendian()){
		uint512_u value_le;
		value_le.u512 = u512;
		for(int i=0;i<64;i++) value_be.u8[i] = value_le.u8[63-i];
	}else{
		value_be.u512 = u512;
	}
	return value_be;
}

//internal debug method which prints all divisions of a 512-bit integer, detected endianness.
//uint512_debug(...) is used as a public-facing macro which only operates in debug builds.
void uint512_debug_(const char* prefix, const uint512_t& u512);

//disables debug macro calls if NDEBUG is defined
#ifdef NDEBUG
#define uint512_debug(a,b) ;
#else
#define uint512_debug(a,b) uint512_debug_(a,b);
#endif


}
