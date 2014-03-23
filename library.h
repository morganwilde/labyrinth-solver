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

Point pointMake(int x, int y);
char gridCell(Grid grid, int x, int y);
char gridCellPoint(Grid grid, Point p);
char *hideExtension(char *filename, char *extension);
char *labyrinthFile();
Grid labyrinthFileRead(char *labyrinth);
void labyrinthPrint(Grid labyrinth);
