
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef PARTICLE_H
#define PARTICLE_H

#include "hash3_vector3.h"

#include <iomanip>
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

    particle& operator=(const particle& other){

        // check for self-assignment
        if(&other == this)
            return *this;

        m_r = other.m_r;
        m_v = other.m_v;
        m_idx = other.m_idx;

        return *this;
    }

    bool operator==(const particle& other) const{
        return m_idx == other.m_idx;
    }

    friend std::ostream& operator<<(std::ostream& os, const particle& p)
    {

        os   << std::setw(20) << p.m_idx << " "
            << std::setw(20) << p.m_r.x << " "
            << std::setw(20) << p.m_r.y << " "
            << std::setw(20) << p.m_r.z << " "
            << std::setw(20) << p.m_v.x << " "
            << std::setw(20) << p.m_v.y << " "
            << std::setw(20) << p.m_v.z << "\n";

        return os;
    }

    virtual ~particle(){};

    vect_type     m_r;
    vect_type     m_v;
    long int    m_idx;

};

class particle_derived : public particle
{
    public:

    particle_derived(const vect_type& v, const vect_type& r,
        long id):
        particle(v,r,id)
    {}

    particle_derived(const particle_derived& x):
        particle(x)
    {
        #ifdef PRINT_CTOR
            std::cout << "particle derived: copy ctor\n";
        #endif
    }

    particle_derived(particle_derived&& x):
        particle(std::move(x))
    {
        #ifdef PRINT_CTOR
            std::cout << "particle derived: move ctor\n";
        #endif
    }


    virtual ~particle_derived(){};

};



#endif // PARTICLE_H
