#Imports
import argparse
import pyvisgraph as vg
import svggen
import os

def ensure_dir(file_path):
    directory = os.path.dirname(file_path)
    if not os.path.exists(directory):
        os.makedirs(directory)

#Commandline-Argumente parsen
parser = argparse.ArgumentParser(description="Lösung zu Lisa rennt, Aufgabe 1, Runde 2, 37. BwInf von Lukas Rost")

parser.add_argument('-i', action="store",dest="input",default="lisarennt1.txt",help="Eingabedatei")
parser.add_argument('-o',action="store",dest="output",default="lisarennt1_output.txt",help="Ausgabedatei")
parser.add_argument('-so', action="store",dest="svg",default="lisarennt1_svg.svg",help="SVG-Ausgabedatei")
parser.add_argument('-d',action="store_true",default=False,dest="debug",help="Debug-Ausgaben aktivieren")
parser.add_argument('-vlisa',action="store",dest="velocity_lisa",default=15,type=float,help="Erweiterung Geschwindigkeiten: Lisa in km/h")
parser.add_argument('-vbus',action="store",dest="velocity_bus",default=30,type=float,help="Erweiterung Geschwindigkeiten: Bus in km/h")
parser.add_argument('-minkowski',action="store",default=None,help="Erweiterung Minkowski-Summe: Eingabedatei (1 Polygon im gleichen Format wie in der normalen Eingabe)")

args = parser.parse_args()

#Maximale x und y für Darstellung
maxx = 0
maxy = 0

# Polygone einlesen
infile = open(args.input,'r')
numpoly = int(infile.readline())
polylist = []

for i in range(numpoly):
    pointlist = []
    line = infile.readline().split(" ")
    line = [int(x) for x in line]
    index = 1
    for j in range(line[0]):
        maxx = max(maxx,line[index])
        maxy = max(maxy,line[index+1])
        pointlist.append(vg.Point(line[index],line[index+1],polygon_id=i+1))
        index += 2
    polylist.append(pointlist)

#Lisas Position einlesen
pos = infile.readline().split(" ")
pos = [int(x) for x in pos]
lisa = vg.Point(pos[0],pos[1])
infile.close()

maxx = max(maxx,pos[0])
maxy = max(maxy,pos[1])

#Graph erstellen
graph = vg.VisGraph(args.velocity_lisa,args.velocity_bus,lisa)
graph.build(polylist)
#shortest = graph.shortest_path(lisa) #TODO change visgraph library

#Debug-Ausgaben
if args.debug:
    outpath = os.path.dirname(args.input) + "/out/debug/" + os.path.basename(args.input).split(".")[0]
    ensure_dir(outpath)
    graph.save(outpath + "_pickle.pk1")
    svgfile = open(outpath + "_visgraph.svg","w")
    svgfile.write(svggen.gen_vis_svg(graph.get_visgraph(),polylist,lisa,maxx+200,maxy+300))
    svgfile.close()
    

#Ausgabe
#TODO

#TODO Erweiterungen
