/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
string operatorsFrom(string str) {
    if (str.empty()) {
        return "";
    } else {
        string target = "[]{}()";
        string check_char = "";
        if (target.find(str[0]) != string::npos) {
            check_char = str[0];
        }
        return check_char + operatorsFrom(str.substr(1));
    }
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool operatorsAreMatched(string ops) {
    if (ops.empty()) {
        return true;
    } else {
        vector<string> search_op{"[]", "()", "{}"};
        for (const auto & op : search_op) {
            size_t pos = ops.find(op);
            if (pos != string::npos) {
                string ret_ops = ops.erase(pos, 2);
                return operatorsAreMatched(ret_ops);
            }
        }
        return false;
    }
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}


STUDENT_TEST("operatorsFrom extended example 1") {
    EXPECT_EQUAL(operatorsFrom("[]{}()"), "[]{}()");
    EXPECT_EQUAL(operatorsFrom("["), "[");
}

STUDENT_TEST("operatorsFrom extended example 2") {
    EXPECT_EQUAL(operatorsFrom(""), "");
    EXPECT_EQUAL(operatorsFrom("123"), "");
}

STUDENT_TEST("operatorsFrom extended example 3") {
    EXPECT_EQUAL(operatorsFrom("123["), "[");
    EXPECT_EQUAL(operatorsFrom("[123"), "[");
    EXPECT_EQUAL(operatorsFrom("[123{"), "[{");
}

STUDENT_TEST("operatorsAreMatched extended example 1") {
    EXPECT(operatorsAreMatched("{}"));
    EXPECT(!operatorsAreMatched("{]"));
    EXPECT(!operatorsAreMatched("}{"));
}

STUDENT_TEST("operatorsAreMatched extended example 2") {
    EXPECT(operatorsAreMatched("{{}}"));
    EXPECT(operatorsAreMatched("{[]}"));
    EXPECT(!operatorsAreMatched("{[}]"));
}

STUDENT_TEST("operatorsAreMatched extended example 3") {
    EXPECT(operatorsAreMatched("{{()}}"));
    EXPECT(operatorsAreMatched("{[]()}"));
    EXPECT(!operatorsAreMatched("{}[])("));
    EXPECT(!operatorsAreMatched("{}[](()}"));
}
