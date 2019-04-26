# SVG des Sichtbarkeitsgraphs erzeugen
def gen_vis_svg(visgraph,polys,lisa,maxx, maxy):
    svgtext = ''' <svg version="1.1" viewBox="0 0 ''' + str(maxx) + " " + str(maxy) + ''' " xmlns="http://www.w3.org/2000/svg">
    <g transform="scale(1 -1)">
    <g transform="translate(0 -''' + str(maxy) + ''')">
	    <line id="y" x1="0" x2="0" y1="0" y2=" '''+str(maxy)+'''" fill="none" stroke="#212121" stroke-width="3"/> \n'''
    for index,polygon in enumerate(polys):
        svgtext += '''<polygon id="P'''+str(index+1)+'''" points="'''
        for point in polygon:
            svgtext += str(point.x) + " "
            svgtext += str(point.y) + " "
        svgtext += '''" fill="#6B6B6B" stroke="#212121" stroke-width="2"/>\n'''
    svgtext += ''' <circle id="L" cx="''' + str(lisa.x) + '''" cy="''' + str(lisa.y) + '''" r="10" fill="#F42121" stroke="#000080" stroke-width="1"/>\n '''
    index = 1
    for e in visgraph.edges:
        svgtext += '''<polyline id="R''' + str(index) + '''" points="'''
        svgtext += str(e.p1.x)  + " "
        svgtext += str(e.p1.y)  + " "
        svgtext += str(e.p2.x)  + " "
        svgtext += str(e.p2.y)  + " "
        svgtext += '''" fill="none" stroke="#000080" stroke-width="4"/>\n'''
        index += 1
    svgtext += '''</g>
    </g>
    </svg>'''
    return svgtext

# Ausgabe-SVG mit der Route erzeugen
def gen_output_svg(route,polys,lisa,maxx, maxy):
    svgtext = ''' <svg version="1.1" viewBox="0 0 ''' + str(maxx) + " " + str(maxy) + ''' " xmlns="http://www.w3.org/2000/svg">
    <g transform="scale(1 -1)">
    <g transform="translate(0 -''' + str(maxy) + ''')">
	    <line id="y" x1="0" x2="0" y1="0" y2=" '''+str(maxy)+'''" fill="none" stroke="#212121" stroke-width="3"/> \n'''
    for index,polygon in enumerate(polys):
        svgtext += '''<polygon id="P'''+str(index+1)+'''" points="'''
        for point in polygon:
            svgtext += str(point.x) + " "
            svgtext += str(point.y) + " "
        svgtext += '''" fill="#6B6B6B" stroke="#212121" stroke-width="2"/>\n'''
    svgtext += ''' <circle id="L" cx="''' + str(lisa.x) + '''" cy="''' + str(lisa.y) + '''" r="10" fill="#F42121" stroke="#000080" stroke-width="1"/>\n '''
    svgtext += '''<polyline id="R" points="'''
    for p in route:
        svgtext += str(p.x)  + " "
        svgtext += str(p.y)  + " "
    svgtext += '''" fill="none" stroke="#000080" stroke-width="4"/>\n'''
    svgtext += '''</g>
    </g>
    </svg>'''
    return svgtext
