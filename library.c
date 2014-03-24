#include "library.h"

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

char *labyrinthFile(char *labName) {
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
                labyrinths[counter] = calloc(sizeof(char), strlen(dirStream->d_name));
                strcpy(labyrinths[counter], dirStream->d_name);

                // Print the UI
                char *filename = malloc(sizeof(char) * 0);
                filename = hideExtension(dirStream->d_name, found);
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
    //printf("extension labyrinth: [%s]\n", labyrinths[0]);

    char *pickFilename = hideExtension(labyrinths[pick-1], strstr(labyrinths[pick-1], ".labyrinth"));

    labName = realloc(labName, sizeof(char) * strlen(pickFilename));
    strcpy(labName, pickFilename);

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

Point *walkPossibilities(Point now, Grid grid, int *pointsCount) {
    int     length  = grid.length - 1,
            lines   = grid.lines - 1;
    // Tests
    char    nw = (now.x > 0 && now.y > 0) ? 1 : 0,
            nn = (now.y > 0) ? 1 : 0,
            ne = (now.x < length && now.y > 0) ? 1 : 0,
            ee = (now.x < length) ? 1 : 0,
            se = (now.x < length && now.y < lines) ? 1 : 0,
            ss = (now.y < lines) ? 1 : 0,
            sw = (now.x > 0 && now.y < lines) ? 1 : 0,
            ww = (now.x > 0) ? 1 : 0;

    /*
    printf("[%d][%d][%d]\n", nw, nn, ne);
    printf("[%d][%c][%d]\n", ww, grid.self[now.y][now.x], ee);
    printf("[%d][%d][%d]\n", sw, ss, se);
    */

    // Add points
    Point *points = malloc(sizeof(Point) * 0);
    int i = 0;

    // TODO check if we're not moving back
    if (grid.self[now.y][now.x] != '*') {
        if (0) { // nw
            if (grid.self[now.y-1][now.x-1] != '*') {
                i++;
                points = realloc(points, sizeof(Point) * i);
                points[i-1] = pointMake(now.x-1, now.y-1);
            }
        }
        if (nn) {
            if (grid.self[now.y-1][now.x] != '*') {
                i++;
                points = realloc(points, sizeof(Point) * i);
                points[i-1] = pointMake(now.x, now.y-1);
            }
        }
        if (0) { // ne
            if (grid.self[now.y-1][now.x+1] != '*') {
                i++;
                points = realloc(points, sizeof(Point) * i);
                points[i-1] = pointMake(now.x+1, now.y-1);
            }
        }
        if (ee) {
            if (grid.self[now.y][now.x+1] != '*') {
                i++;
                points = realloc(points, sizeof(Point) * i);
                points[i-1] = pointMake(now.x+1, now.y);
            }
        }
        if (0) { // se
            if (grid.self[now.y+1][now.x+1] != '*') {
                i++;
                points = realloc(points, sizeof(Point) * i);
                points[i-1] = pointMake(now.x+1, now.y+1);
            }
        }
        if (ss) {
            if (grid.self[now.y+1][now.x] != '*') {
                i++;
                points = realloc(points, sizeof(Point) * i);
                points[i-1] = pointMake(now.x, now.y+1);
            }
        }
        if (0) { // sw
            if (grid.self[now.y+1][now.x-1] != '*') {
                i++;
                points = realloc(points, sizeof(Point) * i);
                points[i-1] = pointMake(now.x-1, now.y+1);
            }
        }
        if (ww) {
            if (grid.self[now.y][now.x-1] != '*') {
                i++;
                points = realloc(points, sizeof(Point) * i);
                points[i-1] = pointMake(now.x-1, now.y);
            }
        }
    }

    *pointsCount = i;
    return points;
}

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
        if (i != 0)
            sprintf(temp, " %d,%d", points[i].x, points[i].y*2);
        else
            sprintf(temp, "%d,%d", points[i].x, points[i].y*2);

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
