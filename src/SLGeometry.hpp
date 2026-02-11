#include "SLBase.hpp"

namespace SL {

template <typename T>
inline bool isPointInRect(const _Point<T>& p, const _Rect<T>& r)
{
    return clamp(r.x,p.x,r.x+r.w) == p.x && clamp(r.y,p.y,r.y+r.h) == p.y;
}

template <typename T>
inline _Point<T> relativePoint(const _Point<T>& p,const _Rect<T>& r)
{
    return p-origin(r);
}

template <typename T>
inline double magnitude(_Point<T> p)
{
    return sqrt(pow(p.x,2)+pow(p.y,2));
}

// return k such that
//  k*p1 + (1-k)*p2 == p3
// where p3 is the orthogonal projection of point onto the line p1,p2
template <typename T>
inline double inverse_lerp_projection(_Point<T> p1,_Point<T> p2, _Point<T> point)
{
    return 1.0+static_cast<double>((p2-p1)*(p1-point))/(pow(magnitude(p2-p1),2));
}

template <typename T>
inline _Point<double> lerp(_Point<T> p1,_Point<T> p2, double k)
{
    return PointCast<double>(p1)*k + PointCast<double>(p2)*(1-k);
}

template <typename T>
_Point<double> closest_point(_Line<T> line, _Point<T> point)
{
    _Point<T> p1 = {line.x1,line.y1};
    _Point<T> p2 = {line.x2,line.y2};
    auto k = inverse_lerp_projection(p1,p2,point);
    return lerp(p1,p2,k);
}

template <typename T>
inline double distance(_Point<T> p1, _Point<T> p2)
{
    return magnitude(p2-p1);
}

template <typename T>
inline double distance(_Line<T> line, _Point<T> point)
{
    return magnitude(closest_point(line,point) - point);
}

} //namespace SL


