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

Walk *walkMake(Point location) {
    Walk *walk = malloc(sizeof(Walk) * 1);
    walk->back = NULL;
    walk->location = location;

    return walk;
}

int main(void) {
    // Decide which labyrinth to walk
    //char *labyrinth = labyrinthFile();

    // open file and read it line by line
    //Grid grid = labyrinthFileRead(labyrinth);
    Grid grid = labyrinthFileRead("test.labyrinth");
    labyrinthPrint(grid);

    printf("start[%c] - (%d, %d)\n", gridCellPoint(grid, grid.start), grid.start.x, grid.start.y);
    printf("end[%c] - (%d, %d)\n", gridCellPoint(grid, grid.end), grid.end.x, grid.end.y);

    Walk *walk = walkMake(grid.start);

    // Tests
    //

    // TODO starting with the entry point create a tree of walks
    // TODO save pointer to exit node
    // TODO walk backwards from exit node to solution
    // TODO save array of vertices of the path back
    // TODO printf the path in reverse order
    // TODO create polyline object in SVG lingo

    return 0;
}
