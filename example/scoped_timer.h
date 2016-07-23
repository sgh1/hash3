
/*****
*This file is part of hash3: https://github.com/sgh1/hash3
*Please see the license and readme!
*****/

#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>
#include <string>


/***
 * particle
 * simple particle class used for hash3 example
 **/

class scoped_timer
{
    public:

    scoped_timer(const std::string& desc):
        m_desc(     desc),
        m_start(    std::chrono::system_clock::now()),
        m_running(  true)
    {}

    scoped_timer(const scoped_timer& x) = delete;
    scoped_timer& operator=(const scoped_timer& other) = delete;

    std::chrono::duration<double> stop(){

        std::chrono::time_point<std::chrono::system_clock> end =
             std::chrono::system_clock::now();

        m_running = false;

        return end - m_start;
    }

    virtual ~scoped_timer(){

        if(m_running){
            std::cout << m_desc << " " << stop().count() << "\n";
        }
	}

    std::string		m_desc;
    std::chrono::time_point<std::chrono::system_clock> m_start;
    bool            m_running;

};



#endif // TIMER_H
