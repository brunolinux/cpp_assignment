// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* remove head and tail punctuation characters
 */
string removeHeaderAndTail(const string& s)
{
    int ind_s = 0;
    int ind_e = s.length() - 1;
    for (; ind_s < s.length(); ind_s ++) {
        if (ispunct(s[ind_s]) == 0) {
            break;
        }
    }

    for (; ind_e >= 0; ind_e --) {
        if (ispunct(s[ind_e]) == 0) {
            break;
        }
    }

    return string(s, ind_s, ind_e - ind_s + 1);
}

/* check whether contain alphabetic characters
 * true: exist at least one alphabetic character
 */
bool checkAlpha(const string& s)
{
    bool no_alpha = true;
    for (auto c : s) {
        if (isalpha(c)) {
            no_alpha = false;
            break;
        }
    }

    return !no_alpha;
}

/*
 */
string cleanToken(string s)
{
    string handled_s = removeHeaderAndTail(s);
    if (!checkAlpha(s)) {
        return string();
    } else {
        string result;
        for (auto c : handled_s) {
            if (isalpha(c)) {
                result += toLowerCase(c);
            } else {
                result += c;
            }
        }
        return result;
    }
}

// get all cleaned tokens from given text (splited by space e.g., ' ')
// note: using set to ensure no repeatation
Set<string> gatherTokens(string text)
{
    Vector<std::string> s_list = stringSplit(text, ' ');
    Set<string> tokens;
    for (const auto & s : s_list) {
        auto token = cleanToken(s);
        if (token.length() > 0) {
            tokens.add(std::move(token));
        }
    }

    return tokens;
}

//  build index line by line
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream ifs;
    int line_num = 0;
    if(openFile(ifs, dbfile)) {
        string url;
        for (std::string line; std::getline(ifs, line); ) {
            if (line_num % 2 != 0) {
                Set<string> tokens = gatherTokens(line);

                for (const auto & token : tokens) {
                    if (index.containsKey(token)) {
                       index[token].add(url);
                    } else {
                       index[token] = Set<string>{url};
                    }
                }
            } else {
                url = line;
            }

            line_num ++;
        }
    }
    return (line_num / 2);
}

// find query's corresponding url page
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;

    auto search_list = stringSplit(query, ' ');

    for (const auto & s : search_list) {
        auto token = cleanToken(s);

        // check token is not empty
        if (token.length() == 0) {
            continue;
        }
        // check token exist in the database
        if (index.containsKey(token)) {

            Set<string> cur_ret = index[token];
            // check first character
            if (s[0] == '+') {
                result = result.intersect(cur_ret);
            } else if (s[0] == '-') {
                result = result.difference(cur_ret);
            } else {
                result = result.unionWith(cur_ret);
            }
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    Map<string, Set<string>> index;
    buildIndex(dbfile, index);

}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("remove head and tail punctuation characters") {
    EXPECT_EQUAL(removeHeaderAndTail("section!"), "section");
    EXPECT_EQUAL(removeHeaderAndTail("<<section>>"), "section");
    EXPECT_EQUAL(removeHeaderAndTail("section"), "section");
    EXPECT_EQUAL(removeHeaderAndTail("section's"), "section's");
    EXPECT_EQUAL(removeHeaderAndTail("section-10"), "section-10");
    EXPECT_EQUAL(removeHeaderAndTail("/section-10/"), "section-10");
}

STUDENT_TEST("check alpha") {
    EXPECT_EQUAL(checkAlpha("section"), true);
    EXPECT_EQUAL(checkAlpha("123"), false);
    EXPECT_EQUAL(checkAlpha("12section"), true);
    EXPECT_EQUAL(checkAlpha("---"), false);
    EXPECT_EQUAL(checkAlpha(""), false);
}
