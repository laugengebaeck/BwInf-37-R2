#Main program
import argparse
import pyvisgraph as vg

#Commandline-Argumente parsen
parser = argparse.ArgumentParser(description="Lösung zu Lisa rennt, Aufgabe 1, Runde 2, 37. BwInf von Lukas Rost")

parser.add_argument('-i', action="store",dest="input",default="lisarennt1.txt",help="Eingabedatei")
parser.add_argument('-o',action="store",dest="output",default="lisarennt1_output.txt",help="Ausgabedatei")
parser.add_argument('-so', action="store",dest="svg",default="lisarennt1_svg.svg",help="SVG-Ausgabedatei")
parser.add_argument('-d',action="store_true",default=False,dest="debug",help="Debug-Ausgaben aktivieren")
parser.add_argument('-vlisa',action="store",dest="velocity_lisa",default=15,type=float,help="Erweiterung Geschwindigkeiten: Lisa in km/h")
parser.add_argument('-vbus',action="store",dest="velocity_bus",default=30,type=float,help="Erweiterung Geschwindigkeiten: Bus in km/h")
parser.add_argument('-mline',action="store",type=float,help="Erweiterung Zielgerade: Anstieg m")
parser.add_argument('-nline',action="store",type=float,help="Erweiterung Zielgerade: y-Achsen-Abschnitt n")
parser.add_argument('-minkowski',action="store_true",default=False,help="Erweiterung Minkowski-Summe: Polygon muss in der letzten Zeile der Eingabe stehen")

args = parser.parse_args()

# Polygone einlesen
infile = open(args.input,'r')
numpoly = int(infile.readline())
polylist = []

for i in range(numpoly):
    pointlist = []
    line = infile.readline().split(" ")
    index = 1
    for j in range(line[0]):
        pointlist.append(vg.Point(line[index],line(index+1),polygon_id="P"+(j+1)))
        index += 2
    polylist.append(pointlist)

#Lisas Position einlesen
pos = infile.readline.split(" ")
infile.close()

#Graph erstellen
graph = vg.VisGraph()
graph.build(polylist)
shortest = graph.shortest_path(vg.Point(pos[0],pos[i],polygon_id="L")) #TODO change visgraph library

#Debug-Ausgaben
graph.save(args.input + "_pickle.pk1")

#Ausgabe
#TODO
#TODO Zeitmessungen

#TODO Erweiterungen
