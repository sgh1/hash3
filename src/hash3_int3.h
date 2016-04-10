
#include <iostream>

#ifndef HASH3_INT3_H
#define HASH3_INT3_H

namespace hash3
{


/**
 * int3
 * minimal implementation of 3d index that
 * we need to help work with bins of the hash
 */

template <typename T>
class int3
{
public:

    int3( T i, T j, T k):
        x(i),y(j),z(k)
    {
    }

    int3(const int3& src):
        x(src.x),y(src.y),z(src.z)
    {
    }

    int3():
        x(0),y(0),z(0)
    {
    }

    bool operator==(const int3& other) const
    {
        return other.x == x &&
            other.y == y && other.z == z;
    }

    bool operator!=(const int3& other) const
    {
        return !(*this == other);
    }

    bool any_greater_than(const int3& other) const
    {
        return other.x > x ||
            other.y > y || other.z > z;
    }

    T x,y,z;

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const int3<T>& i3)
{
    os << "(" << i3.x << "," << i3.y << "," << i3.z << ")";
}

}


#endif /* INT3_H */
