#include "uint512_llvm.hpp"

//forward declaration for uncached LE check below
bool uint512_littleendian_uncached();

//cache LE check value global to this file
const bool isLittleEndian = uint512_littleendian_uncached();

//attempt to check if we use little-endian layout
// construct an integer of exactly 16 bits with the value 1 (00 01 or 01 00 in bytes) (BE and LE respectively)
// read an 8-bit integer from the same address which should be the first byte of the 16-bit integer (00 or 01 respectively)
// if the value is 00, we assume it is Big Endian, otherwise Little Endian
bool uint512_littleendian_uncached()
{
    const uint16_t number = 0x1;
    const void* p = &number;
    const uint8_t* bytePtr = (const uint8_t*) p;
    return (bytePtr[0] != 0);
}


//cached check for endianness
bool uint512_littleendian()
{
    return isLittleEndian;
}

//print debug information about a 512-bit integer and endianness with a prefix name
void uint512_debug_(const char* prefix, const uint512_t& u512){
        uint512_u hash_convert;
        hash_convert.u512 = u512;

        std::cout<<" little-endian? "<<uint512_littleendian()<<std::endl;
        std::cout<<" debug "<<prefix<<" u64[";
        for(int i=0;i<8;i++) std::cout<< hash_convert.u64[i] <<" ";
        std::cout<<"]"<<std::endl;

        std::cout<<" debug "<<prefix<<" u8[";
        for(int i=0;i<64;i++) std::cout<< (unsigned int) hash_convert.u8[i] <<" ";
        std::cout<<"]"<<std::endl;
}

