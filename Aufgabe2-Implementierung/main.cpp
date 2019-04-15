#include<bits/stdc++.h>
#include"argparse.hpp"
#include"triangleAlgorithm.cpp"

using namespace std;

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

void writeOutput(vector<Triangle> triangles, string outputFile, double maxDistance){
    ofstream out(outputFile);
    out << "Gesamtabstand: " << fixed << setprecision(3) << maxDistance << " Meter\n";
    out << "Platzierung der Dreiecke:\n";
    for(auto tri : triangles){
        out << "D" << tri.id << " ";
        out << fixed << setprecision(3) << tri.point1.x << " " << tri.point1.y << " ";
        out << fixed << setprecision(3) << tri.point2.x << " " << tri.point2.y << " ";
        out << fixed << setprecision(3) << tri.point3.x << " " << tri.point3.y << "\n";
    }
}

void writeSVG(vector<Triangle> triangles, string svgFile){
    ofstream out(svgFile);
    out << "<svg version=\"1.1\" viewBox=\"0 0 820 620\" xmlns=\"http://www.w3.org/2000/svg\"> \n <g transform=\"scale(1 -1)\"> \n <g transform=\"translate(0 -600)\" fill=\"#ffcc99\"> \n <line id=\"x\" x1=\"0\" x2=\"820\" y1=\"0\" y2=\"0\" stroke=\"#000000\"/>";
    for(auto tri: triangles){
        out << "<polygon id=\"D" << tri.id << "\" ";
        out << "points=\"" << fixed << setprecision(3) << tri.point1.x << " " << tri.point1.y << " ";
        out << fixed << setprecision(3) << tri.point2.x << " " << tri.point2.y << " ";
        out << fixed << setprecision(3) << tri.point3.x << " " << tri.point3.y << "\" />\n";
    }
    out << "</g> \n </g> \n </svg>";
}

int main(int argc, const char** argv){
    ArgumentParser parser;
    parser.addArgument("-i","--input",1,false);
    parser.addArgument("-s","--svg",1,false);
    parser.addArgument("-o","--output",1,false);
    parser.addArgument("-d");
    parser.parse(argc,argv);

    string input = parser.retrieve<string>("input");
    string svg = parser.retrieve<string>("svg");
    string output = parser.retrieve<string>("output");
    bool debug = parser.exists("d");
    
    double maxDist = 0;
    vector<Triangle> newTriangles;
    vector<Triangle> theTriangles = readInput(input);
    tie(newTriangles,maxDist) = doAlgorithm(theTriangles,debug);
    writeOutput(newTriangles,output,maxDist);
    writeSVG(newTriangles,svg);

    return 0;
}