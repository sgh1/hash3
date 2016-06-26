
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef HASH3_VECT3_H
#define HASH3_VECT3_H

#include "hash3_int3.h"
#include <iostream>
#include "math.h"

namespace hash3
{

/**
 * vector3
 * minimal implementation of 3d vector that
 * we need to help work with spatial coordinates
 * of the hash.  For more complicated vector
 * calculations, this template can't compete perf.-
 * wise with something that uses eg expression
 * templates.
 */


template<typename T>
class vector3
{
public:

    typedef T           num_type;
	typedef vector3<T>  self;

	vector3():
		x(0.0),y(0.0),z(0.0)
	{
	}

	vector3(const T& a, const T& b, const T& c):
		x(a),y(b),z(c)
	{
	}

	template<typename U>
	explicit vector3(const int3<U>& i3):
		x(i3.x),y(i3.y),z(i3.z)
	{
	}

    template<typename U>
	explicit vector3(const U& other):
		x(other.x),y(other.y),z(other.z)
	{
	}

	self operator*(const T& a) const
	{
        return self(x*a, y*a, z*a);
	}
	self operator*(const self& v) const
	{
        	//this cross v
        return self(    y*v.z - z*v.y,
                    -1.*x*v.z + z*v.x,
                        x*v.y - y*v.x);
	}

	self operator+(const self& a) const
	{
        return self( x+a.x, y+a.y, z+a.z);
	}

	self operator-(const self& a) const
	{
        return self( x-a.x, y-a.y, z-a.z);
	}

	self operator/(const self& a) const
	{
        return self( x/a.x, y/a.y, z/a.z);
	}

	template <typename U>
	self& operator=(const U& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	T mag2() const
	{
        return x*x + y*y + z*z;
	}


	T mag() const
	{
        return sqrt( mag2() );
	}

    self unit() const
    {
        T   mag_inv = (1/mag());
        T   xx	= x * mag_inv;
        T   yy	= y * mag_inv;
        T   zz	= z * mag_inv;
        return self(xx,yy,zz);
	}

	T dot(const self& a) const
	{
        return a.x*x + a.y*y + a.z*z;
	}


    T dist2(const self& a) const
	{
        return  (x-a.x)*(x-a.x) +
                (y-a.y)*(y-a.y) +
                (z-a.z)*(z-a.z);
	}

	T dist(const self& a) const
	{
        return sqrt(dist2(a));
	}


	T x,y,z;

};

template<typename T>
vector3<T> operator*(const T& a, const vector3<T>& v)
{
    return vector3<T>(a*v.x,a*v.y,a*v.z);
}

template<typename T>
vector3<T> operator/(const vector3<T>& v, const hash3::int3<int>& idx)
{
    return  vector3<T>( v.x/T(idx.x),
                        v.y/T(idx.y),
                        v.z/T(idx.z)    );
}

template<typename T, typename U>
vector3<T> operator-(const U& u, const vector3<T>& v)
{
    return  vector3<T>( u.x - v.x,
                        u.y - v.y,
                        u.z - v.z );
}


template<typename T>
int3<int> vect32int3(const vector3<T>& v)
{
    return  int3<int>(  int(v.x),
                        int(v.y),
                        int(v.z) );
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const vector3<T>& v3)
{
    os << "(" << v3.x << "," << v3.y << "," << v3.z << ")";
    return os;
}

template<typename V1, typename V2>
auto dist2( const V1& a, const V2& b){

    auto sum = (a.x - b.x)*(a.x - b.x) +
            (a.y - b.y)*(a.y - b.y) +
            (a.z - b.z)*(a.z - b.z);

    return sum;
}



}

#endif
