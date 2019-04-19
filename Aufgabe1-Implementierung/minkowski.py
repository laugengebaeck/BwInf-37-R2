import math
from pyvisgraph.graph import Point

#Minkowski-Summe auf alle Polygone anwenden
def minkowski_sum_list(polygons, lisa_poly):
    newpolygons = []
    for poly in polygons:
        newpolygons.append(minkowski_sum(poly,lisa_poly))
    return newpolygons

#Algorithmus für die Minkowski-Summe wie in der Dokumentation beschrieben
#zuerst Summe laut Definition und dann Convex Hull mit Graham Scan
def minkowski_sum(v,w):
    sump = []
    for p1 in v:
        for p2 in w:
            sump.append(Point(p1.x + p2.x, p1.y + p2.y, polygon_id=p1.real_polygon_id))
    p0 = sump[findsmallest(sump)]
    chull = [p0]
    sump.remove(p0)
    sump.sort(key=lambda point: angle(p0,point))
    for point in sump:
        while len(chull) > 1 and ccw(chull[len(chull)-2],chull[len(chull)-1],point) < 0:
            chull.pop()
        chull.append(point)
    return chull

# Richtung der drei Punkte feststellen (Counter/Clockwise)
# mittels z-Koordinate des Kreuzprodukts aus ab und ac
def ccw(a,b,c):
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)

#Winkel zwischen Vektor ab und positiver x-Achse
#Ansatz siehe https://math.stackexchange.com/questions/1910825/how-do-i-find-the-angle-a-vector-makes-to-the-x-axis
def angle(a, b):
    vx = b.x - a.x
    vy = b.y - a.y
    angle = math.atan2(vy,vx)
    if vy < 0:
        angle += 2 * math.pi
    return angle

#finde den Punkt mit der kleinsten y-Koordinate und setze ihn als ersten Punkt im Polygon
def findsmallest(polygon):
    index = 0
    miny = math.inf
    minx = math.inf
    for i,point in enumerate(polygon):
        if point.y < miny or (point.y == miny and point.x < minx):
            minx = point.x
            miny = point.y
            index = i
    return index