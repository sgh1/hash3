
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef PARTICLETEST_H
#define PARTICLETEST_H

#include "hash3_hash3.h"

#include "particle.h"


#include <iosfwd>
#include <vector>

namespace particle_test
{
	void create_hash3_move(std::vector<particle>& particles);
	void create_hash3_const_ref(const std::vector<particle>& particles);
	void create_hash3_ptr(const std::vector<particle>& particles);

	void print_hash3_python_plot(const hash3::hash3<particle>& particles, std::ostream& os);


};

#endif
