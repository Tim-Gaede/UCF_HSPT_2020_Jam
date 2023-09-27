#include <bits/stdc++.h>

using namespace std;
typedef long double ld;

// double equality checkers
static bool eq(ld a, ld b) { return abs(a - b) < 1e-6; }
static bool leq(ld a, ld b) { return eq(a, b) || a < b; }

struct Vec
{
    ld x, y;
    Vec(ld x, ld y): x(x), y(y) {}

    // vector addition
    Vec operator+(const Vec &o) const { return Vec(x + o.x, y + o.y); }
    Vec operator-(const Vec &o) const { return Vec(x - o.x, y - o.y); }

    // scalar mutliplication
    Vec operator*(const ld c) const { return Vec(c*x, c*y); }
    
    // dot product
    ld operator*(const Vec &o) const { return x*o.x + y*o.y; }

    // cross product
    ld operator%(const Vec &o) const { return x*o.y - y*o.x; }

    // vector magnitude
    ld mag2() const { return x*x + y*y; }
    ld mag() const { return sqrt(mag2()); }

    // other transformations
    Vec unit() const { return (*this) * (1 / mag()); }
	Vec rot90() const { return Vec(-y, x); }
	Vec rot270() const{ return Vec(y, -x); }

    // vector equality
    bool operator==(const Vec &o) const { return eq(x, o.x) && eq(y, o.y); }
    bool operator!=(const Vec &o) const { return !(*this == o); }
};

struct Circle
{
    Vec c;
    ld r;

    Circle(Vec c, ld r): c(c), r(r) {}

    // circle equality
    bool operator==(const Circle &o) const { return c == o.c && eq(r, o.r); }
    bool operator!=(const Circle &o) const { return !(*this == o); }

    // returns true if the point is inside the circle
    // for this problem, if a point is exactly on the circle,
    // we count it as not inside the circle
    bool contains(Vec v)
    {
        Vec dir = c - v;
        
        return !eq(dir.mag(), r) && leq(dir.mag(), r);
    }

    // returns the intersection points of two circles
    vector<Vec> intersect(Circle o)
    {
        Vec dir = o.c - c;
        Vec unit = dir.unit();
        ld d2 = dir.mag2();
        ld d = sqrt(d2);

        if (r + o.r < d || r + d < o.r || o.r + d < r)
            return vector<Vec>();
        
        if (eq(d, r + o.r) || eq(r, d + o.r) || eq(o.r, d + r))
            return vector<Vec>(1, c + unit * (r / d));

        if (d > r + o.r)
            return vector<Vec>();

        ld d1 = (r*r + d2 - o.r*o.r) / (2 * d);
        ld h = sqrt(r*r - d1*d1);

        vector<Vec> ret;
        ret.push_back(c + (unit*d1) + (unit.rot270() *h));
        ret.push_back(c + (unit*d1) + (unit.rot90() * h));

        return ret;
    }
};

void solve()
{
    int n; cin >> n;
    vector<Circle> drops;

    for (int i = 0; i < n; i++)
    {
        ld x, y, r; cin >> x >> y >> r;

        drops.push_back(Circle(Vec(x, y), r));
    }

    vector<Vec> pts;

    // the points we're interested in are the intersection points of all 
    // pairs of circles, and the point on each circle closest to the origin
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            
            // take the intersection points of all pairs of circles, 
            // and add them to our list
            vector<Vec> inter = drops[i].intersect(drops[j]);
            pts.insert(pts.end(), inter.begin(), inter.end());
        }

        // find the point on the circle closest to the origin
        // by drawing a vector to the circle center, and removing
        // the radius from the magnitude
        Vec dir = drops[i].c;
        if (dir == Vec(0, 0))
            pts.push_back(Vec(drops[i].r, 0));
        else
            pts.push_back(dir.unit() * (dir.mag() - drops[i].r));
    }

    ld ans = 1e12;

    // now that we have all of our points, we can only use it if the
    // point is not inside of any circle.
    for (int j = 0; j < static_cast<int>(pts.size()); j++)
    {
        bool good = true;

        // loop over all circles, and check if this point is inside a circle
        for (int i = 0; i < n; i++)
            if (drops[i].contains(pts[j]))
            {
                good = false;
                break;
            }

        if (good)
            ans = min(ans, pts[j].mag());
    }

		std::cout << std::setprecision(3) << std::fixed << ans << "\n";
}

int main()
{
    int t; cin >> t;

    while (t-->0)
        solve();

    return 0;
}
