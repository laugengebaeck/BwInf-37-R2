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
from heapq import heapify, heappush, heappop
from pyvisgraph.visible_vertices import edge_distance
from pyvisgraph.graph import Point
import math

try:
    dict.iteritems
except AttributeError:
    # Python 3
    def iteritems(d):
        return iter(d.items())
else:
    # Python 2
    def iteritems(d):
        return d.iteritems()

#Implementierung des Dijkstra-Algorithmus
#mit einem Priority-Dict (aehnlich Priority-Queue) umgesetzt
def dijkstra(graph, origin):
    D = {} #Distanz
    P = {} #Elternknoten
    Q = priority_dict()
    Q[origin] = 0

    for v in Q:
        D[v] = Q[v]

        edges = graph[v]
        for e in edges:
            w = e.get_adjacent(v)
            elength = D[v] + edge_distance(v, w)
            if w in D:
                if elength < D[w]:
                    raise ValueError
            elif w not in Q or elength < Q[w]:
                Q[w] = elength
                P[w] = v
    return (D, P)

#Kuerzesten Weg vom Startpunkt zur y-Achse bestimmen
#Idee ist in Kapitel 1.5 beschrieben
def shortest_path(graph, origin, vlisa, vbus):
    #Dijkstra für alle Knoten ausführen
    D, P = dijkstra(graph, origin)
    mintime = -math.inf
    minpoint = Point(0,0)
    min_bus_time = 0.0
    #Besten Companion-Punkt bestimmen...
    for point in graph.get_points():
        if point.x == 0:
            bus_time = point.y / vbus
            lisa_time = D[point] / vlisa
            #... mithilfe der spätesten Startzeit
            #Zeitangabe: negativ vor 7.30, positiv danach
            total_time = bus_time - lisa_time
            if(total_time > mintime):
                mintime = round(total_time,2)
                minpoint = point
                min_bus_time = round(bus_time,2)
    #Pfad zum Punkt finden und zurückgeben
    path = []
    destination = minpoint
    while 1:
        path.append(destination)
        if destination == origin: break
        destination = P[destination]
    path.reverse()
    return path,mintime,min_bus_time, minpoint,round(D[minpoint],2)

#Priority-Dict als Implementierung einer Priority-Queue
class priority_dict(dict):
    """Dictionary that can be used as a priority queue.

    Keys of the dictionary are items to be put into the queue, and values
    are their respective priorities. All dictionary methods work as expected.
    The advantage over a standard heapq-based priority queue is that priorities
    of items can be efficiently updated (amortized O(1)) using code as
    'thedict[item] = new_priority.'

    Note that this is a modified version of
    https://gist.github.com/matteodellamico/4451520 where sorted_iter() has
    been replaced with the destructive sorted iterator __iter__ from
    https://gist.github.com/anonymous/4435950
    """
    def __init__(self, *args, **kwargs):
        super(priority_dict, self).__init__(*args, **kwargs)
        self._rebuild_heap()

    def _rebuild_heap(self):
        self._heap = [(v, k) for k, v in iteritems(self)]
        heapify(self._heap)

    def smallest(self):
        heap = self._heap
        v, k = heap[0]
        while k not in self or self[k] != v:
            heappop(heap)
            v, k = heap[0]
        return k

    def pop_smallest(self):
        heap = self._heap
        v, k = heappop(heap)
        while k not in self or self[k] != v:
            v, k = heappop(heap)
        del self[k]
        return k

    def __setitem__(self, key, val):
        super(priority_dict, self).__setitem__(key, val)

        if len(self._heap) < 2 * len(self):
            heappush(self._heap, (val, key))
        else:
            self._rebuild_heap()

    def setdefault(self, key, val):
        if key not in self:
            self[key] = val
            return val
        return self[key]

    def update(self, *args, **kwargs):
        super(priority_dict, self).update(*args, **kwargs)
        self._rebuild_heap()

    def __iter__(self):
        def iterfn():
            while len(self) > 0:
                x = self.smallest()
                yield x
                del self[x]
        return iterfn()
