// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* generate valid moves based on current location
 * invalid move includes:
 *     locations out of border
 *     locations with wall
 *     locations are not neighboors of current locations
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    /* TODO: Fill in the remainder of this function. */
    int cols = maze.numCols();
    int rows = maze.numRows();

    int r = cur.row;
    int c = cur.col;

    if (r > 0 && maze[r-1][c]) {
        neighbors.add({r-1, c});
    }

    if (r < rows-1 && maze[r+1][c]) {
        neighbors.add({r+1, c});
    }

    if (c > 0 && maze[r][c-1]) {
        neighbors.add({r, c-1});
    }

    if (c < cols-1 && maze[r][c+1]) {
        neighbors.add({r, c+1});
    }

    return neighbors;
}

/* check the validation of the whole path
 * valid situation must conforms:
 *     start location must be (0, 0)
 *     end location must be (maze.numRows()-1,  maze.numCols()-1)
 *     no repeated location
 *     every move between two adjacent locations must be valid
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.isEmpty()) {
        error("Path is empty");
    }

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    /* TODO: Fill in the remainder of this function. */

    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
    Set<GridLocation> checked_locations;
    while (!path.isEmpty()) {
        auto loc = path.pop();

        if (checked_locations.contains(loc)) {
            error("Path contains repeated locations");
        }

        if (!path.isEmpty()) {
            auto prev_loc = path.peek();
            Set<GridLocation> moves = generateValidMoves(maze, prev_loc);

            if (!moves.contains(loc)) {
                error("Path contains invalid move");
            }
        } else {
            if (loc != GridLocation(0, 0)) {
                error("Path does not start at maze entry");
            }
        }

        checked_locations.add(loc);
    }
}


/* find the next location base on the top element set of the `path_set` and the top location
 * of the `path`
 * path_set: all possible locations of each step from start location
 * path: current chosen location path
 *
 * return
 *     true: we find a path to the end location of given maze
 *     false: a valid path is not found until current step
 */
bool findNextLocation(Grid<bool>& maze, Stack<Set<GridLocation>> &path_set, Stack<GridLocation> &path)
{
    // std::cout << path.toString() << std::endl;
    MazeGraphics::highlightPath(path, "green", 10);

    // get all possible locations of current step
    Set<GridLocation> cur_locs = path_set.pop();

    // if no locations are available
    if (cur_locs.isEmpty()) {
        // return back to previous step
        path.pop();
        // try to find another path
        if (findNextLocation(maze, path_set, path)) {
            return true;
        }

    // if there exists locations
    } else {
        // for each possible location
        for (GridLocation cur_loc : cur_locs) {
            // find all next available locations for next step
            Set<GridLocation> next_locs = generateValidMoves(maze, cur_loc);

            // remove repeated locations for next step
            {
                Set<GridLocation> past_locs;
                Stack<GridLocation> path_clone = path;
                while (!path_clone.isEmpty()) {
                    past_locs.add(path_clone.pop());
                }

                next_locs.difference(past_locs);
            }

            // remove the chosen location from avilable locations of current step
            // because we are going to test the chose location
            Set<GridLocation> cur_locs_clone = cur_locs;
            cur_locs_clone.remove(cur_loc);
            path_set.push(cur_locs_clone);

            // add the chosen location to path
            path.push(cur_loc);
            // add next available locations for next step
            path_set.push(next_locs);

            // check whether current location is the end
            // if true, then return true
            if (cur_loc == GridLocation{maze.numRows()-1,  maze.numCols()-1}) {
                return true;
            }

            // otherwise try to continue the search
            // if the search return true, mean one path is successed.
            if (findNextLocation(maze, path_set, path)) {
                return true;
            }
        }
    }
    return false;
}

bool findPathByBFS(Grid<bool>& maze, Stack<GridLocation> &target_path)
{
    Queue<Stack<GridLocation>> search_queue;
    Stack<GridLocation> init_path;
    init_path.push(GridLocation{0, 0});
    search_queue.enqueue(init_path);

    while (true) {
        int number = search_queue.size();

        if (number == 0) {
            break;
        }

        for (int i = 0; i < number; i ++) {
            Stack<GridLocation> path = search_queue.dequeue();

            // gui
            MazeGraphics::highlightPath(path, "green", 10);

            auto cur_loc = path.peek();

            if (cur_loc == GridLocation{maze.numRows()-1,  maze.numCols()-1}) {
                target_path = path;
                return true;
            }

            Set<GridLocation> next_locs = generateValidMoves(maze, cur_loc);
            {
                Set<GridLocation> past_locs;
                Stack<GridLocation> path_clone = path;
                while (!path_clone.isEmpty()) {
                    past_locs.add(path_clone.pop());
                }

                next_locs.difference(past_locs);
            }

            for (auto next_loc : next_locs) {
                Stack<GridLocation> path_clone = path;
                path_clone.push(next_loc);
                search_queue.enqueue(path_clone);
            }
        }
    }
    return false;
}

/*
 * solve maze problem by DFS
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {

    // method by DFS
//    MazeGraphics::drawGrid(maze);
//    Stack<GridLocation> path;
//    /* TODO: Fill in the remainder of this function. */
//    GridLocation init_loc(0, 0);
//    Set<GridLocation> init_set{init_loc};

//    Stack<Set<GridLocation>> path_set;
//    path_set.push(init_set);
//    findNextLocation(maze, path_set, path);

//    return path;

    // method by BFS
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    findPathByBFS(maze, path);
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("generateValidMoves on location in the border of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 0};
    Set<GridLocation> expected = {{0, 0}, {2, 0}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves on location in the border of 3x3 grid with a wall on center location") {
    Grid<bool> maze = {{true, true, true},
                       {true, false, true},
                       {true, true, true}};
    GridLocation center = {1, 0};
    Set<GridLocation> expected = {{0, 0}, {2, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves on location in the border of 3x3 grid with all walls") {
    Grid<bool> maze = {{false, true, true},
                       {true, false, true},
                       {false, true, true}};
    GridLocation center = {1, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, true, false},
                       {true, false, false},
                       {true, true, true}};
    Stack<GridLocation> out_of_border = {{0, 0}, {1, 0}, {1, -1}, {2, -1}, {2, 0}, {2, 1}, {2, 2}};
    Stack<GridLocation> inside_wall = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}};

    EXPECT_ERROR(validatePath(maze, out_of_border));
    EXPECT_ERROR(validatePath(maze, inside_wall));
}
