
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
 * default_bdry
 * any struct that def's check_hi or check_low can be
 * implemented to decide what to do when a T escapes
 * the domain of the spatial hash. This one returns
 * false if the T is outside of the domain
 */
struct default_bdry
{
    typedef int3<int>                   idx_t;

    static bool check_hi(const idx_t& idx, const idx_t& sz){
        if(idx.any_greater_than(sz))
        {
            return false;
        }
        return true;
    }

    static bool check_lo(const vector3<double>& r,
        const vector3<double>& p0){
        if(r.x < p0.x || r.y < p0.y || r.z < p0.z)
        {
            return false;
        }
        return true;
    }
};


/**
 * compare
 * describes how to compare two T's for equality
 */
template<typename T>
struct compare
{
    bool operator()(const T& a, const T& b){
        return true;
    }
};



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


}


#endif // HISTO3_H
