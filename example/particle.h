#ifndef PARTICLE_H
#define PARTICLE_H

#include "hash3_vector3.h"

/**
 * particle
 * simple particle class used for hash3 example
 */

class particle
{
    typedef hash3::vector3<double> vect3_t;

    public:

    /**
     * to work with hash3, the below needs to be
     * added -- one typedef and the two get_xyz
     * functions.
     */

    typedef double num_type;

    static const vect3_t& get_xyz(const particle& p){
        return p.m_r;
    }

    static vect3_t& get_xyz(particle& p){
        return p.m_r;
    }

    /**
     * end hash3 boilerplate
     */

    particle(const vect3_t& v, const vect3_t& r,
        long id):
        m_r(r),m_v(v),m_idx(id)
    {}

    virtual ~particle(){};

    vect3_t m_r;
    vect3_t m_v;
    long int    m_idx;

};





#endif // PARTICLE_H
