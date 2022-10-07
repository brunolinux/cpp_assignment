/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * get corresponding points
 */
int points(string str) {
    if (str.size() < 4)
        return 0;
    else
        return str.size() - 3;
}


std::vector<GridLocation> getAllNeighbors(GridLocation center, const Grid<bool>& occupy) {
    int c_x = center.col;
    int c_y = center.row;

    GridLocationRange range(c_y - 1, c_x - 1, c_y + 1, c_x + 1);

    std::vector<GridLocation> result;
    for (const auto & loc : range) {
        if (occupy.inBounds(loc) && loc != center && !occupy.get(loc)) {
            result.push_back(loc);
        }
    }
    return result;
}


void collectWordSetFromGivenIndex(const Grid<char>& board, const Lexicon& lex,
                                  Grid<bool>& occupy, Set<string> &word_set,
                                  GridLocation index, string word) {
    word += board.get(index);

    if (!lex.containsPrefix(word)) {
        return;
    }
    occupy.set(index, true);

    std::vector<GridLocation> next_locs = getAllNeighbors(index, occupy);
    for (const auto & loc : next_locs) {
        collectWordSetFromGivenIndex(board, lex, occupy, word_set, loc, word);
    }
    if (lex.contains(word)) {
        word_set.add(word);
    }

    occupy.set(index, false);
    return;
}

/*
 *
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {

    Grid<bool> occupy(board.numRows(), board.numCols(), false);
    Set<string> word_set;

    GridLocationRange range(0, 0, board.numRows()-1, board.numCols()-1);
    string word("");
    for (const auto & loc : range) {
        collectWordSetFromGivenIndex(board, lex, occupy, word_set, loc, word);
    }

    int count = 0;
    for (const auto & word : word_set) {
        count += points(word);
    }

    return count;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}


STUDENT_TEST("Test getAllNeighbors with non-occupied board") {
    Grid<bool> occupy(5, 5, false);

    EXPECT_EQUAL(getAllNeighbors({0, 0}, occupy).size(), 3);
    EXPECT_EQUAL(getAllNeighbors({1, 0}, occupy).size(), 5);
    EXPECT_EQUAL(getAllNeighbors({1, 1}, occupy).size(), 8);
}

STUDENT_TEST("Test getAllNeighbors with occupied board") {
    Grid<bool> occupy = {{false, true, false, false, false},
                         {true, true, false, false, false},
                         {false, false, false, false, false},
                         {false, false, false, false, false},
                         {false, false, false, false, false}};

    EXPECT_EQUAL(getAllNeighbors({0, 0}, occupy).size(), 0);
    EXPECT_EQUAL(getAllNeighbors({1, 0}, occupy).size(), 3);
    EXPECT_EQUAL(getAllNeighbors({1, 1}, occupy).size(), 6);
}
