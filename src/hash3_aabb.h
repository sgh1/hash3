
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef HASH3_AABB_H
#define HASH3_AABB_H

#include <iostream>

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

	~aabb() = default;
	
    my_vect_t m_min;
    my_vect_t m_max;
    

};

}


#endif /* INT3_H */
