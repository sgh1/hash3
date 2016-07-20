#include "particle.h"
#include "hash3_hash3.h"

#include <algorithm>
#include <chrono>
#include <fstream>
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

    hash3::hash3<particle> storage(particles,
        hash3::hash3<particle>::my_vect3_t(1.,1.,1.));

    //storage.insert(particle(    vect3_t(0.0,0.0,0.0 ),
    //                            vect3_t(2.0,2.0,2.0 ) , storage.total() ) );

    particle p( vect3_t(0.0,0.0,0.0 ),
                vect3_t(2.0,2.0,2.0 ), storage.total() );

    storage.nearest_neighbor_in_bin(p);
    storage.nearest_neighbor(particles.back());
    storage.print(std::cout);

    std::cout << "total T's: " << storage.total() << "\n";

    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();

    for(const particle& p : particles){
        storage.nearest_neighbor_naive(p);
    }

    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "\nelapsed time, naive search: " << elapsed_seconds.count() << "s\n";


    start = std::chrono::system_clock::now();
    for(const particle& p : particles){
        storage.nearest_neighbor(p);
    }
    end = std::chrono::system_clock::now();

    elapsed_seconds = end-start;
    std::cout << "elapsed time, hash3 search: " << elapsed_seconds.count() << "s\n";

    auto collide_f = [](particle& p1, particle& p2)
    {
        double d2 = p1.m_r.x*p1.m_r.x +
                    p1.m_r.y*p1.m_r.y +
                    p1.m_r.z*p1.m_r.z;

        if(d2 < 1e-5){
            p1.m_v.x *= -1.0;
            p1.m_v.y *= -1.0;
            p1.m_v.z *= -1.0;
        }
    };

    auto advance_f = [](particle& p, double dt)
    {
        p.m_r.x += dt*p.m_v.x;
        p.m_r.y += dt*p.m_v.y;
        p.m_r.z += dt*p.m_v.z;
    };


    int i = 0;
    double dt = 1e-5;
    std::for_each(storage.begin(), storage.end(),
        [&i,&storage,collide_f,advance_f,dt](particle &p)
        {
            //collide to see if particle changes direction
            storage.for_each_neighbor(p,collide_f);

            //advance the particle
            advance_f(p,dt);
        });

    std::vector<particle> particles_updated =
        storage.aggregate_once();

    std::sort(  particles_updated.begin(),
                particles_updated.end(),
                [](particle& a, particle &b){
                    return a.m_idx < b.m_idx;
                });

    std::ofstream f("particles_hash_updated");
    for( const particle& p : particles_updated)
    {
        f   << std::setw(20) << p.m_idx << " "
            << std::setw(20) << p.m_r.x << " "
            << std::setw(20) << p.m_r.y << " "
            << std::setw(20) << p.m_r.z << " "
            << std::setw(20) << p.m_v.x << " "
            << std::setw(20) << p.m_v.y << " "
            << std::setw(20) << p.m_v.z << "\n";
    }

    f.close();

    //do the same with raw vector

    std::vector<particle> particles_copy(particles);

    for(particle& p1 : particles_copy)
    {
        for(particle& p2 : particles_copy)
        {
            collide_f(p1,p2);
        }

        advance_f(p1,dt);
    }

    std::ofstream f2("particles_n2_updated");
    for( const particle& p : particles_copy)
    {
        f2   << std::setw(20) << p.m_idx << " "
            << std::setw(20) << p.m_r.x << " "
            << std::setw(20) << p.m_r.y << " "
            << std::setw(20) << p.m_r.z << " "
            << std::setw(20) << p.m_v.x << " "
            << std::setw(20) << p.m_v.y << " "
            << std::setw(20) << p.m_v.z << "\n";
    }

    f2.close();




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

    std::vector<particle> particles;
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<double> uniform_dist(0, 10.0);

    for(int i = 0; i < 500; i++)
    {
        particles.push_back( particle(  vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ),
                                        vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ), i ) );
    }

    create_hash3_const_ref(particles);
    //create_hash3_move(particles);


	return 0;

}
