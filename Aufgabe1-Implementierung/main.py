#Imports
import argparse
import os

import pyvisgraph as vg
import svggen
import minkowski

def ensure_dir(file_path):
    directory = os.path.dirname(file_path)
    if not os.path.exists(directory):
        os.makedirs(directory)

def numtotime(num):
    hours = num // 3600
    minutes = (num % 3600) // 60
    seconds = (num % 3600) % 60
    return hours, minutes, seconds

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

#Geschwindigkeiten in m/s umrechnen
real_v_lisa = args.velocity_lisa / 3.6
real_v_bus = args.velocity_bus / 3.6

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
        pointlist.append(vg.Point(line[index],line[index+1],polygon_id=("P" + str(i+1))))
        index += 2
    polylist.append(pointlist)

#Lisas Position einlesen
pos = infile.readline().split(" ")
pos = [int(x) for x in pos]
lisa = vg.Point(pos[0],pos[1],polygon_id="L")
infile.close()

maxx = max(maxx,pos[0])
maxy = max(maxy,pos[1])

#Erweiterung Minkowski-Summe
if args.minkowski is not None:
    minfile = open(args.minkowski,'r')
    lisa_poly = []
    line = minfile.readline().split(" ")
    minfile.close()
    line = [int(x) for x in line]
    index = 1
    for j in range(line[0]):
        lisa_poly.append(vg.Point(line[index],line[index+1]))
        index += 2
    polylist = minkowski.minkowski_sum_list(polylist,lisa_poly)

#Graph erstellen und Algorithmus ausführen
graph = vg.VisGraph(real_v_lisa,real_v_bus,lisa)
graph.build(polylist)
path,mintime,min_bus_time, minpoint,dist_minpoint = graph.shortest_path()
mintime = abs(mintime)

#Debug-Ausgaben
if args.debug:
    outpath = os.path.dirname(args.input) + "/out/debug/" + os.path.basename(args.input).split(".")[0]
    ensure_dir(outpath)
    graph.save(outpath + "-pickle.pk1")
    svgfile = open(outpath + "-visgraph.svg","w")
    svgfile.write(svggen.gen_vis_svg(graph.get_visgraph(),polylist,lisa,maxx+200,maxy+500))
    svgfile.close()
    

#Ausgabe SVG
svgfile = open(args.svg,"w")
svgfile.write(svggen.gen_output_svg(path,polylist,lisa,maxx+200,maxy+500))
svgfile.close()

#Ausgabe Text
outtext = ""
hours, minutes, seconds = numtotime(mintime)
hours = 7 - hours
minutes = 30 - minutes
bhours, bminutes, bseconds = numtotime(min_bus_time)
bhours = 7 + bhours
bminutes = 30 + bminutes
outtext += "Lisa startet um {:02d}:{:02d}:{:02d} und erreicht den Bus um {:02d}:{:02d}:{:02d}.\n".format(int(round(hours)), int(round(minutes)), int(round(seconds)),int(round(bhours)), int(round(bminutes)), int(round(bseconds)))
outtext += "Sie trifft bei der y-Koordinate {} auf den Bus.\n".format(minpoint.y)
outtext += "Die Route dauert {:0.2f} Minuten und ist {:0.2f} Meter lang.\n".format(dist_minpoint/(real_v_lisa*60),dist_minpoint)
outtext += "Die Route besteht aus folgenden Punkten:\n"
for point in path:
    outtext += "{} {} {}\n".format(point.x,point.y,point.real_polygon_id)

outfile = open(args.output,"w")
outfile.write(outtext)
outfile.close()

#TODO alles kommentieren + testen
