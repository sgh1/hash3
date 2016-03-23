#ifndef PARTICLE_H
#define PARTICLE_H

#include "x_vector3.h"
#include "defs.h"

class particle
{
    public:
        particle(const vector3<DATA>& v, const vector3<DATA>& r,
            long m_id);
        virtual ~particle();

    vector3<DATA> m_r;
    vector3<DATA> m_v;

    long int    m_idx;

};

#endif // PARTICLE_H
