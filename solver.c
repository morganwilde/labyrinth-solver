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

Point pointMake(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;

    return p;
}

char gridCell(Grid grid, int x, int y) {
    return grid.self[y][x];
}

char gridCellPoint(Grid grid, Point p) {
    return gridCell(grid, p.x, p.y);
}

char *hideExtension(char *filename, char *extension) {
    char *temp = malloc(sizeof(char) * 1);
    int i = 0;
    while (filename[i] != '\0') {
        if (filename[i] != extension[0]) {
            temp[i] = filename[i];
            temp = realloc(temp, sizeof(char) * (i+1));
            i++;
        } else {
            break;
        }
    }
    temp[i] = '\0';

    return temp;
}

char *labyrinthFile() {
    // Look for *.labyrinth files in pwd
    DIR *directory = opendir(".");
    struct dirent *dirStream = readdir(directory);;
    char **labyrinths = malloc(sizeof(char *) * 1);;

    int counter = 0;
    while (dirStream) {
        if ((dirStream = readdir(directory)) != NULL) {
            char *found = strstr(dirStream->d_name, ".labyrinth");
            if (found != NULL) {
                // Save as an option
                labyrinths = realloc(labyrinths, sizeof(char *) * (counter+1));
                labyrinths[counter] = malloc(sizeof(char) * strlen(dirStream->d_name));
                strcpy(labyrinths[counter], dirStream->d_name);

                // Print the UI
                char *filename = hideExtension(dirStream->d_name, found);
                printf("%d. " FONT_U "%s" FONT_N "\n", (counter+1), filename);
                counter++;
            }
        }
    }
    closedir(directory);

    int pick;
    printf("Pick your labyrinth: ");
    // TODO replace this with AVIR
    scanf("%d", &pick);
    char *pickFilename = hideExtension(labyrinths[pick-1], strstr(labyrinths[pick-1], ".labyrinth"));
    printf("Walking the '%s' labyrinth...\n", pickFilename);

    return labyrinths[pick-1];
}

Grid labyrinthFileRead(char *labyrinth) {
    // Open stream
    FILE    *file = fopen(labyrinth, "r");
    char    *line = malloc(sizeof(char) * 1),
            c = 0;

    // Init the grid
    Grid    grid;
    grid.self   = malloc(sizeof(char *) * 1);
    grid.lines  = 0;
    grid.length = 0;

    if (file == NULL) {
        grid.lines = -1;
        grid.length = -1;
        return grid;
    } else {
        int     scanned,
                testDash,
                testPlus,
                testStar,
                testSpace,
                testNewline,
                tests = 1;
        while (tests && scanned != -1) {
            grid.self = realloc(grid.self, sizeof(char *) * (grid.lines+1));
            grid.self[grid.lines] = malloc(sizeof(char) * 1);

            scanned = fscanf(file, "%c", &c);
            
            // Tests
            testDash    = (c == '-');
            testPlus    = (c == '+');
            testStar    = (c == '*');
            testSpace   = (c == ' ');
            testNewline = (c == '\n');
            tests = testDash || testPlus || testStar || testSpace || testNewline;
            
            grid.length = 0;
            while (!testNewline && scanned != -1) {
                // Read chars till new line
                grid.self[grid.lines] = realloc(grid.self[grid.lines], sizeof(char) * (grid.length+1));
                grid.self[grid.lines][grid.length] = c;
                grid.length++;

                // Check for end points
                if (c == '+') {
                    grid.start = pointMake(grid.length-1, grid.lines);
                } else if (c == '-') {
                    grid.end = pointMake(grid.length-1, grid.lines);
                }
                
                //printf("%c", c);
                scanned = fscanf(file, "%c", &c);
                testNewline = (c == '\n');
            }
            grid.self[grid.lines][grid.length] = '\0';
            /*
            if (scanned != -1)
                printf("%c", c);
            else
                printf("\n");
            */

            grid.lines++;
        }
    }

    return grid;
}

void labyrinthPrint(Grid labyrinth) {
    int i;
    for (i = 0; i < labyrinth.lines; i++) {
        printf("%s\n", labyrinth.self[i]);
    }
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

    // TODO create a char array with the data
    // TODO locate and save entry/exit coordinates
    // TODO starting with the entry point create a tree of walks
    // TODO save pointer to exit node
    // TODO walk backwards from exit node to solution
    // TODO save array of vertices of the path back
    // TODO printf the path in reverse order
    // TODO create polyline object in SVG lingo

    return 0;
}
