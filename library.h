#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define FONT_N "\x1B[0m"
#define FONT_U "\x1B[4m"

typedef struct Point {
    int     x;
    int     y;
} Point;

typedef struct Grid {
    char    **self;
    int     lines;
    int     length;
    Point   start;
    Point   end;
} Grid;

typedef struct Walk {
    struct Walk *back;
    struct Walk **forward;
    Point       location;
} Walk;

Point pointMake(int x, int y);
char gridCell(Grid grid, int x, int y);
char gridCellPoint(Grid grid, Point p);
char *hideExtension(char *filename, char *extension);
char *labyrinthFile();
Grid labyrinthFileRead(char *labyrinth);
void labyrinthPrint(Grid labyrinth);

// Walker
Point *walkPossibilities(Point now, Grid grid, int *pointsCount);
void solutionReverse(Point *points, Point **storage, int vertices);
void solution(Walk *walk, Point **points, int *vertices);
void solutionPrint(Point *points, char **attribute, int count);
void walkGo(Walk *walk, Grid grid, Walk **success);
Walk *walkMake(Point location, Walk *back);
