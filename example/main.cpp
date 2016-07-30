#include "particle_test.h"

#include <iostream>
#include <random>


int main( int argc, char* argv[] ){

    typedef myvect3d vect3_t;

    std::vector<particle> particles;
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<double> uniform_dist(0, 10.0);

    for(int i = 0; i < 50; i++)
    {
        particles.push_back( particle(  vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ),
                                        vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ), i ) );
    }

    particle_test::create_hash3_const_ref(particles);
    //create_hash3_move(particles);

    particle_test::create_hash3_ptr(particles);


	return 0;

}
