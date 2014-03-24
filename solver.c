#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "library.h"

typedef struct Walk {
    struct Walk *back;
    struct Walk **forward;
    Point       location;
} Walk;

Walk *walkMake(Point location, Walk *back) {
    Walk *walk = malloc(sizeof(Walk) * 1);
    walk->back = back;
    walk->forward = malloc(sizeof(Walk *) * 1);
    walk->location = location;

    return walk;
}

void walkGo(Walk *walk, Grid grid, Walk **success) {
    // Base case - end of labyrinth
    if (grid.self[walk->location.y][walk->location.x] == '-') {
        *success = walk;
    } else {
        //grid.self[walk->location.y][walk->location.x] = '.';
        //labyrinthPrint(grid);
        int     pointsCount = 0;
        Point *points = walkPossibilities(walk->location, grid, &pointsCount);

        int i;
        for (i = 0; i < pointsCount; i++) {
            if ((walk->back == NULL) || 
                (points[i].x != walk->back->location.x) ||
                (points[i].y != walk->back->location.y)) {

                Walk *forward = walkMake(points[i], walk);

                walk->forward = realloc(walk->forward, sizeof(Walk *) * (i+1));
                walk->forward[i] = forward;

                walkGo(forward, grid, success);
            }
        }
    }
}

int main(void) {
    // Decide which labyrinth to walk
    //char *labyrinth = labyrinthFile();

    // Open file and read it line by line
    //Grid grid = labyrinthFileRead(labyrinth);
    Grid grid = labyrinthFileRead("test.labyrinth");
    labyrinthPrint(grid);

    // Start/End points
    printf("start[%c] - (%d, %d)\n", gridCellPoint(grid, grid.start), grid.start.x, grid.start.y);
    printf("end[%c] - (%d, %d)\n", gridCellPoint(grid, grid.end), grid.end.x, grid.end.y);

    // Start walking
    Walk *walk = walkMake(grid.start, NULL);

    Walk *success = malloc(sizeof(Walk) * 1);
    walkGo(walk, grid, &success);
    printf("(%d, %d)\n", success->location.x, success->location.y);
    
    // TODO starting with the entry point create a tree of walks
    // TODO save pointer to exit node
    // TODO walk backwards from exit node to solution
    // TODO save array of vertices of the path back
    // TODO printf the path in reverse order
    // TODO create polyline object in SVG lingo

    return 0;
}
