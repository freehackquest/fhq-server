#include <unit_test_merge_text.h>
#include <utils/utils_merge_text.h>
#include <vector>
#include <iostream>

REGISTRY_UNIT_TEST(UnitTestMergeText)

UnitTestMergeText::UnitTestMergeText()
    : WSJCppUnitTestBase("UnitTestMergeText") {
    // 
}

// ---------------------------------------------------------------------

void UnitTestMergeText::init() {
}

// ---------------------------------------------------------------------

bool UnitTestMergeText::run() {

    struct LTest {
        LTest(
            std::string txt1,
            std::string txt2,
            std::string txt3,
            std::vector<UtilsMergeTextRow *> arr1,
            std::vector<UtilsMergeTextRow *> arr2,
            unsigned int n
        ) : txt1(txt1), txt2(txt2), txt3(txt3), arr1(arr1), arr2(arr2), n(n) {
            //
        }
        std::string txt1;
        std::string txt2;
        std::string txt3;
        std::vector<UtilsMergeTextRow *> arr1;
        std::vector<UtilsMergeTextRow *> arr2;
        unsigned int n;
    };

    std::vector<LTest *> tests1;
    std::vector<UtilsMergeTextRow *> arr1, arr2;

    std::string text1 = text001();
    std::string text2 = text002();

    // std::cout << text1.toStdString() << "\n";

    //empty (Test №1)
    tests1.push_back(new LTest("", "", "", arr1, arr2, 0));
    //without any difference (Test №2)
    tests1.push_back(new LTest("I\ngot\nMike", "I\ngot\nMike", "I\ngot\nMike", arr1, arr2, 0));
    //with a difference in the ending (Tests №3-6)
    tests1.push_back(new LTest("I\ngot\nMike", "I\ngot\nMike", "I\ngot\nNike", arr1, arr2, 1));
    tests1.push_back(new LTest("I\ngot\nNike", "I\ngot\nMike", "I\ngot\nMike", arr1, arr2, 1));
    tests1.push_back(new LTest("I\ngot\nNike", "I\ngot\nMike", "I\ngot\nNike", arr1, arr2, 1));
    tests1.push_back(new LTest("I\ngot\nBike", "I\ngot\nMike", "I\ngot\nNike", arr1, arr2, 1));
    //with a difference in the beginning (Tests №7-10)
    tests1.push_back(new LTest("I\ngot\nMike", "I\ngot\nMike", "You\ngot\nMike", arr1, arr2, 1));
    tests1.push_back(new LTest("You\ngot\nMike", "I\ngot\nMike", "I\ngot\nMike", arr1, arr2, 1));
    tests1.push_back(new LTest("You\ngot\nMike", "I\ngot\nMike", "You\ngot\nMike", arr1, arr2, 1));
    tests1.push_back(new LTest("We\ngot\nMike", "I\ngot\nMike", "You\ngot\nMike", arr1, arr2, 1));
    //with a difference in the middle of the row (Tests №11-15)
    tests1.push_back(new LTest("I\nhave\ncute\ncats", "I\nhave\ncute\ncats", "I\nhave\nhuge\ncats", arr1, arr2, 1));
    tests1.push_back(new LTest("I\nhave\nhuge\ncats", "I\nhave\ncute\ncats", "I\nhave\ncute\ncats", arr1, arr2, 1));
    tests1.push_back(new LTest("I\nhave\nhuge\ncats", "I\nhave\ncute\ncats", "I\nhave\nhuge\ncats", arr1, arr2, 1));
    tests1.push_back(new LTest("I\nhave\ndifference\ncats", "I\nhave\ncute\ncats", "I\nhave\nhuge\ncats", arr1, arr2, 1));
    tests1.push_back(new LTest("I\nbetray\ncats", "I\nhave\ncats", "I\nlove\ncats", arr1, arr2, 1));
    //delete lines from the end of text (Tests №16-19)
    tests1.push_back(new LTest("Go\nto\nthe\nschool", "Go\nto\nthe\nschool", "Go\nto", arr1, arr2, 2));
    tests1.push_back(new LTest("Go\nto", "Go\nto\nthe\nschool", "Go\nto\nthe\nschool", arr1, arr2, 2));
    tests1.push_back(new LTest("Go\nto\nthe", "Go\nto\nthe\nschool", "Go\nto", arr1, arr2, 2));
    tests1.push_back(new LTest("Go\nto", "Go\nto\nthe\nschool", "Go\nto\nthe", arr1, arr2, 2));
    //add lines in the end of text (Tests №20-23)
    tests1.push_back(new LTest("Go\nto", "Go\nto", "Go\nto\nthe\nschool", arr1, arr2, 2));
    tests1.push_back(new LTest("Go\nto\nthe\nschool", "Go\nto", "Go\nto", arr1, arr2, 2));
    tests1.push_back(new LTest("Go\nto\nthe", "Go\nto", "Go\nto\nthe\nschool", arr1, arr2, 2));
    tests1.push_back(new LTest("Go\nto\nthe\nschool", "Go\nto", "Go\nto\nthe", arr1, arr2, 2));
    //delete rows from the beginning and the middle of text (Tests №24-27)
    tests1.push_back(new LTest("Go\nto\nthe\nschool", "Go\nto\nthe\nschool", "the\nschool", arr1, arr2, 2));
    tests1.push_back(new LTest("the\nschool", "Go\nto\nthe\nschool", "Go\nto\nthe\nschool", arr1, arr2, 2));
    tests1.push_back(new LTest("Go\nto\nthe\nschool", "Go\nto\nthe\nschool", "Go\nto\nschool", arr1, arr2, 1));
    tests1.push_back(new LTest("Go\nto\nschool", "Go\nto\nthe\nschool", "Go\nto\nthe\nschool", arr1, arr2, 1));
    //add rows to the beginning and the middle of text (Tests №28-31)
    tests1.push_back(new LTest("the\nschool", "the\nschool", "Go\nto\nthe\nschool", arr1, arr2, 2));
    tests1.push_back(new LTest("Go\nto\nthe\nschool", "the\nschool", "the\nschool", arr1, arr2, 2));
    tests1.push_back(new LTest("Go\nthe\nschool", "the\nschool", "the\nschool", arr1, arr2, 1));
    tests1.push_back(new LTest("the\nschool", "the\nschool", "Go\nthe\nschool", arr1, arr2, 1));
    //complex (Tests №32-33)
    tests1.push_back(new LTest("You\nare\ngot\nand\ngood", "I\ngot\nMike\nand\nthis is\ngood\nboy", "I\ncure\ndamn\nill\ncancer", arr1, arr2, 8));
    tests1.push_back(new LTest("We\nspent\nthe rest\nof our life\non\nthis\ncourse\nwork", "We\nspent\na lot of\ntime\non\nthis\ncourse\nwork", "We\ngonna\nspend\na lot of\nfunky\ntime\non\ncourse\nwork", arr1, arr2, 6));

    unsigned int nSuccess = 0;
    for (unsigned int i = 0; i < tests1.size(); i++) {
        std::string txt1 = tests1[i]->txt1;
        std::string txt2 = tests1[i]->txt2;
        std::string txt3 = tests1[i]->txt3;
        UtilsMergeText::merge(txt1, txt2, txt3, arr1, arr2);
        unsigned int n = tests1[i]->n;

        if (arr1.size()==n) {
            nSuccess++;
        } else {
            WSJCppLog::err(TAG, "In the test №" + std::to_string(i + 1) + " the length of the vector is expected: "
                 + std::to_string(n) + ", but obtained: " + std::to_string(arr1.size()));
        }
        arr1.clear(), arr2.clear();
    }

    //sort module test
    struct LTest2 {
        LTest2(
            std::string txt1,
            std::string txt2,
            std::string txt3,
            std::vector<UtilsMergeTextRow *> arr1,
            std::vector<UtilsMergeTextRow *> arr2,
            std::vector<UtilsMergeTextRow *> arr3
        ) : txt1(txt1), txt2(txt2), txt3(txt3), arr1(arr1), arr2(arr2), arr3(arr3) {
            // 
        }

        std::string txt1;
        std::string txt2;
        std::string txt3;
        std::vector<UtilsMergeTextRow *> arr1;
        std::vector<UtilsMergeTextRow *> arr2;
        std::vector<UtilsMergeTextRow *> arr3;
    };

    std::vector<LTest2 *> tests2;
    std::vector<UtilsMergeTextRow *> arr3;
    int id[8] = {0,1,1,2,3,4,5,6};
    std::vector<std::string> skey;
    skey.push_back("I");
    skey.push_back("got");
    skey.push_back("!add");
    skey.push_back("Mike");
    skey.push_back("and");
    skey.push_back("this is");
    skey.push_back("!del");
    skey.push_back("!del");

    std::vector<std::string> sline;
    sline.push_back("You");
    sline.push_back("cure");
    sline.push_back("are");
    sline.push_back("damn");
    sline.push_back("ill");
    sline.push_back("cancer");
    sline.push_back("good");
    sline.push_back("boy");

    for (int i=0;i<skey.size();++i) arr3.push_back(new UtilsMergeTextRow(id[i], skey.at(i), sline.at(i)));

    tests2.push_back(new LTest2(
        "You\nare\ngot\nand\ngood",
        "I\ngot\nMike\nand\nthis is\ngood\nboy",
        "I\ncure\ndamn\nill\ncancer",
        arr1, arr2, arr3));

    std::string txt1 = tests2[0]->txt1;
    std::string txt2 = tests2[0]->txt2;
    std::string txt3 = tests2[0]->txt3;
    UtilsMergeText::merge(txt1, txt2, txt3, arr1, arr2);

    unsigned int Success = 0;
    for (int i = 0; i < 8; ++i) {
        int id1 = arr1.at(i)->id;
        int id2 = arr3.at(i)->id;
        std::string key1 = arr1.at(i)->key;
        std::string key2 = arr3.at(i)->key;
        std::string line1 = arr1.at(i)->line;
        std::string line2 = arr3.at(i)->line;
        if (id1==id2 && key1==key2 && line1==line2) {
            Success+=1;
        } else {
            WSJCppLog::info(TAG, "In the sort test in the element №" + std::to_string(i+1) + ":");

            if (id1!=id2) {
                WSJCppLog::err(TAG, "expected id: '" + std::to_string(id2) + "', but obtained: '" + std::to_string(id1) + "'");
            }

            if (key1!=key2) {
                WSJCppLog::err(TAG, "expected key: '" + key2 + "', but obtained: '" + key1 + "'");
            }

            if (line1!=line2) {
                WSJCppLog::err(TAG, "expected line: '" + line2 + "', but obtained: '" + line1 + "'");
            }
        }
    }

    if (Success == 8) {
        nSuccess++;
    }

    return nSuccess == tests1.size() + 1;
}

// ---------------------------------------------------------------------

std::string UnitTestMergeText::text001() {
    return "Str1\n"
        "Str\n"
        "Str\n"
        "Str\n"
        "Str2\n"
        "Str3\n"
        "Str\n"
        "Str\n"
        "Str\n";
}

// ---------------------------------------------------------------------

std::string UnitTestMergeText::text002() {
    return "Str1\n"
        "Str \n"
        "Str\n"
        "Str\n"
        "Str2\n"
        "Str\n"
        "Str3\n"
        "Str\n"
        "Str\n"
        "Str\n";
}

// ---------------------------------------------------------------------
