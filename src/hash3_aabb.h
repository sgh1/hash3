
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef HASH3_AABB_H
#define HASH3_AABB_H

#include "hash3_util.h"
#include "hash3_vector3.h"


#include <iostream>
#include <type_traits>

namespace hash3
{

/*****
*very simple axis-aligned bounding-box template
*add methods as needed
*****/

template <typename T>
class aabb
{
public:

	typedef vector3<T>              my_vect3_t;

    aabb() = default;
	aabb(const aabb& other) = default;

	aabb(const my_vect3_t& mn, const my_vect3_t& mx):
		m_min(mn),m_max(mx)
	{}


    //from http://www.scratchapixel.com/lessons/3d-basic-rendering/
    //minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

    //please note that this intersects an "infinite ray", eg, "two rays"
    //emanate from orig in the direction r.dir and -r.dir from r.origin
    //it is not good for eg starting a ray inside the hash and tracing
    //in one direction only.
	template<typename RAY>
	bool ray_intersect(const RAY& r)
    {
        T tmin = (m_min.x - r.orig.x) / r.dir.x;
        T tmax = (m_max.x - r.orig.x) / r.dir.x;

        if (tmin > tmax)
            std::swap(tmin, tmax);

        T tymin = (m_min.y - r.orig.y) / r.dir.y;
        T tymax = (m_max.y - r.orig.y) / r.dir.y;

        if (tymin > tymax)
            std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            return false;

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        T tzmin = (m_min.z - r.orig.z) / r.dir.z;
        T tzmax = (m_max.z - r.orig.z) / r.dir.z;

        if (tzmin > tzmax)
            std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;

        if (tzmin > tmin)
            tmin = tzmin;

        if (tzmax < tmax)
            tmax = tzmax;

        return true;
    }

	~aabb() = default;


    my_vect3_t m_min;
    my_vect3_t m_max;
};


template <typename T>
struct use_expanded_bb : public std::true_type {	};

template <bool use_expanded>
struct get_bb
{

    using aabb_t = aabb<double>;
    using my_vect3_t = aabb<double>::my_vect3_t;

	static aabb_t get(int3<int> idx, const my_vect3_t& d)
	{

		return aabb_t(  my_vect3_t( bin_min_1d<double>(idx.x,d.x),
                                    bin_min_1d<double>(idx.y,d.y),
                                    bin_min_1d<double>(idx.z,d.z) ),

                        my_vect3_t( bin_max_1d<double>(idx.x,d.x),
                                    bin_max_1d<double>(idx.y,d.y),
                                    bin_max_1d<double>(idx.z,d.z) ) );
	}
};

template <>
struct get_bb<true>
{
    using aabb_t = aabb<double>;
    using my_vect3_t = aabb<double>::my_vect3_t;

	static aabb_t get(int3<int> idx, const my_vect3_t& d)
	{

		return aabb_t(    my_vect3_t(   bin_min_1d_expanded<double>(idx.x,d.x),
                                        bin_min_1d_expanded<double>(idx.y,d.y),
                                        bin_min_1d_expanded<double>(idx.z,d.z) ),

                        my_vect3_t( bin_max_1d_expanded<double>(idx.x,d.x),
                                    bin_max_1d_expanded<double>(idx.y,d.y),
                                    bin_max_1d_expanded<double>(idx.z,d.z) ) );

	}
};





}


#endif /* AABB_H */
