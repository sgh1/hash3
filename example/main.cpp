#include "particle.h"
#include "hash3_hash3.h"

#include <iostream>
#include <random>

void create_hash3_move(std::vector<particle>& particles){

    typedef myvect3d vect3_t;
//    typedef hash3::int3<int>       idx_t;

    hash3::hash3<particle> storage(std::move(particles),
        hash3::hash3<particle>::my_vect3_t(1.,1.,1.));

    storage.insert(particle(    vect3_t(0.0,0.0,0.0 ),
                                vect3_t(2.0,2.0,2.0 ) ,0 ) );

    storage.print(std::cout);

}

void create_hash3_const_ref(const std::vector<particle>& particles){

    typedef myvect3d vect3_t;
  //  typedef hash3::int3<int>       idx_t;

    hash3::hash3<particle> storage(particles,
        hash3::hash3<particle>::my_vect3_t(1.,1.,1.));

    storage.insert(particle(    vect3_t(0.0,0.0,0.0 ),
                                vect3_t(2.0,2.0,2.0 ) ,0 ) );

    particle p( vect3_t(0.0,0.0,0.0 ),
        vect3_t(2.0,2.0,2.0 ) ,0 );

    storage.nearest_neighbor_in_bin(p,
        storage.hash_func(p) );

    storage.nearest_neighbor(particles.back());

    storage.print(std::cout);

    std::cout << storage.total();

}

void create_hash3_ptr(const std::vector<particle>& particles){

    typedef myvect3d vect3_t;

    hash3::hash3<particle*> storage(
        hash3::hash3<particle*>::my_vect3_t(1.,1.,1.));

    storage.insert(new particle(    vect3_t(0.0,0.0,0.0 ),
                                vect3_t(2.0,2.0,2.0 ) ,0 ) );

    storage.print(std::cout);

}

int main( int argc, char* argv[] ){

    typedef myvect3d vect3_t;
//    typedef hash3::int3<int>       idx_t;

    std::vector<particle> particles;
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<double> uniform_dist(0, 10.0);

    for(int i = 0; i < 5000; i++)
    {
        particles.push_back( particle(  vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ),
                                        vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ), i ) );
    }

    create_hash3_const_ref(particles);
    create_hash3_move(particles);


	return 0;

}
