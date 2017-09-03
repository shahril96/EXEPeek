
#include <initializer_list>

namespace Misc
{

template <typename T1, typename T2>
struct pair {

    T1 first;
    T2 second;

    pair(void) : first(T1()), second(T2()) {} // default constructor
    
    pair(const T1 &first, const T2 &second) : first(first), second(second) {}
    
    template <typename U1, typename U2> 
    pair(const pair<U1, U2> &p) : first(p.first), second(p.second) {}

    template <typename U1, typename U2>
    pair &operator=(const pair<U1, U2> &p) { first = p.first; second = p.second; return *this; }

};

template <typename T1, typename T2>
pair<T1, T2> make_pair(const T1 &a, const T2 &b)
{
    pair<T1, T2> p(a, b);
    return p;
}

};