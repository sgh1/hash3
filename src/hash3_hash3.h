
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef HASH3_H
#define HASH3_H

#include <cassert>      // assert
#include <cstddef>      // ptrdiff_t
#include <iterator>     // iterator
#include <type_traits>  // remove_cv
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <limits>
#include <utility>      // swap

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

    typedef typename bin_t::iterator    vect_iter_t;
    typedef typename std::map<idx_t,bin_t>::iterator    map_iter_t;


    template<
            class U,
            class UnqualifiedType = std::remove_cv_t<T>>
    class ForwardIterator
        : public std::iterator<std::forward_iterator_tag,
                               UnqualifiedType,
                               std::ptrdiff_t,
                               U*,
                               U&>
    {
        public:

        typedef typename hash3_base<U>::vect_iter_t iter_vect_iter_t;
        typedef typename hash3_base<U>::map_iter_t  iter_map_iter_t;

        iter_vect_iter_t        m_v_itr;
        iter_map_iter_t         m_m_itr;
        const hash3_base<U>*    m_hash;

        explicit ForwardIterator(iter_vect_iter_t vect_it, iter_map_iter_t map_it,
            const hash3_base<U>* hash_p):
            m_v_itr(vect_it),
            m_m_itr(map_it),
            m_hash(hash_p)
        {
        }

        ForwardIterator():
            m_v_itr(),
            m_m_itr(),
            m_hash(nullptr)
        {
        }

        ForwardIterator& operator++ () // Pre-increment
        {
            iter_vect_iter_t new_vect_it =
                m_hash->next_vect_it(m_m_itr,m_v_itr);

            iter_map_iter_t new_map_it =
                m_hash->next_map_it(m_m_itr,m_v_itr);

            m_m_itr = new_map_it;
            m_v_itr = new_vect_it;

            return *this;
        }

        ForwardIterator operator++ (int) // Post-increment
        {
            //assert(itr != nullptr && "Out-of-bounds iterator increment!");
            ForwardIterator tmp(*this);

            iter_vect_iter_t new_vect_it =
                m_hash->next_vect_it(m_m_itr,m_v_itr);

            iter_map_iter_t new_map_it =
                m_hash->next_map_it(m_m_itr,m_v_itr);

            m_m_itr = new_map_it;
            m_v_itr = new_vect_it;

            return tmp;
        }

        U& operator* () const{
            return *m_v_itr;
        }

        U& operator-> () const{
            return m_v_itr.operator->();
        }

        // two-way comparison: v.begin() == v.cbegin() and vice versa
        // only have to compare vector iterator
        template<class OtherType>
        bool operator == (const ForwardIterator<OtherType>& rhs) const{
            return m_v_itr == rhs.m_v_itr;
        }

        template<class OtherType>
        bool operator != (const ForwardIterator<OtherType>& rhs) const{
            return m_v_itr != rhs.m_v_itr;
        }
    };

    typedef ForwardIterator<T>          iterator;
    typedef ForwardIterator<const T>    const_iterator;

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

    ///in order for these to work correctly,
    ///bins of 0 size are prohibited!

    void prune()
    {
        auto itr = m_bins.begin();
        while (itr != m_bins.end())
        {
            if (itr->second.size() == 0) {
               itr = m_bins.erase(itr);
            } else {
               ++itr;
            }
        }
    }

    iterator begin()
    {
        prune();

        return iterator(m_bins.begin()->second.begin(),
                        m_bins.begin(),
                        this);
    }

    iterator end(){
        return iterator(m_bins.rbegin()->second.end(),
                        m_bins.end(),   //not valid or checked, only vector iterator is compared
                        this);
    }

    //want these to be const...but for prune..?
    const_iterator cbegin()
    {
        prune();

        return const_iterator(  m_bins.begin().second.begin(),
                                m_bins.begin(),
                                this);
    }
    const_iterator cend(){
        return const_iterator(  m_bins.rbegin().second.end(),
                                m_bins.end(),
                                this);
    }

    map_iter_t next_map_it(map_iter_t mit, vect_iter_t vit) const{

        //we have more bins.  we iterate to map.begin(), vect.end(),
        //so we should never get to mit.end()
        if(mit != --(m_bins.end()))
        {
            if(vit == --(mit->second.end())){
                mit++;
                //std::cout << "moving to bin" << (mit)->first << "\n";
                return mit;
            }

            return mit;
        }

        //valid iff vit is at end, todo
        //throw "iterator out of bounds!\n";
        return mit;
    }

    //check the state of mit and vit, and return a new vector iterator
    vect_iter_t next_vect_it(map_iter_t mit, vect_iter_t vit) const{

        //we have more bins
        if(mit != --(m_bins.end()))
        {
            if(vit == --(mit->second.end())){
                mit++;
                return mit->second.begin();
            }
            else{
                return ++vit;
            }
        }

        //we're on the last bin, if vit is at .back(), go to
        //.end, if we're already at .end(), we've made a mistake
        //check
        if(m_bins.rbegin()->second.end() == vit){
            throw "iterator out of bounds!\n";
        }

        return ++vit;
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

//    template<typename U>
//    hash3(const hash3&& other, const my_vect3_t& d):
//		hash3_base<T>(   d )
//    {
//        std::vector<T> all = other.aggregate_once();
//
//		for( typename ctor_std_vector_get<decltype(inp)>::ref_type t : all){
//		    insert(ctor_std_vector_get<decltype(inp)>::get(t));
//        }
//    }
//
	virtual~hash3(){}

    //move Ts out of the hash into a vector
	std::vector<T> aggregate_once()
	{
	    std::vector<T> ret;

	    ret.reserve(this->total());

	    for( const auto& bin : m_bins)
        {
            for(const T& t : bin.second){
                ret.push_back(std::move(t));
            }
        }

        //reset the map
        m_bins.clear();
        return ret;
	}

    std::vector<T> aggregate()
	{
	    std::vector<T> ret;

	    ret.reserve(this->total());

	    for( const auto& bin : m_bins)
        {
            for(const T& t : bin.second){
                ret.push_back(t);
            }
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

    //I think we want U&& here. we ant U&& to be
    //somethin related to T, but it can be hacked, I suppose,
    //to do other things
    template<typename U, typename FUNC>
    void for_each_neighbor(U& test, const FUNC& f)
    {
        idx_t idx = hash_func(test);

        if(this->total() == 0){
            return;
        }

        //for each neighbor (and this) bin
        for(int i = idx.x - 1 ; i < idx.x + 1; i++){
        for(int j = idx.y - 1 ; j < idx.y + 1; j++){
        for(int k = idx.z - 1 ; k < idx.z + 1; k++)
        {
            idx_t idx_cur(i,j,k);

            if(m_bins.find(idx_cur) == m_bins.end()){
                continue;
            }

            typename bin_type<T>::type& cur_bin = m_bins[idx_cur];

            //still a little work to do, what should
            //this type be?
            for(T& t: cur_bin)
            {
                if(t == test){
                    continue;
                }

                f(test,t);
            }
        }}}

        return;
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
