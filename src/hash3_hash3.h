
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

	void print(std::ostream& os)
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

	auto begin(){
        return m_bins.begin();
	}

	auto end(){
        return m_bins.end();
	}

    //give total Ts in hash
	size_t total()
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

	typedef vector3<double>             my_vect3_t;
	typedef int3<int>                   idx_t;
	typedef typename T::vect_type		vect3_t;
	typedef bin_type<T> 				bin_t;

	using hash3_base<T>::m_d;
	using hash3_base<T>::m_bins;

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
        my_vect3_t  r = T::get_xyz(t) - my_vect3_t(0.0,0.0,0.0);
        idx_t       idx = vect32int3(r / m_d);

        m_bins[idx].push_back(std::forward<U>(t));
    }
};



//partial specialization for T*.  We do some things
//a little differently here.


template<typename T>
class hash3<T*>: public hash3_base<T*>
{
    //we must use T* below.
    public:

	typedef vector3<double>             my_vect3_t;
	typedef int3<int>                   idx_t;
	typedef typename T::vect_type		vect3_t;
	typedef bin_type<T*>                bin_t;

    using hash3_base<T*>::m_d;
	using hash3_base<T*>::m_bins;

    hash3():
		hash3_base<T*>( )
	{}

	hash3(const my_vect3_t& d):
		hash3_base<T*>(   d )
	{}

	//we need another ctor here, for a vector...
	//...of something, T*, const T*, ptr_type<T>?

	virtual ~hash3(){}

    //stupid simple re-hash
	void redistribute()
	{
        bin_t all = aggregate_once();

        for( auto const& bin : m_bins){
            bin.second.clear();
        }

        for( const T& t : all){
          insert(std::move(t));
		}
	}

    //move Ts out of the hash into a vector
	bin_t aggregate_once()
	{
        bin_t ret;

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

    //copy a T into the hash
    void insert(const T& t)
    {
        my_vect3_t  r = T::get_xyz(t) - my_vect3_t(0.0,0.0,0.0);
        idx_t       idx = vect32int3(r / m_d);

        m_bins[idx].push_back(
            std::move( std::unique_ptr<T>(new T(t)) ) );
    }

    //move a unique_ptr<T> into the hash
	void insert(std::unique_ptr<T>&& t)
    {
        my_vect3_t  r = T::get_xyz(t) - my_vect3_t(0.0,0.0,0.0);
        idx_t       idx = vect32int3(r / m_d);

        m_bins[idx].push_back(std::move(t));
    }

};

}

#endif // HISTO3_H
