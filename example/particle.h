#ifndef PARTICLE_H
#define PARTICLE_H

#include "hash3_vector3.h"

class particle
{
    typedef hash3::vector3<double> vect3_t;

    public:


    typedef double num_type;

    static const vect3_t& get_xyz(const particle& p){
        return p.m_r;
    }

    static vect3_t get_xyz(particle& p){
        return p.m_r;
    }

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
