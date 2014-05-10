#ifndef UINT64_CHAR_H
#define UINT64_CHAR_H

#include <string.h>

#include "DgTypes.h"

/*!
 * @ingroup
 *
 * @class uint64_char
 *
 * @brief Wrapper for uint64 and char[8] union
 *
 * @author Frank Hart
 * @date 10/05/2014
 */
class uint64_char
{
public:
    //Constructor/destructor
    uint64_char() { data.i = 0; }
    uint64_char(uint64 i) { data.i = i; }
    uint64_char(const std::string& a_str) { Set(a_str); }
    uint64_char(const char a_chr[], uint32 lng) { Set(a_chr, lng); }
    ~uint64_char() {}

    //Copy operations
    uint64_char(const uint64_char& other){ data.i = other.data.i; }
    uint64_char& operator= (const uint64_char& other) { data.i = other.data.i; return *this; }

    //Comparisons
    bool operator==(const uint64_char& other) const { return data.i == other.data.i; }
    bool operator!=(const uint64_char& other) const { return data.i != other.data.i; }

    //Gets
    uint64 i() const { return data.i; }
    std::string str() const { return std::string(data.c); }
    void CopyToBuffer(char* out) const { memcpy(out, data.c, 8); }

    //Conversion operators
    operator uint64() const { return data.i; }
    operator std::string() const { return std::string(data.c); }

    //Various ways of setting the union.
    void Set(uint64 a_i) { data.i = a_i; }
    void Set(const std::string& a_str)
    {
        data.i = 0;
        memcpy(data.c, a_str.c_str(), (a_str.length()<8) ? a_str.length() : 7);
    }

    void Set(const char a_chr[], uint32 lng)
    {
        data.i = 0;
        memcpy(data.c, a_chr, (lng<8) ? lng : 7);
    }

private:
    union Data
    {
        uint64 i;
        char c[8];
    } data;

};

#endif