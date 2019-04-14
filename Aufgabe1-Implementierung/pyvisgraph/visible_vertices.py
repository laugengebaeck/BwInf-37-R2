"""
The MIT License (MIT)

Copyright (c) 2016 Christian August Reksten-Monsen & 2019 Lukas Rost

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""
from __future__ import division
from math import pi, sqrt, atan, acos
from pyvisgraph.graph import Point

INF = 10000
# Toleranz für Gleitkommazahlen
COLIN_TOLERANCE = 10
T = 10**COLIN_TOLERANCE
T2 = 10.0**COLIN_TOLERANCE

def visible_vertices(point, graph, origin=None, destination=None, scan='full'):
    """Gibt Liste der von point sichtbaren Punkte zurück. origin und destination werden dabei ebenfalls mit einbezogen. scan='full' überprüft die Sichtbarkeit aller Punkte und wird hier ausschließlich verwendet.
    """
    edges = graph.get_edges()
    points = graph.get_points()
    if origin: points.append(origin)
    if destination: points.append(destination)
    points.sort(key=lambda p: (angle(point, p), edge_distance(point, p)))

    # open_edges initialisieren (alle Kanten, die die anfaengliche Sweep line schneiden)
    open_edges = []
    point_inf = Point(INF, point.y)
    for e in edges:
        if point in e: continue
        if edge_intersect(point, point_inf, e):
            if on_segment(point, e.p1, point_inf): continue
            if on_segment(point, e.p2, point_inf): continue
            k = EdgeKey(point, point_inf, e)
            insort(open_edges, k)

    visible = []
    prev = None
    prev_visible = None
    for p in points:
        if p == point: continue
        if scan == 'half' and angle(point, p) > pi: break

        #Inzidente Kanten, die in Uhrzeigerrichtung liegen, entfernen
        if open_edges:
            for edge in graph[p]:
                if ccw(point, p, edge.get_adjacent(p)) == -1:
                    k = EdgeKey(point, p, edge)
                    index = bisect(open_edges, k) - 1
                    if len(open_edges) > 0 and open_edges[index] == k:
                        del open_edges[index]

        # Sichtbarkeit von p von point aus überprüfen
        is_visible = False
        # vorheriger Punkt (prev) nicht auf der Geraden point-p
        if prev is None or ccw(point, prev, p) != 0 or not on_segment(point, prev, p):
            #keine offenen Kanten -> sichtbar
            if len(open_edges) == 0:
                is_visible = True
            #Strecke schneidet erste Kante nicht -> sichtbar
            elif not edge_intersect(point, p, open_edges[0].edge):
                is_visible = True
        # ...liegen auf einer Geraden *und* vorheriger Punkt war nicht sichtbar -> nicht sichtbar
        elif not prev_visible:
            is_visible = False
        # ...liegen auf einer Geraden *und* vorheriger Punkt war sichtbar -> alle offenen Kanten überprüfen
        else:
            is_visible = True
            for e in open_edges:
                if prev not in e.edge and edge_intersect(prev, p, e.edge):
                    is_visible = False
                    break
            if is_visible and edge_in_polygon(prev, p, graph):
                    is_visible = False

        # Strecke liegt komplett in einem Polygon -> nicht sichtbar
        if is_visible and p not in graph.get_adjacent_points(point):
            is_visible = not edge_in_polygon(point, p, graph)

        if is_visible: visible.append(p)

        # Inzidente Kanten, die gegen Uhrzeigerrichtung liegen, hinzufügen
        for edge in graph[p]:
            if (point not in edge) and ccw(point, p, edge.get_adjacent(p)) == 1:
                k = EdgeKey(point, p, edge)
                insort(open_edges, k)

        prev = p
        prev_visible = is_visible
    return visible


def polygon_crossing(p1, poly_edges):
    """Überprüft, ob Punkt in Polygon liegt und verwendet dazu die Strahl-Methode (https://de.wikipedia.org/wiki/Punkt-in-Polygon-Test_nach_Jordan). """
    p2 = Point(INF, p1.y)
    intersect_count = 0
    co_flag = False
    co_dir = 0
    for edge in poly_edges:
        if p1.y < edge.p1.y and p1.y < edge.p2.y: continue
        if p1.y > edge.p1.y and p1.y > edge.p2.y: continue
        co0 = (ccw(p1, edge.p1, p2) == 0) and (edge.p1.x > p1.x)
        co1 = (ccw(p1, edge.p2, p2) == 0) and (edge.p2.x > p1.x)
        if co0 and co1: continue
        co_point = edge.p1 if co0 else edge.p2
        if co0 or co1:
            if edge.get_adjacent(co_point).y > p1.y:
                co_dir += 1
            else:
                co_dir -= 1
            if co_flag:
                if co_dir == 0:
                    intersect_count += 1
                co_flag = False
                co_dir = 0
            else:
                co_flag = True
        elif edge_intersect(p1, p2, edge):
            intersect_count += 1
    if intersect_count % 2 == 0:
        return False
    return True


def edge_in_polygon(p1, p2, graph):
    """Überprüft, ob Kante zwischen zwei Polygon-Punkten innerhalb eines Polygons liegt. """
    if p1.polygon_id != p2.polygon_id:
        return False
    if p1.polygon_id == -1 or p2.polygon_id == -1:
        return False
    mid_point = Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2)
    return polygon_crossing(mid_point, graph.polygons[p1.polygon_id])


def point_in_polygon(p, graph):
    """Überprüft, ob Punkt innerhalb irgendeines Polygons liegt. """
    for polygon in graph.polygons:
        if polygon_crossing(p, graph.polygons[polygon]):
            return polygon
    return -1

def edge_distance(p1, p2):
    """Euklidische Distanz zwischen zwei Punkten."""
    return sqrt((p2.x - p1.x)**2 + (p2.y - p1.y)**2)


def intersect_point(p1, p2, edge):
    """Schnittpunkt von p1p2 und edge."""
    if p1 in edge: return p1
    if p2 in edge: return p2
    if edge.p1.x == edge.p2.x:
        if p1.x == p2.x:
            return None
        pslope = (p1.y - p2.y) / (p1.x - p2.x)
        intersect_x = edge.p1.x
        intersect_y = pslope * (intersect_x - p1.x) + p1.y
        return Point(intersect_x, intersect_y)

    if p1.x == p2.x:
        eslope = (edge.p1.y - edge.p2.y) / (edge.p1.x - edge.p2.x)
        intersect_x = p1.x
        intersect_y = eslope * (intersect_x - edge.p1.x) + edge.p1.y
        return Point(intersect_x, intersect_y)

    pslope = (p1.y - p2.y) / (p1.x - p2.x)
    eslope = (edge.p1.y - edge.p2.y) / (edge.p1.x - edge.p2.x)
    if eslope == pslope:
        return None
    intersect_x = (eslope * edge.p1.x - pslope * p1.x + p1.y - edge.p1.y) / (eslope - pslope)
    intersect_y = eslope * (intersect_x - edge.p1.x) + edge.p1.y
    return Point(intersect_x, intersect_y)


def point_edge_distance(p1, p2, edge):
    """Distanz von p1 bis zum Schnittpunkt von p1p2 mit edge."""
    ip = intersect_point(p1, p2, edge)
    if ip is not None:
        return edge_distance(p1, ip)
    return 0


def angle(center, point):
    """Winkel zwischen Gerade cp und positiver x-Achse.
     ------p
     |   /
     |  /
    c|a/
    """
    dx = point.x - center.x
    dy = point.y - center.y
    if dx == 0:
        if dy < 0:
            return pi * 3 / 2
        return pi / 2
    if dy == 0:
        if dx < 0:
            return pi
        return 0
    if dx < 0:
        return pi + atan(dy / dx)
    if dy < 0:
        return 2 * pi + atan(dy / dx)
    return atan(dy / dx)


def angle2(point_a, point_b, point_c):
    """Implementierung des Cosinussatzes.
           c
         /  \
       /    B\
      a-------b
    """
    a = (point_c.x - point_b.x)**2 + (point_c.y - point_b.y)**2
    b = (point_c.x - point_a.x)**2 + (point_c.y - point_a.y)**2
    c = (point_b.x - point_a.x)**2 + (point_b.y - point_a.y)**2
    cos_value = (a + c - b) / (2 * sqrt(a) * sqrt(c))
    return acos(int(cos_value*T)/T2)


def ccw(A, B, C):
    """Ueberprueft, ob C gegen den Uhrzeigersinn bezueglich des Strahls AB liegt (1 wenn ja, -1 wenn im Uhrzeigersinn, 0 wenn auf dem Strahl) """
    #  Rounding this way is faster than calling round()
    area = int(((B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x))*T)/T2
    if area > 0: return 1
    if area < 0: return -1
    return 0


def on_segment(p, q, r):
    """Ueberprüft, ob q auf pr liegt."""
    if (q.x <= max(p.x, r.x)) and (q.x >= min(p.x, r.x)):
        if (q.y <= max(p.y, r.y)) and (q.y >= min(p.y, r.y)):
            return True
    return False


def edge_intersect(p1, q1, edge):
    """Ueberprueft ob p1q1 und edge sich schneiden.
    http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/"""
    p2 = edge.p1
    q2 = edge.p2
    o1 = ccw(p1, q1, p2)
    o2 = ccw(p1, q1, q2)
    o3 = ccw(p2, q2, p1)
    o4 = ccw(p2, q2, q1)

    # General case
    if (o1 != o2 and o3 != o4):
        return True
    # p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if o1 == 0 and on_segment(p1, p2, q1):
        return True
    # p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if o2 == 0 and on_segment(p1, q2, q1):
        return True
    # p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if o3 == 0 and on_segment(p2, p1, q2):
        return True
    # p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if o4 == 0 and on_segment(p2, q1, q2):
        return True
    return False


def insort(a, x):
    """Einfügen in den binaeren Suchbaum. """
    lo = 0
    hi = len(a)
    while lo < hi:
        mid = (lo+hi)//2
        if x < a[mid]: hi = mid
        else: lo = mid+1
    a.insert(lo, x)


def bisect(a, x):
    """Binaere Suche für den Suchbaum."""
    lo = 0
    hi = len(a)
    while lo < hi:
        mid = (lo+hi)//2
        if x < a[mid]: hi = mid
        else: lo = mid+1
    return lo


class EdgeKey(object):
    """Element des Suchbaums. """
    def __init__(self, p1, p2, edge):
        self.p1 = p1
        self.p2 = p2
        self.edge = edge

    def __eq__(self, other):
        if self.edge == other.edge:
            return True

    def __lt__(self, other):
        if self.edge == other.edge:
            return False
        if not edge_intersect(self.p1, self.p2, other.edge):
            return True
        self_dist = point_edge_distance(self.p1, self.p2, self.edge)
        other_dist = point_edge_distance(self.p1, self.p2, other.edge)
        if self_dist > other_dist:
            return False
        if self_dist < other_dist:
            return True
        # If the distance is equal, we need to compare on the edge angles.
        if self_dist == other_dist:
            if self.edge.p1 in other.edge:
                same_point = self.edge.p1
            elif self.edge.p2 in other.edge:
                same_point = self.edge.p2
            aslf = angle2(self.p1, self.p2, self.edge.get_adjacent(same_point))
            aot = angle2(self.p1, self.p2, other.edge.get_adjacent(same_point))
            if aslf < aot:
                return True
            return False

    def __repr__(self):
        reprstring = (self.__class__.__name__, self.edge, self.p1, self.p2)
        return "{}(Edge={!r}, p1={!r}, p2={!r})".format(*reprstring)
