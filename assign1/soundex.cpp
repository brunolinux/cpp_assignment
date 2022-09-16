/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
#include "set.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

string encode(string s) {
    string result = "";
    Set<char> zero{'A', 'E', 'I', 'O', 'U', 'H', 'W', 'Y'};
    Set<char> one{'B', 'F', 'P', 'V'};
    Set<char> two{'C', 'G', 'J', 'K', 'Q', 'S', 'X', 'Z'};
    Set<char> three{'D', 'T'};
    Set<char> four{'L'};
    Set<char> five{'M', 'N'};
    Set<char> six{'R'};

    for (int i = 0; i < s.length(); i++) {
        if (zero.contains(toUpperCase(s[i])))
            result += "0";
        if (one.contains(toUpperCase(s[i])))
            result += "1";
        if (two.contains(toUpperCase(s[i])))
            result += "2";
        if (three.contains(toUpperCase(s[i])))
            result += "3";
        if (four.contains(toUpperCase(s[i])))
            result += "4";
        if (five.contains(toUpperCase(s[i])))
            result += "5";
        if (six.contains(toUpperCase(s[i])))
            result += "6";
    }

    return result;
}

string coalesce(string s) {
    if (s.empty()) {
        return s;
    }

    string result = charToString(s[0]);
    char cur = s[0];
    for (int i = 1; i < s.length(); i++) {
        if (charToInteger(cur) == charToInteger(s[i]))
            continue;

        cur = s[i];
        result += s[i];
    }
    return result;
}

string header(string s, string orgin)
{

  if (!orgin.empty()) {
      //  s[0] = toUpperCase(orgin[0]);
      s.replace(0, 1, charToString(toUpperCase(orgin[0])));
  } else {
      s = "0";
  }

  return s;
}

string remove_zero(string s) {
    string result = charToString(s[0]);
    for (int i = 1; i < s.length(); i++) {
        if (charToInteger(s[i]) == 0)
            continue;
        result += s[i];
    }
    return result;
}

string round(string s) {
    string result = "";
    for (int i = 0; i < 4; i++) {
        if (i >= s.length())
            result += "0";
        else
            result += s[i];
    }
    return result;
}

string soundex(string s) {
    string s_nonletters = removeNonLetters(s);

    return round(remove_zero(header(coalesce(encode(s_nonletters)), s_nonletters)));
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */

    while (true) {
      string name = getLine("Enter a surname (RETURN to quit): ");
      if (name.empty()) {
        cout << "All done!" << endl;
        break;
      }

      string code = soundex(name);

      cout << "Soundex code is " << code << endl;

      Vector<string> matchNames;
      for (const string& name : databaseNames) {
        if (soundex(name) == code)
          matchNames.add(name);
      }
      matchNames.sort();
      cout << "Matches from database: " << matchNames << endl;
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "-xyz";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "xyz");
}

PROVIDED_TEST("encode string") {
    EXPECT_EQUAL(encode("Curie"), "20600");
}

PROVIDED_TEST("coalesce encoder") {
    EXPECT_EQUAL(coalesce("20600"), "2060");
    EXPECT_EQUAL(coalesce("222025"), "2025");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("Test removing empty characters, characters with all spaces") {
    string s = "";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");

    s = "1234";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");

    s = "    ";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}

