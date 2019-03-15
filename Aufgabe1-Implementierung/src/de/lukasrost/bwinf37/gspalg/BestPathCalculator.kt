package de.lukasrost.bwinf37.gspalg

import java.io.File

class BestPathCalculator {
    val WIDTH = 1200
    val HEIGHT = 1200
    fun readRunSave(input: String, output: String, svg: String, runExtension: Boolean){
        val io = IOHelper()
        val lee = LeeVisibilityAlgorithm()
        val svggen = SVGGenerator()
        val pp = io.read(File(input))
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
        io.writeImage(File(svg),svgpic)
        //gen output
        io.write(File(output),route,//TODO)
    }

    fun genRoute() : Pair<ArrayList<Point>,Double> {
        //TODO
    }
}