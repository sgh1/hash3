
#include "hash3_vector3.h"
#include "hash3_int3.h"

#ifndef HASH3_DET_H
#define HASH3_DET_H

namespace hash3
{

/**
 * get_xyz
 * describes how to get the spatial position for some T
 */
template<typename T>
struct get_xyz{};

template<>
struct get_xyz<vector3<double>>
{
    static vector3<double>& get(vector3<double>& x ){
        return x;
    }
    static const vector3<double>& get(const vector3<double>& x ){
        return x;
    }
};

/**
 * num_type
 * provides the underlying floating point type
 * of the vector type MY_VECT used in the hash
 */
template<typename T>
struct num_type{};

template<>
struct num_type<vector3<double>>
{
    typedef double type;
};


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

}


#endif // HISTO3_H
