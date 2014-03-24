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

void solutionPrint(Point *points, char **attribute, int count) {
    int i;
    for (i = 0; i < count; i++) {
        char temp[32];
        sprintf(temp, "%d,%d ", points[i].x, points[i].y*2);

        attribute[0] = realloc(*attribute, sizeof(char) * (strlen(*attribute)+strlen(temp)+1));
        strcat(*attribute, temp);
    }
}

void solution(Walk *walk, Point **points, int *vertices) {
    if (walk) {
        int i = *vertices;
        points[0] = realloc(points[0], sizeof(Point) * (i+1));
        points[0][i] = walk->location;
        i++;
        *vertices = i;

        solution(walk->back, points, vertices);
    }
}

void solutionReverse(Point *points, Point **storage, int vertices) {
    storage[0] = realloc(*storage, sizeof(Point) * vertices);

    int i, j;
    for (i = vertices-1, j = 0; i >= 0; i--, j++) {
        storage[0][j] = points[i];
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
    
    // Print solution
    Point   *solutionBackwards = malloc(sizeof(Point) * 0);
    int     vertices = 0;
    Point   *sorted = malloc(sizeof(Point) * 0);
    solution(success, &solutionBackwards, &vertices);
    solutionReverse(solutionBackwards, &sorted, vertices);


    char    walkPolyOpen[] = "<polyline fill=\"none\" stroke=\"#000000\" points=\"",
            *walkPolyPoints = malloc(sizeof(char) * 0),
            walkPolyClose[] = "\" />";

    solutionPrint(sorted, &walkPolyPoints, vertices);
    //walkSolutionPrint(success, &walkPolyPoints);
    printf("%s%s%s\n", walkPolyOpen, walkPolyPoints, walkPolyClose);


    return 0;
}
