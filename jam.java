import java.util.*;

public class jam
{
    // double equality checking
    static final double EPS = 1e-6;
    boolean eq(double a, double b) { return Math.abs(a - b) < EPS; }
    boolean leq(double a, double b) { return eq(a, b) || a < b; }
    boolean geq(double a, double b) { return eq(a, b) || a > b; }

    class Vec
    {
        double x, y;
        
        Vec(double xx, double yy) { x = xx; y = yy; }

        Vec add(Vec o) { return new Vec(x + o.x, y + o.y); }
        Vec sub(Vec o) { return new Vec(x - o.x, y - o.y); }
        Vec mult(double c) { return new Vec(x*c, y*c); }
        double dot(Vec o) { return x*o.x + y*o.y; }
        double cross(Vec o) { return x*o.y - y*o.x; }
        
        double mag2() { return x*x + y*y; }
        double mag() { return Math.sqrt(mag2()); }
        Vec unit() { double d = mag(); return new Vec(x/d, y/d); }
        Vec rot90() { return new Vec(-y, x); }
        Vec rot270() { return new Vec(y, -x); }

        boolean equals(Vec o) { return eq(x, o.x) && eq(y, o.y); }
    }

    class Circle
    {
        Vec c;
        double r;

        Circle(Vec cc, double rr) { c = cc; r = rr; }

        // returns true if the point is inside the circle
        // for this problem, if a point is exactly on the circle,
        // we count it as not inside the circle
        boolean contains(Vec v)
        {
            double d = c.sub(v).mag();

            return !eq(d, r) && leq(d, r);
        }

        // returns the intersection points of two circles
        ArrayList<Vec> intersect(Circle o)
        {
            Vec dir = o.c.sub(c);
            Vec unit = dir.unit();
            double d2 = dir.mag2();
            double d = dir.mag();

            if (r + o.r < d || r + d < o.r || o.r + d < r)
                return new ArrayList<>();
        
            if (eq(d, r + o.r) || eq(r, d + o.r) || eq(o.r, d + r))
            {
                Vec add = c.add(unit.mult(r / d));
                ArrayList<Vec> ret = new ArrayList<>();
                ret.add(add);
                return ret;
            }

            if (d > r + o.r)
                return new ArrayList<>();

            double d1 = (r*r + d2 - o.r*o.r) / (2 * d);
            double h = Math.sqrt(r*r - d1*d1);

            ArrayList<Vec> ret = new ArrayList<>();
            ret.add(c.add(unit.mult(d1)).add(unit.rot270().mult(h)));
            ret.add(c.add(unit.mult(d1)).add(unit.rot90().mult(h)));

            return ret;
        }
    }

    void solve(Scanner sc)
    {
        int n = sc.nextInt();
        ArrayList<Circle> drops = new ArrayList<>();

        for (int i = 0; i < n; i++)
            drops.add(new Circle(new Vec(sc.nextDouble(), sc.nextDouble()), sc.nextDouble()));

        ArrayList<Vec> pts = new ArrayList<>();

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
                ArrayList<Vec> inter = drops.get(i).intersect(drops.get(j));
                pts.addAll(inter);
            }

            // find the point on the circle closest to the origin
            // by drawing a vector to the circle center, and removing
            // the radius from the magnitude
            Vec dir = drops.get(i).c;
            if (dir.equals(new Vec(0, 0)))
                pts.add(new Vec(drops.get(i).r, 0));
            else
                pts.add(dir.unit().mult(dir.mag() - drops.get(i).r));
        }
        
        double ans = 1e12;

         // now that we have all of our points, we can only use it if the
        // point is not inside of any circle.
        for (int j = 0; j < pts.size(); j++)
        {
            boolean good = true;

            // loop over all circles, and check if this point is inside a circle
            for (int i = 0; i < n; i++)
                if (drops.get(i).contains(pts.get(j)))
                {
                    good = false;
                    break;
                }

            if (good)
                ans = Math.min(ans, pts.get(j).mag());
        }

        System.out.printf("%.3f%n", ans);
    }

    public static void main(String[] args)
    {
        Scanner sc = new Scanner(System.in);

        int t = sc.nextInt();
        while (t-->0)
            new jam().solve(sc);
    }
}
