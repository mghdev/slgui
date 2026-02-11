#ifndef SILHOUETTE_INCLUDE_BASE_H_
#define SILHOUETTE_INCLUDE_BASE_H_

#include <cstddef>
#include <cmath>

namespace SL {

template <typename T>
struct _Point
{
    T x,y;
    bool operator==(const _Point& o) const
    {
        return x==o.x && y==o.y;
    }

    _Point<T>& operator+=(const _Point<T>& r)
    {
        x += r.x;
        y += r.y;
        return *this;
    }
    
    _Point<T>& operator-=(const _Point<T>& r)
    {
        x -= r.x;
        y -= r.y;
        return *this;
    }
    
    _Point<T>& operator*=(const T& r)
    {
        x *= r;
        y *= r;
        return *this;
    }
    
    T operator*(const _Point<T>& o)
    {
        return x*o.x + y*o.y;
    }

    _Point<T> operator+(const _Point<T>& r) const
    {
        _Point<T> o{x,y};
        o += r;
        return o;
    }
    
    _Point<T> operator-(const _Point<T>& r) const
    {
        _Point<T> o{x,y};
        o -= r;
        return o;
    }
    
    _Point<T> operator*(const T& r) const
    {
        _Point<T> o{x,y};
        o *= r;
        return o;
    }

    bool operator<(const _Point<T>& r) const  
    {
        return x < r.x ? true : (x == r.x ? y<r.y : false);
    }
    
    double angleTo(const _Point<T>& b) const
    {
        return atan2(static_cast<double>(b.y-y),static_cast<double>(b.x-x));
    }
    
    struct hash
    {
        std::size_t operator()(const _Point<T>& point) const
        {
            return (point.x << 16) ^ point.y;
        }
    };
};
typedef _Point<unsigned> PointU;
typedef _Point<int> PointI;
typedef _Point<double> PointF;
typedef PointI Point;

template <typename T2,typename T1>
_Point<T2> PointCast(_Point<T1> p)
{
    return _Point<T2>{static_cast<T2>(p.x),static_cast<T2>(p.y)};
}

template <typename T>
struct _Rect
{
    T x,y,w,h;
    
    _Point<T> origin() const
    {
        return {x,y};
    }
    
    bool operator==(const _Rect& o) const
    {
        return x==o.x && y==o.y && w==o.w && h==o.h;
    }
    bool operator!=(const _Rect& o) const
    {
        return !(*this == o);
    }
    _Rect<T> operator+(const _Point<T>& r) const
    {
        _Rect<T> o{x,y,w,h};
        o.x += r.x;
        o.y += r.y;
        return o;
    }
};
typedef _Rect<unsigned> RectU;
typedef _Rect<int> RectI;
typedef _Rect<double> RectF;
typedef RectI Rect;

template <typename T>
_Point<T> origin(const _Rect<T>& r)
{
    return {r.x,r.y};
}

template <typename T>
_Point<T> maxPoint(const _Rect<T>& r)
{
    return {r.x+r.w,r.y+r.h};
}

template <typename T>
_Rect<T> rectFromPoints(const _Point<T>& a,const _Point<T>& b)
{
    auto x = std::min(a.x,b.x);
    auto y = std::min(a.y,b.y);
    auto w = std::max(a.x,b.x) - x;
    auto h = std::max(a.y,b.y) - y;
    return {x,y,w,h};
}

template <typename T>
struct _Line
{
    T x1,y1,x2,y2;
    // _Line(_Point<T> p1,_Point<T> p2) : x1(p1.x),y1(p1.y),x2(p2.x),y2(p2.y) {}
};
typedef _Line<unsigned> LineU;
typedef _Line<int> LineI;
typedef _Line<double> LineF;
typedef LineI Line;

template <typename T>
T clamp(const T& min,const T& value,const T& max)
{
    return value<min ? min : (value > max ? max : value);
}

struct Color
{
    Uint8 r=0,g=0,b=0;
};
static constexpr Color BLACK = {0,0,0};

} //namespace SL



// template <typename T>
// struct SLUnorderedPair
// {
//     T a,b;
    
//     bool operator==(const SLUnorderedPair<T>& o) const
//     {
//         return (a==o.a && b==o.b) || (a==o.b && b==o.a);
//     }
//     bool operator<(const SLUnorderedPair<T>& o) const  
//     {
//         // return a+b < o.a+o.b;
//         return a+b == o.a+o.b ? (*this != o && a < o.a) : a+b < o.a+o.b;
//     }
    
//     SLUnorderedPair<T> operator-() const
//     {
//         return SLUnorderedPair<T>{b,a};
//     }
    
//     struct hash
//     {
//         std::size_t operator()(const SLUnorderedPair<T>& pair) const
//         {
//             const std::size_t a = static_cast<std::size_t>(pair.a);
//             const std::size_t b = static_cast<std::size_t>(pair.b);
//             return (a<b) ? ((b << 32) ^ a) : ((a << 32) ^ b);
//         }
//     };
// };
// typedef SLUnorderedPair<unsigned> SLUPair;

#endif




