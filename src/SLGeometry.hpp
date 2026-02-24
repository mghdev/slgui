#include "SLBase.hpp"

namespace SL {

template <typename T>
inline _Rect<T> superRect(const _Rect<T>& r1, const _Rect<T>& r2)
{
    auto x = std::min(r1.x,r2.x);
    auto y = std::min(r1.y,r2.y);
    return {
        x,
        y,
        std::max(r1.x+r1.w,r2.x+r2.w)-x,
        std::max(r1.y+r1.h,r2.y+r2.h)-y
    };
}

template <typename T>
inline _Rect<T> intersection(const _Rect<T>& r1, const _Rect<T>& r2)
{
    if(detectCollision(r1,r2)) {
        auto x = std::max(r1.x,r2.x);
        auto y = std::max(r1.y,r2.y);
        return {
            x,
            y,
            std::min(r1.x+r1.w,r2.x+r2.w)-x,
            std::min(r1.y+r1.h,r2.y+r2.h)-y
        };
    }
    return {0,0,0,0};
}

template <typename T>
inline bool detectCollision(const _Rect<T>& r1, const _Rect<T>& r2)
{
    return (r2.x+r2.w >= r1.x && r1.x+r1.w >= r2.x) && (r2.y+r2.h >= r1.y && r1.y+r1.h >= r2.y);
}

template <typename T>
inline bool isPointInRect(const Vec2<T>& p, const _Rect<T>& r)
{
    return clamp(r.x,p.x,r.x+r.w) == p.x && clamp(r.y,p.y,r.y+r.h) == p.y;
}

template <typename T>
inline Vec2<T> relativePoint(const Vec2<T>& p,const _Rect<T>& r)
{
    return p-origin(r);
}

template <typename T>
inline double magnitude(Vec2<T> p)
{
    return sqrt(pow(p.x,2)+pow(p.y,2));
}

// return k such that
//  k*p1 + (1-k)*p2 == p3
// where p3 is the orthogonal projection of point onto the line p1,p2
template <typename T>
inline double inverseLerpProjection(Vec2<T> p1,Vec2<T> p2, Vec2<T> point)
{
    return 1.0+static_cast<double>((p2-p1)*(p1-point))/(pow(magnitude(p2-p1),2));
}

template <typename T>
inline Vec2<double> lerp(Vec2<T> p1,Vec2<T> p2, double k)
{
    return PointCast<double>(p1)*k + PointCast<double>(p2)*(1-k);
}

template <typename T>
Vec2<double> closestPoint(_Line<T> line, Vec2<T> point)
{
    Vec2<T> p1 = {line.x1,line.y1};
    Vec2<T> p2 = {line.x2,line.y2};
    auto k = inverseLerpProjection(p1,p2,point);
    return lerp(p1,p2,k);
}

template <typename T>
inline double distance(Vec2<T> p1, Vec2<T> p2)
{
    return magnitude(p2-p1);
}

template <typename T>
inline double distance(_Line<T> line, Vec2<T> point)
{
    return magnitude(closestPoint(line,point) - point);
}

} //namespace SL


