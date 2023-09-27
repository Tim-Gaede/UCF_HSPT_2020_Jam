import math

EPSILON = 1e-5   # epsilon for checking relatively close points

def eq(a, b): # equality function to check equality using epsilons
    return abs(a - b) <= EPSILON

#class to hold points (vectors)
class vec(object):
    def __init__(self, xx, yy):
        self.x = xx
        self.y = yy
    def angle(self):
        return math.atan2(self.y, self.x)
    def mag(self, o):
        xx = self.x - o.x
        yy = self.y - o.y
        return xx * xx + yy * yy
    def dist(self, o):
        return math.sqrt(self.mag(o))
    def __add__(self, v):
        return vec(self.x + v.x, self.y + v.y)
    def __sub__(self, v):
        return vec(self.x - v.x, self.y - v.y)
    def __str__(self):
        return "(%.2f, %.2f)" % (round(self.x, 2), round(self.y, 2))

#class to hold circles
class circle(object):
    def __init__(self, xx, yy, rr):
        self.center = vec(xx, yy)
        self.r = rr
    def containsPoint(self, pt):
        d = self.center.dist(pt)
        return eq(d, self.r) or d < self.r
    def containsCircle(self, c):
        if not self.containsPoint(c.center):
            return False
        d = self.center.dist(c.center)
        return d + c.r + EPSILON < self.r
    def getPt(self, angle):
        return vec(self.center.x + math.cos(angle) * self.r, self.center.y + math.sin(angle) * self.r)
    def intersect(self, c):
        ans = []
        me = circle(self.center.x, self.center.y, self.r)
        d = self.center.dist(c.center)
        # 0 intersections
        if d > self.r + c.r + EPSILON:
            return ans
        if self.containsCircle(c) or c.containsCircle(me):
            return ans
        cur_angle = math.atan2(c.center.y - self.center.y, c.center.x - self.center.x)
        if eq(d, self.r + c.r):
            ans.append(self.getPt(cur_angle))
        else:
            a = self.r
            b = c.r
            angle = math.acos((b*b - a*a - d*d) / (-2 * a * d))
            ans.append(self.getPt(cur_angle + angle))
            ans.append(self.getPt(cur_angle - angle))
        return ans

def solve():
    n = int(input())
    cs = []
    for i in range(n):
        x, y, r = (int(i) for i in input().split(" "))
        cs.append(circle(x, y, r))
    candidates = []
    for i in range(n):
        for j in range(i + 1, n):
            inter = cs[i].intersect(cs[j])
            for p in inter:
                candidates.append(p)
    origin = vec(0, 0)
    for c in cs:
        if c.center.x == 0 and c.center.y == 0:
            candidates.append(c.getPt(0))
        else:
            angle = c.center.angle()
            candidates.append(c.getPt(angle))
            candidates.append(c.getPt(angle + math.pi))
    ans = 1e18
    for p in candidates:
        ok = True
        for c in cs:
            dist = p.dist(c.center)
            if eq(dist, c.r):
                continue
            if dist < c.r:
                ok = False
        if ok:
            ans = min(ans, p.dist(origin))
    return ans

testCases = int(input())
for t in range(testCases):
    print("%.3f"%(solve()))
