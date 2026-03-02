#ifndef SILHOUETTE_INCLUDE_BASE_H_
#define SILHOUETTE_INCLUDE_BASE_H_

#include <cstddef>
#include <cmath>
#include <stdint.h>

namespace SL {

template <typename T>
struct Vec2
{
    T x,y;
    bool operator==(const Vec2& o) const
    {
        return x==o.x && y==o.y;
    }

    Vec2<T>& operator+=(const Vec2<T>& r)
    {
        x += r.x;
        y += r.y;
        return *this;
    }
    
    Vec2<T>& operator-=(const Vec2<T>& r)
    {
        x -= r.x;
        y -= r.y;
        return *this;
    }
    
    Vec2<T>& operator*=(const T& r)
    {
        x *= r;
        y *= r;
        return *this;
    }
    
    T operator*(const Vec2<T>& o)
    {
        return x*o.x + y*o.y;
    }

    Vec2<T> operator+(const Vec2<T>& r) const
    {
        Vec2<T> o{x,y};
        o += r;
        return o;
    }
    
    Vec2<T> operator-(const Vec2<T>& r) const
    {
        Vec2<T> o{x,y};
        o -= r;
        return o;
    }
    
    Vec2<T> operator-() const
    {
        return {-x,-y};
    }
    
    Vec2<T> operator*(const T& r) const
    {
        Vec2<T> o{x,y};
        o *= r;
        return o;
    }

    bool operator<(const Vec2<T>& r) const  
    {
        return x < r.x ? true : (x == r.x ? y<r.y : false);
    }
    
    double angleTo(const Vec2<T>& b) const
    {
        return atan2(static_cast<double>(b.y-y),static_cast<double>(b.x-x));
    }
    
    struct hash
    {
        std::size_t operator()(const Vec2<T>& point) const
        {
            return (point.x << 16) ^ point.y;
        }
    };
};
typedef Vec2<int> Vec2I;
typedef Vec2I PointI;
typedef Vec2<double> Vec2F;
typedef Vec2F PointF;
typedef PointF Point;

template <typename T2,typename T1>
Vec2<T2> PointCast(Vec2<T1> p)
{
    return Vec2<T2>{static_cast<T2>(p.x),static_cast<T2>(p.y)};
}

template <typename T>
struct _Rect
{
    T x,y,w,h;
    
    Vec2<T> origin() const
    {
        return {x,y};
    }
    
    Vec2<T> size() const
    {
        return {w,h};
    }
    
    Vec2<T> maxPoint() const
    {
        return {x+w,y+h};
    }
    
    _Rect<T> transform(const Vec2<T>& offset, const Vec2<T>& scale = {1,1}) const
    {
        return {x+offset.x,y+offset.y,w*scale.x,h*scale.y};
    }
    
    bool containsOther(const _Rect& other)
    {
        return x <= other.x && y <= other.y && (x+w >= other.x+other.w) && (y+h >= other.y+other.h);
    }
    
    bool operator==(const _Rect& o) const
    {
        return x==o.x && y==o.y && w==o.w && h==o.h;
    }
    bool operator!=(const _Rect& o) const
    {
        return !(*this == o);
    }
    _Rect<T> operator+(const Vec2<T>& r) const
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
typedef RectF Rect;

template <typename T>
_Rect<T> rectFromPoints(const Vec2<T>& a,const Vec2<T>& b)
{
    auto x = std::fmin(a.x,b.x);
    auto y = std::fmin(a.y,b.y);
    auto w = std::fmax(a.x,b.x) - x;
    auto h = std::fmax(a.y,b.y) - y;
    return {x,y,w,h};
}

template <typename T>
struct _Line
{
    T x1,y1,x2,y2;
    // _Line(Vec2<T> p1,Vec2<T> p2) : x1(p1.x),y1(p1.y),x2(p2.x),y2(p2.y) {}
};
typedef _Line<unsigned> LineU;
typedef _Line<int> LineI;
typedef _Line<double> LineF;
typedef LineF Line;

template <typename T>
T clamp(const T& min,const T& value,const T& max)
{
    return value<min ? min : (value > max ? max : value);
}

struct Color
{
    uint8_t r=0,g=0,b=0,a=0;
    
    bool operator==(const Color& o) const
    {
        return r==o.r && g==o.g && b==o.b && a==o.a;
    }
};

namespace COLOR {
    static constexpr Color NONE = {0,0,0,0};
    static constexpr Color WHITE = {255,255,255,255};
    static constexpr Color BLACK = {0,0,0,255};
    static constexpr Color DARKMODE_BACKGROUND = {29,30,32,255};
} //namespace Color

} //namespace SL

#endif




