#include <unit_test_merge_text.h>
#include <utils_merge_text.h>
#include <vector>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <iostream>

QString UnitTestMergeText::name(){
    return "UnitTestMergeText";
}

void UnitTestMergeText::init(){
}

bool UnitTestMergeText::run(){

    struct LTest {
        LTest(QString txt1, QString txt2, QString txt3, std::vector<row *> arr1, std::vector<row *> arr2, unsigned int n) :
            txt1(txt1), txt2(txt2), txt3(txt3), arr1(arr1), arr2(arr2), n(n) {}
        QString txt1;
        QString txt2;
        QString txt3;
        std::vector<row *> arr1;
        std::vector<row *> arr2;
        unsigned int n;
    };

    std::vector<LTest *> tests1;
    std::vector<row *> arr1, arr2;

    // Load texts from resources
    QString text1 = UnitTestMergeText::loadFileFromRes(":/res/unit_test_merge/text001.txt");
    QString text2 = UnitTestMergeText::loadFileFromRes(":/res/unit_test_merge/text002.txt");

    // std::cout << text1.toStdString() << "\n";

    //empty (Test №1)
    tests1.push_back(new LTest("", "", "", arr1, arr2, 0));
    //without any difference (Test №2)
    tests1.push_back(new LTest("I/got/Mike", "I/got/Mike", "I/got/Mike", arr1, arr2, 0));
    //with a difference in the ending (Tests №3-6)
    tests1.push_back(new LTest("I/got/Mike", "I/got/Mike", "I/got/Nike", arr1, arr2, 1));
    tests1.push_back(new LTest("I/got/Nike", "I/got/Mike", "I/got/Mike", arr1, arr2, 1));
    tests1.push_back(new LTest("I/got/Nike", "I/got/Mike", "I/got/Nike", arr1, arr2, 1));
    tests1.push_back(new LTest("I/got/Bike", "I/got/Mike", "I/got/Nike", arr1, arr2, 1));
    //with a difference in the beginning (Tests №7-10)
    tests1.push_back(new LTest("I/got/Mike", "I/got/Mike", "You/got/Mike", arr1, arr2, 1));
    tests1.push_back(new LTest("You/got/Mike", "I/got/Mike", "I/got/Mike", arr1, arr2, 1));
    tests1.push_back(new LTest("You/got/Mike", "I/got/Mike", "You/got/Mike", arr1, arr2, 1));
    tests1.push_back(new LTest("We/got/Mike", "I/got/Mike", "You/got/Mike", arr1, arr2, 1));
    //with a difference in the middle of the row (Tests №11-15)
    tests1.push_back(new LTest("I/have/cute/cats", "I/have/cute/cats", "I/have/huge/cats", arr1, arr2, 1));
    tests1.push_back(new LTest("I/have/huge/cats", "I/have/cute/cats", "I/have/cute/cats", arr1, arr2, 1));
    tests1.push_back(new LTest("I/have/huge/cats", "I/have/cute/cats", "I/have/huge/cats", arr1, arr2, 1));
    tests1.push_back(new LTest("I/have/difference/cats", "I/have/cute/cats", "I/have/huge/cats", arr1, arr2, 1));
    tests1.push_back(new LTest("I/betray/cats", "I/have/cats", "I/love/cats", arr1, arr2, 1));
    //delete lines from the end of text (Tests №16-19)
    tests1.push_back(new LTest("Go/to/the/school", "Go/to/the/school", "Go/to", arr1, arr2, 2));
    tests1.push_back(new LTest("Go/to", "Go/to/the/school", "Go/to/the/school", arr1, arr2, 2));
    tests1.push_back(new LTest("Go/to/the", "Go/to/the/school", "Go/to", arr1, arr2, 2));
    tests1.push_back(new LTest("Go/to", "Go/to/the/school", "Go/to/the", arr1, arr2, 2));
    //add lines in the end of text (Tests №20-23)
    tests1.push_back(new LTest("Go/to", "Go/to", "Go/to/the/school", arr1, arr2, 2));
    tests1.push_back(new LTest("Go/to/the/school", "Go/to", "Go/to", arr1, arr2, 2));
    tests1.push_back(new LTest("Go/to/the", "Go/to", "Go/to/the/school", arr1, arr2, 2));
    tests1.push_back(new LTest("Go/to/the/school", "Go/to", "Go/to/the", arr1, arr2, 2));
    //delete rows from the beginning and the middle of text (Tests №24-27)
    tests1.push_back(new LTest("Go/to/the/school", "Go/to/the/school", "the/school", arr1, arr2, 2));
    tests1.push_back(new LTest("the/school", "Go/to/the/school", "Go/to/the/school", arr1, arr2, 2));
    tests1.push_back(new LTest("Go/to/the/school", "Go/to/the/school", "Go/to/school", arr1, arr2, 1));
    tests1.push_back(new LTest("Go/to/school", "Go/to/the/school", "Go/to/the/school", arr1, arr2, 1));
    //add rows to the beginning and the middle of text (Tests №28-31)
    tests1.push_back(new LTest("the/school", "the/school", "Go/to/the/school", arr1, arr2, 2));
    tests1.push_back(new LTest("Go/to/the/school", "the/school", "the/school", arr1, arr2, 2));
    tests1.push_back(new LTest("Go/the/school", "the/school", "the/school", arr1, arr2, 1));
    tests1.push_back(new LTest("the/school", "the/school", "Go/the/school", arr1, arr2, 1));
    //complex (Tests №32-33)
    tests1.push_back(new LTest("You/are/got/and/good", "I/got/Mike/and/this is/good/boy", "I/cure/damn/ill/cancer", arr1, arr2, 8));
    tests1.push_back(new LTest("We/spent/the rest/of our life/on/this/course/work", "We/spent/a lot of/time/on/this/course/work", "We/gonna/spend/a lot of/funky/time/on/course/work", arr1, arr2, 6));

    unsigned int nSuccess = 0;
    for(unsigned int i = 0; i < tests1.size(); i++){
        QString txt1 = tests1[i]->txt1;
        QString txt2 = tests1[i]->txt2;
        QString txt3 = tests1[i]->txt3;
        UtilsMergeText::merge(txt1, txt2, txt3, arr1, arr2);
        unsigned int n = tests1[i]->n;

        if(arr1.size()==n) nSuccess++;
        else std::cout << "In the test №" << i+1
                       << " the length of the vector is expected: " << n
                       << ", but obtained: " << arr1.size() <<".\n";

        arr1.clear(), arr2.clear();
    }

    //sort module test
    struct LTest2 {
        LTest2(QString txt1, QString txt2, QString txt3, std::vector<row *> arr1, std::vector<row *> arr2, std::vector<row *> arr3) :
            txt1(txt1), txt2(txt2), txt3(txt3), arr1(arr1), arr2(arr2), arr3(arr3) {}
        QString txt1;
        QString txt2;
        QString txt3;
        std::vector<row *> arr1;
        std::vector<row *> arr2;
        std::vector<row *> arr3;
    };

    std::vector<LTest2 *> tests2;
    std::vector<row *> arr3;
    int id[8] = {0,1,1,2,3,4,5,6};
    QStringList skey = (QStringList() << "I" << "got" << "!add" << "Mike" << "and" << "this is" << "!del" << "!del");
    QStringList sline = (QStringList() << "You" << "cure" << "are" << "damn" << "ill" << "cancer" << "good" << "boy");
    for(int i=0;i<skey.size();++i) arr3.push_back(new row(id[i], skey.at(i), sline.at(i)));

    tests2.push_back(new LTest2("You/are/got/and/good", "I/got/Mike/and/this is/good/boy", "I/cure/damn/ill/cancer", arr1, arr2, arr3));

    QString txt1 = tests2[0]->txt1;
    QString txt2 = tests2[0]->txt2;
    QString txt3 = tests2[0]->txt3;
    UtilsMergeText::merge(txt1, txt2, txt3, arr1, arr2);

    unsigned int Success = 0;
    for(int i=0;i<8;++i)
    {
        int id1 = arr1.at(i)->id;
        int id2 = arr3.at(i)->id;
        QString key1 = arr1.at(i)->key;
        QString key2 = arr3.at(i)->key;
        QString line1 = arr1.at(i)->line;
        QString line2 = arr3.at(i)->line;
        if(id1==id2 && key1==key2 && line1==line2) Success+=1;
        else
        {
            std::cout << "In the sort test in the element №" << i+1 << ":\n";
            if(id1!=id2) std::cout << "expected id: " << id2
                                   << ", but obtained: " << id1 << std::endl;
            if(key1!=key2) std::cout  << "expected key: " << key2.toStdString()
                                      << ", but obtained: " << key1.toStdString() << std::endl;
            if(line1!=line2) std::cout  << "expected line: " << line2.toStdString()
                                        << ", but obtained: " << line1.toStdString() << std::endl;
        }
    }

    if(Success==8) nSuccess++;

    return nSuccess == tests1.size()+1;
}

QString UnitTestMergeText::loadFileFromRes(QString res_name){
    QFile file_res(res_name);
    if (!file_res.open(QIODevice::ReadOnly)){
        std::cerr << "Could not open file: '" << res_name.toStdString() << "'\n";
        return "";
    }
    QTextStream stream_res( &file_res );
    QString sFileContent;
    sFileContent.append(stream_res.readAll());
    file_res.close();
    return sFileContent;
}
