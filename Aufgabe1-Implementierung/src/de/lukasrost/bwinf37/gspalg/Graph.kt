package de.lukasrost.bwinf37.gspalg

class Graph {
    val adjacencyMap: HashMap<Int, HashSet<Pair<Int,Double>>> = HashMap()

    fun addEdge(sourceVertex: Int, destinationVertex: Int, weight: Double) {
        // Add edge to source vertex / node.
        adjacencyMap
            .computeIfAbsent(sourceVertex) { HashSet() }
            .add(Pair(destinationVertex,weight))
        // Add edge to destination vertex / node.
        adjacencyMap
            .computeIfAbsent(destinationVertex) { HashSet() }
            .add(Pair(sourceVertex,weight))
    }

    override fun toString(): String = StringBuffer().apply {
        for (key in adjacencyMap.keys) {
            append("$key -> ")
            append(adjacencyMap[key]?.joinToString(", ", "[", "]\n"))
        }
    }.toString()

    fun dijkstra(from: Int): List<Double>{
        val dist = List<Double>(adjacencyMap.size){if (it == from) 0.0 else Double.MAX_VALUE}
        //do dijkstra
        return dist
    }
}