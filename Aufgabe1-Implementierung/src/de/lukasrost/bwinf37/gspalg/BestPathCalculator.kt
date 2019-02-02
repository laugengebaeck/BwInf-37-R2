package de.lukasrost.bwinf37.gspalg

import java.io.File

class BestPathCalculator {
    fun readRunSave(input: String, output: String, svg: String, runExtension: Boolean){
        val io = IOHelper()
        val lee = LeeVisibilityAlgorithm()
        val pp = io.read(File(input))
        val polygons = pp.first
        val lisa = pp.second

        val graph = lee.generateVisGraph(polygons, lisa)
        val dijkstraResult = graph.dijkstra(lisa)
        val dist = dijkstraResult.first
        val parent = dijkstraResult.second

        //gen svg
        //gen output
        //save both
    }
}