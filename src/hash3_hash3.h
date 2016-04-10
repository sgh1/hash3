
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
	typename BDRY_PLCY = default_bdry
>
class hash3
{
    public:

	typedef std::vector<T> 				bin_t;
	typedef typename T::num_type 	    num_t;
	typedef vector3<double>             vect3_t;
	typedef int3<int>                   idx_t;

    hash3():
	    m_sz(	),
		m_p0(   ),
		m_d(   	)
	{
        alloc();
	}

	hash3(const vect3_t& p0, const vect3_t& p1, idx_t sz):
	    m_sz(	sz),
		m_p0(   p0),
		m_d(   (p1 - p0 ) / sz )
	{
        alloc();
	}

	hash3(const std::vector<T>& inp, idx_t sz):
		m_sz(	sz)
	{
		alloc();

		num_t big = std::numeric_limits<num_t>::max();
		num_t small = std::numeric_limits<num_t>::min();

		vect3_t min_pt( big, big, big );
		vect3_t max_pt( small, small, small);

		for( const T& t : inp)
		{
		    const vect3_t& cur_vect = T::get_xyz(t);

            min_pt = min_pt.min_of(cur_vect);
            max_pt = max_pt.max_of(cur_vect);
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
	    for(int i = 0; i < m_sz.x; i++){
            for(int j = 0; j < m_sz.y; j++){
                for(int k = 0; k < m_sz.z; k++){

					idx_t idx(i,j,k);
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
        vect3_t r_diff = T::get_xyz(t) - m_p0;

        if(!BDRY_PLCY::check_lo(r_diff,m_p0)){
            return;
        }

        idx_t idx = vect32int3(r_diff / m_d);

        if(!BDRY_PLCY::check_hi(idx,m_sz)){
            return;
        }

        get_bin(idx).push_back(t);
	}

	void insert(T&& t)
	{
        vect3_t r_diff = T::get_xyz(t) - m_p0;

        if(!BDRY_PLCY::check_lo(r_diff,m_p0)){
            return;
        }

        idx_t idx = r_diff / m_d;

        if(!BDRY_PLCY::check_hi(idx,m_sz)){
            return;
        }

        get_bin(idx).push_back(std::move(t));
	}

	bin_t& get_bin(const idx_t& idx)
	{
		idx_t idx_2 = idx.max(idx_t(0,0,0));
		idx_t idx_3 = idx_2.min(m_sz - idx_t(1,1,1));

		int ii = idx_3.x*(m_sz.z*m_sz.y);
		int jj = idx_3.y*m_sz.z;
		int kk = idx_3.z;

		return m_bins[ ii + jj + kk];
	}

	void alloc()
	{
		for( int i = 0; i < m_sz.x*m_sz.y*m_sz.z; i++){
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

	vect3_t					m_d;
	idx_t					m_sz;
	std::vector< bin_t >	m_bins;
	vect3_t                 m_p0;

};


}

#endif // HISTO3_H
