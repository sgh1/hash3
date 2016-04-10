#include "particle.h"
#include "hash3_hash3.h"

#include <iostream>
#include <random>

using namespace std;

int main( int argc, char* argv[] ){

    typedef hash3::vector3<double> vect3_t;
    typedef hash3::int3<int>       idx_t;

    std::vector<particle> particles;

    // Seed with a real random value, if available
    std::random_device r;

    // Choose a random mean between 1 and 6
    std::default_random_engine e1(r());
    std::uniform_real_distribution<double> uniform_dist(0, 10.0);

    for(int i = 0; i < 100; i++)
    {
        particles.push_back( particle(  vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ),
                                        vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ), i ) );
    }

    hash3::hash3<particle> storage(particles, idx_t(10,10,10));

    storage.print(std::cout);


	return 0;

}
