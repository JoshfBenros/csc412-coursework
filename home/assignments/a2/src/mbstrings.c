#include "mbstrings.h"

/* mbslen - multi-byte string length
 * - Description: returns the number of UTF-8 code points ("characters")
 * in a multibyte string. If the argument is NULL or an invalid UTF-8
 * string is passed, returns -1.
 *
 * - Arguments: A pointer to a character array (`bytes`), consisting of UTF-8
 * variable-length encoded multibyte code points.
 *
 * - Return: returns the actual number of UTF-8 code points in `src`. If an
 * invalid sequence of bytes is encountered, return -1.
 *
 * - Hints:
 * UTF-8 characters are encoded in 1 to 4 bytes. The number of leading 1s in the
 * highest order byte indicates the length (in bytes) of the character. For
 * example, a character with the encoding 1111.... is 4 bytes long, a character
 * with the encoding 1110.... is 3 bytes long, and a character with the encoding
 * 1100.... is 2 bytes long. Single-byte UTF-8 characters were designed to be
 * compatible with ASCII. As such, the first bit of a 1-byte UTF-8 character is
 * 0.......
 *
 * You will need bitwise operations for this part of the assignment!
 */
size_t mbslen(const char* bytes) {
    if (!bytes) {
        return (size_t)-1;
    }

    const unsigned char* p = (const unsigned char*)bytes;
    size_t count = 0;

    while (*p != '\0') {
        size_t need = 0;

        if ((*p & 0x80u) == 0x00u) {
            need = 1;
        } else if ((*p & 0xE0u) == 0xC0u) {
            need = 2;
        } else if ((*p & 0xF0u) == 0xE0u) {
            need = 3;
        } else if ((*p & 0xF8u) == 0xF0u) {
            need = 4;
        } else {
            return (size_t)-1;
        }

        for (size_t i = 1; i < need; i++) {
            if (p[i] == '\0' || (p[i] & 0xC0u) != 0x80u) {
                return (size_t)-1;
            }
        }

        p += need;
        count++;
    }

    return count;
}
