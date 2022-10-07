// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

int checkSubsetAtGivenIndex(Vector<int>& blocks, int half_value, int cur_index, int target_index, int sum) {
    if (cur_index == blocks.size()) {
        return 0;
    } else if (cur_index == target_index) {
        return checkSubsetAtGivenIndex(blocks, half_value, cur_index + 1, target_index, sum);
    } else {
        int count = 0;
        int sum_with_cur = sum + blocks[cur_index];
        count += checkSubsetAtGivenIndex(blocks, half_value, cur_index + 1, target_index, sum);
        count += checkSubsetAtGivenIndex(blocks, half_value, cur_index + 1, target_index, sum_with_cur);

        if (sum_with_cur <= half_value && sum_with_cur + blocks[target_index] > half_value) {
            count ++;
        }
        return count;
    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    int half_value = 0;
    for (auto val : blocks) {
        half_value += val;
    }
    half_value = half_value / 2;

    Vector<int> result;
    int total_count = 0;
    for (int i = 0; i < blocks.size(); i++) {
        int count = checkSubsetAtGivenIndex(blocks, half_value, 0, i, 0);
        result.add(count);
        total_count += count;
    }

    for (int i = 0; i < result.size(); i++) {
        result[i] = result[i] * 100 / total_count;
    }

    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


