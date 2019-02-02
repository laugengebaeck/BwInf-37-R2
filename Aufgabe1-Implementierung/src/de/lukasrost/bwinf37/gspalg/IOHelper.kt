package de.lukasrost.bwinf37.gspalg

import org.jfree.graphics2d.svg.SVGGraphics2D
import org.jfree.graphics2d.svg.SVGUtils
import java.io.File
import java.lang.StringBuilder

class IOHelper {
    fun read(f: File): Pair<ArrayList<Polygon>,Point>{
        val reader = f.inputStream().bufferedReader()
        val p = reader.readLine().toInt()
        val polylist = arrayListOf<Polygon>()

        for (i in 0 until p){
            val pread = reader.readLine().split(" ")
            val n =  pread[0].toInt()
            val polyId = "P${i+1}"
            val poly = Polygon(polyId,n, arrayListOf())

            for (j in 0 until n){
                val x = pread[2*j+1].toInt()
                val y = pread[2*j+2].toInt()
                poly.vertices.add(Point(polyId, x, y))
            }
            polylist.add(poly)
        }

        val lread = reader.readLine().split(" ")
        val lisa = Point("L", lread[0].toInt(),lread[1].toInt())

        return Pair(polylist,lisa)
    }

    fun write(file: File, route: List<Point>, start : String, dest: String, ycoord : Double, duration: Double, length: Double){
        val sb = StringBuilder()
        sb.append("Walk from ",start," until ",dest,"\n")
        sb.append("Y-coordinate of the meeting: ",String.format("%.3f", ycoord),"\n")
        sb.append("Duration: ",duration, " minutes, Length: ",length," metres\n")
        sb.append("Points of the route (x,y,polygon):","\n")
        for(pnt in route){
            sb.append(String.format("%.3f", pnt.x)," ",String.format("%.3f", pnt.y)," ",pnt.polyId,"\n")
        }
        sb.append("A visualisation was saved at the specified location.\n")
        val text = sb.toString()
        file.writeText(text)
    }

    fun writeImage(file: File, svgGraphics2D: SVGGraphics2D){
        SVGUtils.writeToSVG(file,svgGraphics2D.svgElement)
    }
}