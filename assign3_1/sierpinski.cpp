/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* TODO: Implement this function. */
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    } else {
        GPoint p1 = GPoint(one.x + two.x, one.y + two.y) * 0.5;
        GPoint p2 = GPoint(one.x + three.x, one.y + three.y) * 0.5;
        GPoint p3 = GPoint(two.x + three.x, two.y + three.y) * 0.5;
        int count1 = drawSierpinskiTriangle(window, one, p1, p2, order - 1);
        int count2 = drawSierpinskiTriangle(window, two, p1, p3, order - 1);
        int count3 = drawSierpinskiTriangle(window, three, p2, p3, order - 1);
        return count1 + count2 + count3;
    }
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

