
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef HASH3_H
#define HASH3_H

#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <limits>

#include "hash3_vector3.h"
#include "hash3_int3.h"
#include "hash3_detail.h"

namespace hash3
{

template<typename T>
class hash3_base
{
public:

	typedef vector3<double>             my_vect3_t;
	typedef int3<int>                   idx_t;

    using bin_t = typename bin_type<T>::type;

    hash3_base():
		m_d(   	)
	{}

	hash3_base(const my_vect3_t& d):
		m_d(   d )
	{}

	virtual ~hash3_base(){}

	void print(std::ostream& os) const
	{
        for( auto const& bin : m_bins){

            os << "bin: "   << bin.first
               << " has: " << std::setw(6) << bin.second.size()
               << " items \n";
        }
    }


    //get a ref. to a bin at idx
	bin_t& get_bin(const idx_t& idx){
        return m_bins[idx];
	}

	const bin_t& get_bin(const idx_t& idx) const{
        return m_bins[idx];
	}

	bin_t& get_bin(const T& t){
        return m_bins[hash_func(t)];
	}

	const bin_t& get_bin(const T& t) const{
        return m_bins[hash_func(t)];
	}

    //not really vaulable until we hvave a fulll
    //iterator
	auto begin(){
        return m_bins.begin();
	}

	auto end(){
        return m_bins.end();
	}

    //give total Ts in hash
	size_t total() const
	{
	    size_t t = 0;
        for( auto const& bin : m_bins){
            t += bin.second.size();
        }

        return t;
	}

	my_vect3_t  m_d;
	std::map< idx_t, bin_t > m_bins;
};


template<typename T>
class hash3 : public hash3_base<T>
{
    public:

	typedef vector3<double>              my_vect3_t;
	typedef int3<int>                    idx_t;
	typedef typename remove_ptr<T>::type value_t;
	typedef typename value_t::vect_type		vect3_t;
	typedef bin_type<T> 				bin_t;

	using hash3_base<T>::m_d;
	using hash3_base<T>::m_bins;

	struct nearest{
        typename bin_type<T>::type::const_iterator it;
        double  dist;
	};

    hash3():
		hash3_base<T>(   	)
	{}

	hash3(const my_vect3_t& d):
		hash3_base<T>(   d )
	{}

    template<typename U>
	hash3(U&& inp, const my_vect3_t& d):
		hash3_base<T>(   d )
	{
		for( typename ctor_std_vector_get<decltype(inp)>::ref_type t : inp){
		    insert(ctor_std_vector_get<decltype(inp)>::get(t));
        }
	}

	virtual~hash3(){}

    //stupid simple re-hash
	void redistribute()
	{
        std::vector<T> all = aggregate_once();

        for( auto const& bin : m_bins){
            bin.second.clear();
        }

        for( const T& t : all){
          insert(std::move(t));
		}
	}

    //move Ts out of the hash into a vector
	std::vector<T> aggregate_once()
	{
	    std::vector<T> ret;

	    ret.reserve(this->total());

	    for( auto const& bin : m_bins)
        {
            for(const T& t : bin.second){
                ret.push_back(std::move(t));
            }

            bin.second.clear();
        }

        return ret;
	}

    //copy or move a T into the hash
    template <typename U>
	void insert(U&& t)
    {
        idx_t       idx = hash_func(t);
        m_bins[idx].push_back(std::forward<U>(t));
    }

    //get the nearest neighbor to 'test' in bin 'idx'
    //note this may not be the absolute nearest neighbor
    //todo: make const, we need to remove eg m_bins[idx]
    //use this like: nearest_neighbor_in_bin(test) != get_bin(
    //note, nn.dist is really dist2, todo
    nearest nearest_neighbor_in_bin(const T& test, const idx_t& idx)
    {
        nearest nn;
        nn.it = m_bins[idx].end();
        nn.dist = std::numeric_limits<double>::max();

        for(auto it = m_bins[idx].begin(); it != m_bins[idx].end(); it++)
        {
            const T& t = *it;

            //avoid finding self
            if(test == t){
                continue;
            }

            double cur_dist = dist2(
                value_t::get_xyz(get_const_ref<T>::get(t)),
                value_t::get_xyz(get_const_ref<T>::get(test)) );

            if(cur_dist < nn.dist){
                nn.it = it;
                nn.dist = cur_dist;
            }
        }

        return nn;
    }

    nearest nearest_neighbor_in_bin(const T& test){
        return nearest_neighbor_in_bin(test,hash_func(test));
    }

    nearest nearest_neighbor(const T& test)
    {
        idx_t idx = hash_func(test);

        nearest nn;
        nn.it = m_bins[idx].end();
        nn.dist = std::numeric_limits<double>::max();

        if(this->total() == 0){
            return nn;
        }

        //this can be improved.  if we already have
        //a nn in bin idx, we don't need to search
        //bins that are further away, sim. to kd tree
        for(int i = idx.x - 1 ; i < idx.x + 1; i++){
        for(int j = idx.y - 1 ; j < idx.y + 1; j++){
        for(int k = idx.z - 1 ; k < idx.z + 1; k++)
        {
            idx_t idx_cur(i,j,k);
            nearest cur_near = nearest_neighbor_in_bin(
                test,idx_cur);

            if(cur_near.it != this->get_bin(idx_cur).end())
            {
                if(cur_near.dist < nn.dist){
                    nn.it = cur_near.it;
                    nn.dist = cur_near.dist;
                }
            }
        }}}

        //we found something
        if(nn.it != m_bins[idx].end()){
            return nn;
        }

        //resort to O(n) search
        return nearest_neighbor_naive(test);
    }

    nearest nearest_neighbor_naive(const T& test)
    {
        idx_t idx = hash_func(test);

        nearest nn;
        nn.it = m_bins[idx].end();
        nn.dist = std::numeric_limits<double>::max();

        if(this->total() == 0){
            return nn;
        }

        for( auto const& bin : m_bins)
        {
            idx_t idx_cur(bin.first);
            nearest cur_near = nearest_neighbor_in_bin(
                test,idx_cur);

            //or bin.second
            if(cur_near.it != this->get_bin(idx_cur).end())
            {
                if(cur_near.dist < nn.dist){
                    nn.it = cur_near.it;
                    nn.dist = cur_near.dist;
                }
            }
        }

        if(nn.it != m_bins[idx].end()){
            //error;
        }

        return nn;
    }




    idx_t hash_func(const T& t)
    {
        idx_t ret;
        my_vect3_t  r =
            my_vect3_t( value_t::get_xyz(get_const_ref<T>::get(t)) );

        ret.x = r.x / this->m_d.x;
        ret.y = r.y / this->m_d.y;
        ret.z = r.z / this->m_d.z;

        // a / b is int(a) / int(b) for >=, ie,
        //bins are [**,**)
        if(r.x < 0.0){
            ret.x--;
        }

        if(r.y < 0.0){
            ret.y--;
        }

        if(r.z < 0.0){
            ret.z--;
        }

        return ret;
    }
};



}

#endif // HISTO3_H
