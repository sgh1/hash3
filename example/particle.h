#ifndef PARTICLE_H
#define PARTICLE_H

#include "hash3_vector3.h"

class particle
{
    typedef hash3::vector3<double> vect3_t;

    public:
    particle(const vect3_t& v, const vect3_t& r,
        long id):
        m_v(v),m_r(r),m_idx(id)
    {}

    virtual ~particle(){};

    vect3_t m_r;
    vect3_t m_v;
    long int    m_idx;

};

#endif // PARTICLE_H
