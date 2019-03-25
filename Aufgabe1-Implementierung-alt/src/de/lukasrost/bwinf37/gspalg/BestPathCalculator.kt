package de.lukasrost.bwinf37.gspalg

import java.io.File

class BestPathCalculator {
    val WIDTH = 1200
    val HEIGHT = 1200
    fun readRunSave(input: File, output: File, svg: File, runExtension: Boolean){
        val io = IOHelper()
        val lee = LeeVisibilityAlgorithm()
        val svggen = SVGGenerator()
        val pp = io.read(input)
        val polygons = pp.first
        val lisa = pp.second

        val graph = lee.generateVisGraph(polygons, lisa)
        val dijkstraResult = graph.dijkstra(lisa)
        val dist = dijkstraResult.first
        val parent = dijkstraResult.second

        val genroute = genRoute()
        val route = genroute.first
        val ycoord = genroute.second

        //gen svg
        val svgpic = svggen.createSVG(WIDTH,HEIGHT,polygons,route,ycoord)
        io.writeImage(svg,svgpic)
        //gen output
        //io.write(output,route,//TODO)
    }

    fun genRoute() : Pair<ArrayList<Point>,Double> {
        //TODO
        return Pair(arrayListOf(Point("A",1,2)),1.0)
    }
}