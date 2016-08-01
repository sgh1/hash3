

#include "particle_test.h"
#include "scoped_timer.h"

#include "hash3_hash3.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>

namespace particle_test
{

	auto collide_f = [](particle& p1, particle& p2)
    {
        double d2 = (p1.m_r.x-p2.m_r.x)*(p1.m_r.x-p2.m_r.x) +
                    (p1.m_r.x-p2.m_r.y)*(p1.m_r.x-p2.m_r.y) +
                    (p1.m_r.x-p2.m_r.z)*(p1.m_r.x-p2.m_r.z);

        if(d2 < 1e-5){
            p1.m_v.x *= -1.0 + sin(p1.m_v.x)*sin(p1.m_v.x + 1.0);
            p1.m_v.y *= -1.0 + sqrt(sin(p1.m_v.x));
            p1.m_v.z *= -1.0;
        }

        //std::cout << p1.m_idx << " " << p2.m_idx << "\n";
    };

    auto advance_f = [](particle& p, double dt)
    {
        p.m_r.x += dt*p.m_v.x;
        p.m_r.y += dt*p.m_v.y;
        p.m_r.z += dt*p.m_v.z;
    };

	void create_hash3_move(std::vector<particle>& particles)
	{

		typedef myvect3d vect3_t;

		hash3::hash3<particle> storage(std::move(particles),
			hash3::hash3<particle>::my_vect3_t(1.,1.,1.));

		storage.insert(particle(    vect3_t(0.0,0.0,0.0 ),
									vect3_t(2.0,2.0,2.0 ) ,0 ) );

		storage.print(std::cout);

	}



	void create_hash3_const_ref(const std::vector<particle>& particles)
	{

		typedef myvect3d vect3_t;

		hash3::hash3<particle> storage(particles,
			hash3::hash3<particle>::my_vect3_t(2.,2.,2.));

		particle p( vect3_t(0.0,0.0,0.0 ),
					vect3_t(2.0,2.0,2.0 ), storage.size() );

		storage.nearest_neighbor_in_bin(p);
		storage.nearest_neighbor(particles.back());
		storage.print(std::cout);

		std::cout << "total T's: " << storage.size() << "\n";

		{
			scoped_timer timer("\nelapsed time, naive search:");

			for(const particle& p : particles){
				storage.nearest_neighbor_naive(p);
			}
		}

		{
			scoped_timer timer("elapsed time, hash3 search:");

			for(const particle& p : particles){
				storage.nearest_neighbor(p);
			}
		}


		int i = 0;
		double dt = 1e-5;

		{
			scoped_timer timer("hash collision time: ");

			std::for_each(storage.begin(), storage.end(),
				[&i,&storage,collide_f,advance_f,dt](particle &p)
				{
					//collide to see if particle changes direction
					storage.for_each_neighbor(p,collide_f);

					//advance the particle
					advance_f(p,dt);
				});
		}

		std::vector<particle> particles_updated =
			storage.aggregate();

		std::sort(  particles_updated.begin(),
					particles_updated.end(),
					[](particle& a, particle &b){
						return a.m_idx < b.m_idx;
					});

		std::ofstream f("particles_hash_updated");
		for( const particle& p : particles_updated){
			f << p;
		}

		f.close();

		//do the same with raw vector

		std::vector<particle> particles_copy(particles);

		{
			scoped_timer timer("naive collision time: ");

			for(particle& p1 : particles_copy)
			{
				for(particle& p2 : particles_copy){
					collide_f(p1,p2);
				}

				advance_f(p1,dt);
			}
		}


		std::ofstream f2("particles_n2_updated");
		for( const particle& p : particles_copy){
			f2 << p;
		}

		f2.close();



        std::ofstream f3("plot_hash3.py");

        print_hash3_python_plot(storage,f3);

        f3.close();

	}

	void create_hash3_ptr(const std::vector<particle>& particles){

		typedef myvect3d vect3_t;

		hash3::hash3<particle*> storage(
			hash3::hash3<particle*>::my_vect3_t(1.,1.,1.));

		storage.insert(new particle(    vect3_t(0.0,0.0,0.0 ),
										vect3_t(2.0,2.0,2.0 ) ,0 ) );

		storage.print(std::cout);

	}

	void print_hash3_python_plot(const hash3::hash3<particle>& particle_hash, std::ostream& os)
	{
        typedef myvect3d vect3_t;

        auto itr = particle_hash.m_bins.begin();

        while (itr != particle_hash.m_bins.end())
        {
            auto idx = itr->first;
            auto idx2 = idx;
            auto d_vect = particle_hash.m_d;

            idx2.x++;
            idx2.y++;
            idx2.z++;

            if(idx.x < 0){
                idx2.x -= 2;
            }

            if(idx.y < 0){
                idx2.y -= 2;
            }

            if(idx.z < 0){
                idx2.z -= 2;
            }

            decltype(d_vect) p0( d_vect.x*idx.x,d_vect.y*idx.y,d_vect.z*idx.z);
            decltype(d_vect) p1( d_vect.x*idx2.x,d_vect.y*idx2.y,d_vect.z*idx2.z);

            std::stringstream ss;

            ss << "draw_hash.draw_box(ax,["  << p0.x << "," << p0.y << "," << p0.z << "],["
                                            << p1.x << "," << p1.y << "," << p1.z << "])\n";

            os << ss.str();

            itr++;

            for(const auto& t : itr->second){

                std::stringstream scatter_str;
                scatter_str << "draw_hash.draw_particle(ax," << t.m_r.x << "," << t.m_r.y << "," << t.m_r.z << ")\n";
                os << scatter_str.str();
            }
        }

    }


};
