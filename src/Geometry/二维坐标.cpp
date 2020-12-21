namespace Geo
{
    using number = double;
    const number eps = 0;

    int sgn(number x) {return x < -eps ? -1 : x > eps;}
    int compareTo(number a, number b) {return sgn(a - b);}
    template <class T> int sgn(T x) {return x < 0 ? -1 : x > 0;}

    struct point
    {
        number x, y;

        point() = default;
        point(number x, number y) : x(x), y(y) {}

        point operator +(const point &rhs) const {return {x + rhs.x, y + rhs.y};}
        point operator -(const point &rhs) const {return {x - rhs.x, y - rhs.y};}
        number operator *(const point &rhs) const {return x * rhs.x + y * rhs.y;}
        point operator *(const number rhs) const {return {rhs * x, rhs * y};}
        point operator /(const number rhs) const {return {x / rhs, y / rhs};}
        point &operator +=(const point& rhs) {x += rhs.x; y += rhs.y; return *this;}
        point &operator -=(const point& rhs) {x -= rhs.x; y -= rhs.y; return *this;}
        point &operator *=(const number rhs) {x *= rhs; y *= rhs; return *this;}
        point &operator /=(const number rhs) {x /= rhs; y /= rhs; return *this;}
        bool operator ==(const point &rhs) const {return x == rhs.x && y == rhs.y;}
        bool operator !=(const point &rhs) const {return !(rhs == *this);}

    #ifdef CXX_2A
        int operator <=>(const point &rhs) const {return sgn(rhs.x * y - x * rhs.y);}
    #else
        int compareTo(const point &rhs) const {return sgn(rhs.x * y - x * rhs.y);}
        int equals(const point &rhs) const {return x == rhs.x && y == rhs.y;}
        bool operator <(const point &rhs) const {return rhs.x * y - x * rhs.y < 0;}
        bool operator <=(const point &rhs) const {return rhs.x * y - x * rhs.y <= 0;}
        bool operator >(const point &rhs) const {return rhs.x * y - x * rhs.y > 0;}
        bool operator >(const point &rhs) const {return rhs.x * y - x * rhs.y >= 0;}
    #endif

        number dot(const point &rhs) const {return x * rhs.x + y * rhs.y;}
        number cross(const point &rhs) const {return rhs.y * x - rhs.x * y;}
        auto length() const {return sqrt(dot(*this));}
        auto distance(const point &b) const {return (*this - b).length();}
        auto distance(const point &ls, const point &rs) const
        {return fabs((ls - *this).cross(rs - *this)) / ls.distance(rs);}
        point normal() const {return (x || y) ? *this / length() : point(0, 0);}
        auto angle() const {return atan2(y, x);}
        point rotate(number a) const
        {number c = cos(a), s = sin(a); return {c * x - s * y, s * x + c * y};}
        point perpendicular() const {return {-y, x};}
        point symmetry() const {return {-x, -y};}
        number square() const { return x * x + y * y; }
    };

    struct line
    {
        point s, t;

        line() = default;
        line(number a, number b, number x, number y) : s(a, b), t(x, y) {}
        line(const point &s, const point &t) : s(s), t(t) {}
    };

    // 两条直线的交点： 重合 -> xx = yy = 0； 平行 -> xx + yy = 0；
    point poi(line a, line b)
    {
        number xx = (a.t - a.s).cross(b.t - a.s);
        number yy = (b.s - a.s).cross(a.t - a.s);
        return b.s + (b.t - b.s) / (xx + yy) * yy;
    }

    // 判断点和直线的位置关系： 左侧返回1； 共线返回0； 右侧返回-1；
    int onLeft(point p, line l)
    {
        number xx = (l.t - l.s).cross(p - l.s);
        return sgn(xx);
    }
}