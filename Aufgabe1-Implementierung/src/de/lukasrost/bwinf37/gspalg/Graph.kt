package de.lukasrost.bwinf37.gspalg

import java.util.*

class Graph {
    val pointMap: HashMap<Point,Int> = HashMap()
    var pointIndex = 0
    val adjacencyList: ArrayList<ArrayList<Pair<Int,Double>>> = ArrayList()

    fun addEdge(source: Point, destination: Point) {
        val weight = Math.sqrt((source.x - destination.x) * (source.x - destination.x) + (source.y - destination.y) * (source.y - destination.y).toDouble())
        // Punkte zu Transformations-Map hinzufügen, falls noch nicht geschehen
        if (!pointMap.containsKey(source)) {
            pointMap[source] = pointIndex
            pointIndex++
        }
        if (!pointMap.containsKey(destination)) {
            pointMap[destination] = pointIndex
            pointIndex++
        }
        // Kante von source zu destination
        adjacencyList[pointMap[source]!!].add(Pair(pointMap[destination]!!,weight))
        // Kante von destination zu source
        adjacencyList[pointMap[destination]!!].add(Pair(pointMap[source]!!,weight))
    }

    fun dijkstra(from: Point): Pair<List<Double>,List<Int>>{
        val nfrom = pointMap[from]!!
        val dist = MutableList(adjacencyList.size){if (it == nfrom) 0.0 else Double.MAX_VALUE}
        val parent = MutableList(adjacencyList.size){-1}
        val Q = PriorityQueue<Pair<Int,Double>>()
        Q.add(Pair(nfrom,0.0))
        while(!Q.isEmpty()){
            val u = Q.poll().first
            for (p in adjacencyList[u]){
                val v = p.first
                val w = p.second
                if(dist[u]+w < dist[v]){
                    dist[v] = dist[u]+w
                    parent[v] = u
                    Q.add(Pair(v,dist[v]))
                }
            }
        }
        return Pair(dist,parent)
    }
}