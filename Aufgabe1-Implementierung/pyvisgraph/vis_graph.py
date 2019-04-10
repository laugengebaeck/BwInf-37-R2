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
from timeit import default_timer
from sys import stdout, version_info
from multiprocessing import Pool
from tqdm import tqdm
from warnings import warn

from pyvisgraph.graph import Graph, Edge, Point
from pyvisgraph.shortest_path import shortest_path
from pyvisgraph.visible_vertices import visible_vertices, point_in_polygon
from pyvisgraph.visible_vertices import closest_point

import math

PYTHON3 = version_info[0] == 3
if PYTHON3:
    xrange = range
    import pickle


class VisGraph(object):

    def __init__(self, vlisa,vbus,lisa_p):
        self.graph = None
        self.visgraph = None
        self.vlisa = vlisa
        self.vbus = vbus
        self.lisa_point = lisa_p

    def load(self, filename):
        """Sichtbarkeitsgraph laden. """
        with open(filename, 'rb') as load:
            self.graph, self.visgraph = pickle.load(load)

    def save(self, filename):
        """Sichtbarkeitsgraph speichern. """
        with open(filename, 'wb') as output:
            pickle.dump((self.graph, self.visgraph), output, -1)

    def build(self, input, status=True): 
        """Sichtbarkeitsgraph aus Liste von Polygonen generieren. Ein Polygon ist eine Liste von Punkten. """

        self.graph = Graph(input)
        self.visgraph = Graph([])

        points = self.graph.get_points()
        batch_size = 10 

        for batch in tqdm([points[i:i + batch_size]
                            for i in xrange(0, len(points), batch_size)],
                        disable=not status):
            for edge in self._vis_graph(self.graph, batch):
                self.visgraph.add_edge(edge)

    def find_visible(self, point):
        """Von einem Punkt aus sichtbare Punkte finden."""

        return visible_vertices(point, self.graph)

    def shortest_path(self):
        """Kürzesten Pfad (als Punkte-Liste) vom Lisa-Punkt aus finden. """

        return shortest_path(self.visgraph, self.lisa_point, self.vlisa, self.vbus)

    def get_visgraph(self):
        return self.visgraph

    def _vis_graph_wrapper(self, args):
        try:
            return self._vis_graph(*args)
        except KeyboardInterrupt:
            pass

    def _vis_graph(self, graph, points):
        """Sichtbarkeitsgraph für points berechnen. Dabei wird für jeden Punkt der Companion-Punkt berechnet. Anschließend wird dieser Punkt bei der Berechnung der visible vertices mit betrachtet. """
        visible_edges = []
        for p1 in points:
            dest_point_y = p1.y + (self.vlisa / self.vbus) * (1 / math.sqrt(1 - ((self.vlisa * self.vlisa) / (self.vbus * self.vbus)))) * p1.x
            dest_point = Point(0,round(dest_point_y,2), polygon_id="Y")
            for p2 in visible_vertices(p1, graph, origin=self.lisa_point,destination=dest_point):
                visible_edges.append(Edge(p1, p2))
        return visible_edges
