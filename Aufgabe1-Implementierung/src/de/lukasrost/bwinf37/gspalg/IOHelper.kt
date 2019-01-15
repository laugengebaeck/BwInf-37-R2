package de.lukasrost.bwinf37.gspalg

import javafx.embed.swing.SwingFXUtils
import javafx.scene.image.WritableImage
import javafx.scene.canvas.Canvas
import java.io.File
import java.lang.StringBuilder
import javax.imageio.ImageIO

class IOHelper {
    fun read(f: File): Pair<ArrayList<Polygon>,Pair<Int,Int>>{
        val reader = f.inputStream().bufferedReader()
        val p = reader.readLine().toInt()
        val polylist = arrayListOf<Polygon>()

        for (i in 0 until p){
            val pread = reader.readLine().split(" ")
            val n =  pread[0].toInt()
            val poly = Polygon(n, arrayListOf())

            for (j in 0 until n){
                val x = pread[2*j+1].toDouble()
                val y = pread[2*j+2].toDouble()
                poly.vertices.add(Pair(x,y))
            }
            polylist.add(poly)
        }

        val lread = reader.readLine().split(" ")
        val lisa = Pair(lread[0].toInt(),lread[1].toInt())

        return Pair(polylist,lisa)
    }

    fun write(file: File, route: List<Pair<Double,Double>>){
        val sb = StringBuilder()
        for(pnt in route){
            sb.append(pnt.first)
            sb.append(" ")
            sb.append(pnt.second)
            sb.append("\n")
        }
        val text = sb.toString()
        file.writeText(text)
    }

    fun writeImage(file: File, canvas: Canvas, w:Int,h:Int){
        val wim = WritableImage(w,h)
        canvas.snapshot(null,wim)
        try {
            ImageIO.write(SwingFXUtils.fromFXImage(wim,null),"png",file)
        } catch (e: Exception){
            e.printStackTrace()
        }
    }
}