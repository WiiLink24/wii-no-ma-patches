#define SECTION(_SECTION) __attribute__((__section__(_SECTION)))
#define LONGCALL __attribute__((__longcall__))
#define PACKED __attribute__((__packed__))

#define _STRIFY1(_VAL) #_VAL
#define STRIFY(_VAL) _STRIFY1(_VAL)

#define AT(_ADDRESS) asm(STRIFY(_ADDRESS))

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;