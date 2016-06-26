
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef HASH3_DET_H
#define HASH3_DET_H

#include "hash3_vector3.h"
#include "hash3_int3.h"

namespace hash3
{

/**
 * ctor_std_vector_get
 * describes how to move Ts in the the hash from the
 * constructor based on whether a const vector<T>& or
 * a vector<T>&& was supplied
 */
template<typename T>
struct ctor_std_vector_get
{
};

template<typename T>
struct ctor_std_vector_get< const std::vector<T>& >
{
    using vector_value_type = T;
    using ref_type = const vector_value_type&;

    static const T& get(const T& x){
        return x;
    }
};

template<typename T>
struct ctor_std_vector_get<std::vector<T>&& >
{
    using vector_value_type = T;
    using ref_type = vector_value_type&;

    static T&& get(T& x){
        return std::move(x);
    }
};


/**
 * bin_type
 * describes what the bucket type will be for
 * a type, T.
*/


template<typename T>
struct bin_type{
    using type =  std::vector<T>;
};

//template<typename T>
//struct bin_type<T*>{
//    using type = std::vector<std::unique_ptr<T>>;
//};




template<typename T>
struct remove_ptr{
    using type = T;
};

template<typename T>
struct remove_ptr<T*>{
    using type = T;
};


template<typename T>
struct get_const_ref
{
    static const T& get(const T& t){
        return t;
    }
};

template<typename T>
struct get_const_ref<T*>
{
    static const T& get(const T* t){

        if(!t){
            throw "nullptr";
        }

        return *t;
    }
};

}


#endif // HISTO3_H
