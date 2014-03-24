#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct DOMSvg {
    int viewBoxWidth;
    int viewBoxHeight;
} DOMSvg;

typedef struct DOMPolygon {
    char *points;
    char *fill;
} DOMPolygon;

typedef struct DOMPolyline {
    char *id;
    char *fill;
    char *stroke;
    char *points;
} DOMPolyline;

DOMSvg domSvg(int w, int h);
char *rawSvg(DOMSvg svg);

DOMPolygon domPolygon(char *points, char *fill);
char *rawPolygon(DOMPolygon p);

DOMPolyline domPolyline(char *id, char *fill, char *stroke, char *points);
char *rawPolyline(DOMPolyline p);
