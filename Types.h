#pragma once

//8-bit data types
typedef unsigned char uint8;
typedef unsigned char u8;
typedef signed char int8;
typedef signed char i8;

//16-bit data types
typedef unsigned short uint16;
typedef unsigned short u16;
typedef signed short int16;
typedef signed short i16;

//32-bit data types
typedef unsigned int uint32;
typedef unsigned int u32;
typedef signed int int32;
typedef signed int i32;
typedef float float32;
typedef float f32;

//64-bit data types
typedef unsigned long long int uint64;
typedef unsigned long long int u64;
typedef signed long long int int64;
typedef signed long long int i64;
typedef double float64;
typedef double f64;

#ifdef __x86_64__

typedef uint64 uArchInt;
typedef int64 sArchInt;

#else

typedef uint32 uArchInt;
typedef int32 sArchInt;

#endif

//NOTE(Tiago): clang format is dogshit and for some reason it shits itself with
//a class to an r-value not allowing any configuration of the style. To avoid this
//we created here our own move implementation that casts to T&&. This way we can
//use move without having to turn off the linter all over the place. Ideally
//clang format would not be shit and would have this a configurable option, however
//after spending much time looking into this it does not seem like such an option
//exists.
// clang-format off

//NOTE(Tiago): for the love of god can this dumb language get nothing right? because
//of the way they handle template argument deductions with references we have to do 
//this hackery to make sure we get a reference to a reference on instantiation... 
//so that we can properly convert to an r-value. ffs what a mess
template<typename T> struct remove_reference      { typedef T type; };
template<typename T> struct remove_reference<T&>  { typedef T type; };
template<typename T> struct remove_reference<T&&> { typedef T type; };

template<typename T> inline typename remove_reference<T>::type&& rmove(T&& param) noexcept 
{
    return static_cast<typename remove_reference<T>::type&&>(param);
}
// clang-format on
