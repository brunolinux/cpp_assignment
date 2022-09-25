/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include "testing/SimpleTest.h"
#include <iostream>
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0)
            cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}


/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long smarterSum(long n) {
    if (n <= 1) {
        return 0;
    } else if (n == 2 || n == 3) {
        return 1;
    }

    long total = 0;

    total += 1;

    long sqrt_n = static_cast<long>(sqrt(n));

    for (long divisor = 2; divisor < sqrt_n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
            total += (n / divisor);
        }
    }

    if (sqrt_n * sqrt_n == n) {
        total += sqrt_n;
    } else {
        if (n % sqrt_n == 0) {
            total += sqrt_n;
            total += (n / sqrt_n);
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfectSmarter(long n) {
    /* TODO: Fill in this function. */
    if (n <= 1) {
        return false;
    } else {
        return (smarterSum(n) == n);
    }
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void findPerfectsSmarter(long stop) {
     /* TODO: Fill in this function. */
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0)
            cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

bool isPrime(long n) {
    return (n != 1) && (smarterSum(n) == 1);
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long findNthPerfectEuclid(long n) {
    /* TODO: Fill in this function. */
    int k = 1;
    long mersenne;
    for (long i = 0; i < n; i ++) {
        do {
            mersenne = pow(2, k) - 1;
            k ++;
        } while (!isPrime(mersenne));
    }
    long target = pow(2, k-2) * mersenne;
    return target;
}

/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
    EXPECT_EQUAL(divisorSum(4), 3);
}

PROVIDED_TEST("Confirm smarterSum of small inputs") {
    EXPECT_EQUAL(smarterSum(1), 0);
    EXPECT_EQUAL(smarterSum(6), 6);
    EXPECT_EQUAL(smarterSum(12), 16);
    EXPECT_EQUAL(smarterSum(4), 3);
}

PROVIDED_TEST("divisorSum vs smarterSum") {
    for (long i = 0; i < 1000; i++) {
        EXPECT_EQUAL(divisorSum(i), smarterSum(i));
    }
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfectSmarter(6));
    EXPECT(isPerfectSmarter(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfectSmarter(12));
    EXPECT(!isPerfectSmarter(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfectSmarter(0));
    EXPECT(!isPerfectSmarter(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfectSmarter(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfectsSmarter(10000));
    TIME_OPERATION(20000, findPerfectsSmarter(20000));
    TIME_OPERATION(40000, findPerfectsSmarter(40000));
}

// TODO: add your student test cases here

PROVIDED_TEST("Test oddballs: 4 and 12 are not prime") {
    EXPECT(!isPrime(4));
    EXPECT(!isPrime(12));
}

PROVIDED_TEST("Test oddballs: 1 and 7 are prime") {
    EXPECT(!isPrime(1));
    EXPECT(isPrime(7));
}

PROVIDED_TEST("Test findNthPerfectEuclid") {
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}

PROVIDED_TEST("Test findNthPerfectEuclid") {
    for (long i = 1; i < 6; i++)
        EXPECT(isPerfectSmarter(findNthPerfectEuclid(i)));
}

//STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
//    TIME_OPERATION(15000, findPerfects(15000));
//    TIME_OPERATION(30000, findPerfects(30000));
//    TIME_OPERATION(60000, findPerfects(60000));
//    TIME_OPERATION(120000, findPerfects(120000));
//}

STUDENT_TEST("Test negtive values: -1 and -10 are not perfect") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-10));
}

STUDENT_TEST("Confirm 0, 1, 10, 25 and 64 are not perfect") {
    EXPECT(!isPerfectSmarter(0));
    EXPECT(!isPerfectSmarter(1));
    EXPECT(!isPerfectSmarter(10));
    EXPECT(!isPerfectSmarter(25));
    EXPECT(!isPerfectSmarter(64));
}

//STUDENT_TEST("Time trials of findPerfectsSmarter on doubling input sizes") {
//    TIME_OPERATION(600000, findPerfectsSmarter(600000));
//    TIME_OPERATION(1200000, findPerfectsSmarter(1200000));
//    TIME_OPERATION(2400000, findPerfectsSmarter(2400000));
//}

STUDENT_TEST("Test findNthPerfectEuclid") {
    EXPECT(isPerfect(findNthPerfectEuclid(1)));
    EXPECT(isPerfect(findNthPerfectEuclid(2)));
    EXPECT(isPerfect(findNthPerfectEuclid(3)));
    EXPECT(isPerfect(findNthPerfectEuclid(4)));
}

