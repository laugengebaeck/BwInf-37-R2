import math
from pyvisgraph.graph import Point

#Minkowski-Summe auf alle Polygone anwenden
#vorher Vorverarbeitung, damit die Eingaben korrekt sind
def minkowski_sum_list(polygons, lisa_poly):
    polygons = [findsmallest(makeccw(poly)) for poly in polygons]
    polygons = [minkowski_sum(poly,lisa_poly) for poly in polygons]
    return polygons

#TODO does not really work
#Algorithmus für die Minkowski-Summe wie in der Dokumentation beschrieben
def minkowski_sum(v,w):
    i = 0
    j = 0
    n = len(v)
    m = len(w)
    v.append(v[0])
    v.append(v[1])
    w.append(w[0])
    w.append(w[1])
    newpoly = []
    while True:
        newpoly.append(Point(v[i].x + w[j].x, v[i].y + w[j].y, polygon_id=v[i].polygon_id))
        if angle(v[i],v[i+1]) < angle(w[j],w[j+1]):
            i = i+1
        elif angle(v[i],v[i+1]) > angle(w[j],w[j+1]):
            j = j+1
        else:
            i = i+1
            j = j+1
        if i == n and j == m:
            break
    return newpoly

#Winkel zwischen Vektor ab und positiver x-Achse
#Ansatz siehe https://math.stackexchange.com/questions/1910825/how-do-i-find-the-angle-a-vector-makes-to-the-x-axis
def angle(a, b):
    vx = b.x - a.x
    vy = b.y - a.y
    angle = math.atan2(vy,vx)
    if vy < 0:
        angle += 2 * math.pi
    return angle

#Polygon-Punkte in Richtung gegen den Uhrzeigersinn bringen
def makeccw(polygon):
    if not isccw(polygon):
        polygon.reverse()
    return polygon

#Berechnung, ob ein Polygon gegen den Uhrzeigersinn (CCW) orientiert ist
#Ansatz: https://de.wikipedia.org/wiki/Gau%C3%9Fsche_Trapezformel
def isccw(polygon):
    sum = 0
    length = len(polygon)
    for i in range(len(polygon)):
        sum += (polygon[(i+1) % length].x - polygon[i].x) * (polygon[(i+1) % length].y + polygon[i].y)
    return sum < 0

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
    #schiebe Punkte davor ans Ende
    for i in range(index):
        polygon.append(polygon[i])
    polygon = polygon[index:]
    return polygon