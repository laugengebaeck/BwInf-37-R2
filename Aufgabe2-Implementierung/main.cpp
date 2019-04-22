#include<bits/stdc++.h>
#include"argparse.hpp"
#include"triangleAlgorithm.cpp"

using namespace std;

// Eingabedatei einlesen
vector<Triangle> readInput(string inputFile){
    ifstream in(inputFile);
    int numtri,three;
    in >> numtri;
    vector<Triangle> triangles;

    for(int i=0; i<numtri; i++){
        in >> three; //"3" einlesen
        double x1,y1,x2,y2,x3,y3;
        in >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
        Point p1 = Point(x1,y1), p2 = Point(x2,y2), p3 = Point(x3,y3);
        triangles.push_back(Triangle(p1, p2, p3,i+1));
    }

    return triangles;
}

// Ausgabedatei generieren
void writeOutput(vector<Triangle> triangles, string outputFile, double maxDistance){
    ofstream out(outputFile);
    out << "Gesamtabstand: " << fixed << setprecision(3) << maxDistance << " Meter\n";
    out << "Platzierung der Dreiecke:\n";
    for(auto tri : triangles){
        out << "D" << tri.id << " ";
        out << fixed << setprecision(3) << tri.points[0].x << " " << abs(tri.points[0].y) << " ";
        out << fixed << setprecision(3) << tri.points[1].x << " " << abs(tri.points[1].y) << " ";
        out << fixed << setprecision(3) << tri.points[2].x << " " << abs(tri.points[2].y) << "\n";
    }
}

// SVG-Datei aus den Dreiecken generieren
void writeSVG(vector<Triangle> triangles, string svgFile){
    ofstream out(svgFile);
    out << "<svg version=\"1.1\" viewBox=\"0 0 820 620\" xmlns=\"http://www.w3.org/2000/svg\"> \n <g transform=\"scale(1 -1)\"> \n <g transform=\"translate(0 -600)\" fill=\"#ffcc99\"> \n <line id=\"x\" x1=\"0\" x2=\"820\" y1=\"0\" y2=\"0\" stroke=\"#000000\"/>\n"; //ViewBox ändern!
    for(auto tri: triangles){
        out << "<polygon id=\"D" << tri.id << "\" ";
        out << "points=\"" << fixed << setprecision(3) << tri.points[0].x << " " << abs(tri.points[0].y) << " ";
        out << fixed << setprecision(3) << tri.points[1].x << " " << abs(tri.points[1].y) << " ";
        out << fixed << setprecision(3) << tri.points[2].x << " " << abs(tri.points[2].y) << "\" />\n";
    }
    out << "</g> \n </g> \n </svg>";
}

int main(int argc, const char** argv){
    // Argumente  konfigurieren & einlesen
    ArgumentParser parser;
    parser.addArgument("-i","--input",1,false);
    parser.addArgument("-s","--svg",1,false);
    parser.addArgument("-o","--output",1,false);
    parser.parse(argc,argv);

    string input = parser.retrieve<string>("input");
    string svg = parser.retrieve<string>("svg");
    string output = parser.retrieve<string>("output");
    
    // Eingabe einlesen, Algorithmus ausführen, Ausgaben generieren
    double maxDist = 0;
    vector<Triangle> newTriangles;
    vector<Triangle> theTriangles = readInput(input);
    tie(newTriangles,maxDist) = doAlgorithm(theTriangles);
    writeOutput(newTriangles,output,maxDist);
    writeSVG(newTriangles,svg);

    return 0;
}