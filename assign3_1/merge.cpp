/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;


void checkInputElement(int cur_value, int &max_value)
{
    if (cur_value < max_value) {
        error("queue doesn't obey ascending order");
    } else {
        max_value = cur_value;
    }
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;

    int max_val = std::numeric_limits<int>::min();

    while (!a.isEmpty() && !b.isEmpty()) {
        int v_a = a.peek();
        int v_b = b.peek();
        int cur_input = 0;

        if (v_a <= v_b) {
            result.enqueue(a.dequeue());
            cur_input = v_a;
        } else {
            result.enqueue(b.dequeue());
            cur_input = v_b;
        }

        checkInputElement(cur_input, max_val);
    }

    Queue<int> *q_ptr = !a.isEmpty() ? &a : &b;

    while (!q_ptr->isEmpty()) {
        int v = q_ptr->dequeue();
        result.enqueue(v);
        checkInputElement(v, max_val);
    }

    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    if (all.size() == 0) {
        return result;
    } else if (all.size() == 1) {
        return all[0];
    } else {
        Vector<Queue<int>> left;
        Vector<Queue<int>> right;

        int mid_index = all.size() / 2;
        left = all.subList(0, mid_index);
        right = all.subList(mid_index);
//        int i = 0;
//        for (; i < ; i++) {
//            left.add(all[i]);
//        }
//        for (; i < all.size(); i ++) {
//            right.add(all[i]);
//        }

        Queue<int> left_queue = recMultiMerge(left);
        Queue<int> right_queue = recMultiMerge(right);
        return binaryMerge(left_queue, right_queue);
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

STUDENT_TEST("binaryMerge, example 1, one empty queue") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 2, 3};
    Queue<int> expected = {2, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, {}), a);
    EXPECT_EQUAL(binaryMerge(b, {}), b);
}

STUDENT_TEST("binaryMerge, example 2, one unordered queue") {
    Queue<int> a = {4, 4, 2};
    Queue<int> b = {1, 2, 3};
    Queue<int> expected = {2, 4, 5};
    EXPECT_ERROR(binaryMerge(a, {}));
    EXPECT_ERROR(binaryMerge(a, b));
}
