#ifndef PARTICLE_H
#define PARTICLE_H

#include "hash3_vector3.h"

#include <iostream>

//#define PRINT_CTOR

/***
 * myvect3d
 * hash3 allows usage of any 3d vector class
 * that has x,y,z members
 **/

struct myvect3d
{

	myvect3d(double xx, double yy, double zz):
		x(xx),y(yy),z(zz)
	{}

	myvect3d():
		x(0.0),y(0.0),z(0.0)
	{}

	double x,y,z;
};

/***
 * particle
 * simple particle class used for hash3 example
 **/

class particle
{
    public:

    /***
     * to work with hash3, the below needs to be
     * added -- one typedef and the two get_xyz
     * functions. The get_xyz only can return anything
     * that has T.x, T.y, T.z
     **/

    typedef double num_type;
	typedef myvect3d vect_type;

    static const vect_type& get_xyz(const particle& p){
        return p.m_r;
    }

    /***
     * end hash3 boilerplate
     **/

    particle(const vect_type& v, const vect_type& r,
        long id):
        m_r(r),m_v(v),m_idx(id)
    {}

    particle(const particle& x):
        m_r(x.m_r),m_v(x.m_v),m_idx(x.m_idx)
    {
        #ifdef PRINT_CTOR
            std::cout << "particle: copy ctor\n";
        #endif
    }

    particle(particle&& x):
        m_r(x.m_r),m_v(x.m_v),m_idx(x.m_idx)
    {
        #ifdef PRINT_CTOR
            std::cout << "particle: move ctor\n";
        #endif
    }

    virtual ~particle(){};

    vect_type     m_r;
    vect_type     m_v;
    long int    m_idx;

};





#endif // PARTICLE_H
