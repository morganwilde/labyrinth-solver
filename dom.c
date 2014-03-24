#include "dom.h"

DOMSvg domSvg(int w, int h) {
    DOMSvg svg;
    svg.viewBoxWidth = w;
    svg.viewBoxHeight = h;

    return svg;
}
char *rawSvg(DOMSvg svg) {
    int     chars = 0;
    char    contentTemps[] = "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"\">",
            viewBox[32];
    
    sprintf(viewBox, "%d %d %d %d", 0, 0, svg.viewBoxWidth, svg.viewBoxHeight);

    chars += strlen(contentTemps);
    chars += strlen(viewBox);

    char *contents = malloc(sizeof(char) * (chars+1));
	contents[0] = '\0';

    strcat(contents, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"");
    strcat(contents, viewBox);
    strcat(contents, "\">");

    return contents;
}

DOMPolygon domPolygon(char *points, char *fill) {
    DOMPolygon polygon;
    polygon.points = points;
    polygon.fill = fill;

    return polygon;
}

char *rawPolygon(DOMPolygon p) {
    int     chars = 0;
    char    contentTemp[] = "<polygon points=\"\" fill=\"\" />";

    chars += strlen(contentTemp);
    chars += strlen(p.points);
    chars += strlen(p.fill);

    char *contents = malloc(sizeof(char) * (chars+1));

	contents[0] = '\0';

    strcat(contents, "<polygon points=\"");
    strcat(contents, p.points);
    strcat(contents, "\" fill=\"");
    strcat(contents, p.fill);
    strcat(contents, "\" />");

	//printf("{%s}\n", contents);

    return contents;
}

DOMPolyline domPolyline(char *id, char *fill, char *stroke, char *points) {
    DOMPolyline polyline;
    polyline.id = id;
    polyline.fill = fill;
    polyline.stroke = stroke;
    polyline.points = malloc(sizeof(char) * strlen(points));

	//printf("[%s]\n", points);
	
	polyline.points[0] = '\0';
	strcpy(polyline.points, points);

    return polyline;
}

char *rawPolyline(DOMPolyline p) {
    int     chars = 0;
    char    contentTemps[] = "<polyline id=\"\" fill=\"\" stroke=\"\" points=\"\" />";

    chars += strlen(contentTemps);
    chars += strlen(p.fill);
    chars += strlen(p.stroke);
    chars += strlen(p.points);
    
    char *contents = malloc(sizeof(char) * (chars+1));
	contents[0] = '\0';

    strcat(contents, "<polyline id=\"");
    strcat(contents, p.id);
    strcat(contents, "\" fill=\"");
    strcat(contents, p.fill);
    strcat(contents, "\" stroke=\"");
    strcat(contents, p.stroke);
    strcat(contents, "\" points=\"");
    strcat(contents, p.points);
    strcat(contents, "\" />");

    return contents;
}
