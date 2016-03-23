
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits>

#include "hash3_vector3.h"
#include "hash3_int3.h"
#include "hash3_detail.h"

#ifndef HISTO3_H
#define HISTO3_H

namespace hash3
{

template<
	typename T,
	typename BDRY_PLCY = hash3_default_bdry,
>
class hash3
{

    public:

	typedef std::vector<T> 				bin_t;
	typedef std::size_t 				idx_t;
	typedef typename num_type<T>::type 	num_t;

    hash3():
	    m_sz(	),
		m_p0(   ),
		m_d(   	)
	{
        alloc();
	}

	hash3(const MY_VECT& p0, const MY_VECT& p1, int3 sz):
	    m_sz(	sz),
		m_p0(   p0),
		m_d(   (p1 - p0 ) / sz )
	{
        alloc();
	}

	hash3(const std::vector<T>& inp, int3 sz):
		m_sz(	sz)
	{
		alloc();

		num_t big = std::numeric_limits<num_t>::max();
		num_t small = std::numeric_limits<num_t>::min();

		MY_VECT min_pt( big, big, big );
		MY_VECT max_pt( small, small, small);

		for( const T& t : inp)
		{
		    const MY_VECT& cur_vect = get_xyz<T>::get(t);

            min_pt.min_of(cur_vect);
            max_pt.max_of(cur_vect);
		}

		m_d = (max_pt - min_pt) / m_sz;
		m_p0 = min_pt;

		for( const T& t : inp){
		    insert(t);
        }
	}


	~hash3(){}

	void print(std::ostream& os)
	{
	    for(size_t i = 0; i < m_isz; i++){
            for(size_t j = 0; j < m_jsz; j++){
                for(size_t k = 0; k < m_ksz; k++){

					int3 idx(i,j,k);
                    bin_t& b = get_bin(idx);
                    os << "bin: "   << idx
                       << " has: " << std::setw(6) << b.size()
                       << " items \n";
                }
            }
	    }
	}

	void redistribute()
	{
        std::vector<T> all = aggregate_once();

        for( bin_t& bin : m_bins){
            bin.clear();
        }

        for( const T& t : all){
          insert(std::move(t));
		}
	}

	std::vector<T> aggregate_once()
	{
	    std::vector<T> ret;

	    for(bin_t& b : m_bins)
        {
            for(const T& t : b){
                ret.push_back(std::move(t));
            }
        }

        return ret;
	}

	void insert(const T& t)
	{
        MY_VECT r_diff = get_xyz<T>::get(t) - m_p0;

        if(!BDRY_PLCY::check_lo(r_diff,m_p0)){
            return;
        }

        int3 idx = r_diff / m_d;

        if(!BDRY_PLCY::check_hi(idx,m_sz)){
            return;
        }

        get_bin(idx).push_back(t);
	}

	void insert(T&& t)
	{
        MY_VECT r_diff = get_xyz<T>::get(t) - m_p0;

        if(!BDRY_PLCY::check_lo(r_diff,m_p0)){
            return;
        }

        int3 idx = r_diff / m_d;

        if(!BDRY_PLCY::check_hi(idx,m_sz)){
            return;
        }

        get_bin(idx).push_back(std::move(t));
	}

	bin_t& get_bin(const int3& idx)
	{
		idx.max(int3(0,0,0));
		idx.min(m_sz - int3::one);

		int ii = idx.x*(m_sz.z*m_sz.y);
		int jj = idx.y*m_sz.z;
		int kk = idx.z;

		return m_bins[ ii + jj + kk];
	}

	void alloc()
	{
		for( size_t i = 0; i < m_sz.x*m_sz.y*m_sz.z; i++){
			m_bins.push_back( bin_t() );
		}
	}

	bin_t* begin()
	{
	    if(m_bins.size() > 0){
            return &m_bins[0];
        }

        return nullptr;
	}

	bin_t* end()
	{
        if(m_bins.size() > 0){
            return &m_bins[m_bins.size()];
        }

        return nullptr;
	}

	size_t total()
	{
	    size_t t = 0;
        for(bin_t& b : m_bins){
            t += b.size();
        }

        return t;
	}

	MY_VECT					m_d;
	int3					m_sz;
	std::vector< bin_t >	m_bins;
	MY_VECT                 m_p0;

};


}

#endif // HISTO3_H
