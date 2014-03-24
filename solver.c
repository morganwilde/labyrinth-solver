#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "library.h"
#include "dom.h"

int main(void) {
    // Decide which labyrinth to walk
    char *labName = malloc(sizeof(char) * 0);
    char *labyrinth = labyrinthFile(labName);
    //char labyrinth[] = "test.labyrinth";

    // Open file and read it line by line
    Grid grid = labyrinthFileRead(labyrinth);
    labyrinthPrint(grid);

    // Start/End points
    printf("start[%c] - (%d, %d)\n", gridCellPoint(grid, grid.start), grid.start.x, grid.start.y);
    printf("end[%c] - (%d, %d)\n", gridCellPoint(grid, grid.end), grid.end.x, grid.end.y);

    // Start walking
    Walk *walk = walkMake(grid.start, NULL);

    Walk *success = malloc(sizeof(Walk) * 1);
    walkGo(walk, grid, &success);
    
    // Print solution
    Point   *solutionBackwards = malloc(sizeof(Point) * 0);
    int     vertices = 0;
    Point   *sorted = malloc(sizeof(Point) * 0);
    solution(success, &solutionBackwards, &vertices);
    solutionReverse(solutionBackwards, &sorted, vertices);

    // Store for printing
    char *polylinePoints = malloc(sizeof(char) * 0);

    solutionPrint(sorted, &polylinePoints, vertices);

    DOMSvg      svg = domSvg(grid.length, grid.lines*2);
    DOMPolyline polylineSolution = domPolyline("solution", "none", "#FF0000", "");
    DOMPolyline polylineStorage = domPolyline("storage", "none", "", polylinePoints);

    // Create an .html file
    char htmlFilename[512];
    strcat(htmlFilename, labName);
    strcat(htmlFilename, ".html");

    FILE *html = fopen(htmlFilename, "wb");

    if (!html)
        return 1;

    // Assemble the parts
    FILE *templateTop = fopen("html/top.html.part", "r");
    FILE *templateBottom = fopen("html/bottom.html.part", "r");

    char c;

    while ((c = fgetc(templateTop)) != EOF) {
        fputc(c, html);
    }

    // Path finder
    fprintf(html, "%s\n", rawSvg(svg));

    // Draw the labyrinth
    char labPoints[64];
    int k, l;
    for (k = 0; k < grid.lines; k++) {
        for (l = 0; l < grid.length; l++) {
            if (grid.self[k][l] == ' ' || grid.self[k][l] == '-' || grid.self[k][l] == '+') {
                sprintf(labPoints, "%.2f,%.2f %.2f,%.2f %.2f,%.2f %.2f,%.2f",
                    l-0.9, (k-0.5)*2,
                    l-0.9, (k+0.5)*2,
                    l+0.9, (k+0.5)*2,
                    l+0.9, (k-0.5)*2);

                DOMPolygon bg = domPolygon(labPoints, "#FFFFFF");
                if (grid.self[k][l] == '-')
                    bg = domPolygon(labPoints, "#71AD2B");
                if (grid.self[k][l] == '+')
                    bg = domPolygon(labPoints, "#FF6840");

                fprintf(html, "%s\n", rawPolygon(bg));
            }
        }
    }

    fprintf(html, "%s\n", rawPolyline(polylineSolution));
    fprintf(html, "%s\n", rawPolyline(polylineStorage));

    while ((c = fgetc(templateBottom)) != EOF) {
        fputc(c, html);
    }

    fclose(templateTop);
    fclose(templateBottom);
    fclose(html);
	
    return 0;
}
