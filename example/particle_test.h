
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef PARTICLETEST_H
#define PARTICLETEST_H

#include "particle.h"

#include <vector>

namespace particle_test
{
	void create_hash3_move(std::vector<particle>& particles);
	void create_hash3_const_ref(const std::vector<particle>& particles);
	void create_hash3_ptr(const std::vector<particle>& particles);

};

#endif
