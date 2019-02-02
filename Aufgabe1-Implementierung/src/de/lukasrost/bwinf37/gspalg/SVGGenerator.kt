package de.lukasrost.bwinf37.gspalg

import org.jfree.graphics2d.svg.SVGGraphics2D
import java.awt.BasicStroke
import java.awt.Color
import java.awt.geom.Path2D

class SVGGenerator {
    fun createSVG(w: Int, h: Int, polys: ArrayList<Polygon>, route: List<Point>, ycoord : Double) : SVGGraphics2D{
        //TODO check if works
        val svg = SVGGraphics2D(w,h)
        svg.color = Color.BLACK
        svg.stroke = BasicStroke(
            10.0f,
            BasicStroke.CAP_SQUARE, BasicStroke.JOIN_ROUND
        )
        for(poly in polys){
            val awtp = java.awt.Polygon()
            for(pnt in poly.vertices) {
                awtp.addPoint(pnt.x,pnt.y)
            }
            svg.drawPolygon(awtp)
        }

        val path = Path2D.Double()
        path.moveTo(route[0].x.toDouble(),route[0].y.toDouble())
        for(i in 1 until route.size){
            path.lineTo(route[i].x.toDouble(),route[i].y.toDouble())
        }
        path.lineTo(0.0,ycoord)

        svg.color = Color.RED
        svg.draw(path)

        return svg
    }
}