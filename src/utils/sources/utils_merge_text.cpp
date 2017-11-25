#include <utils_merge_text.h>

void UtilsMergeText::compare(QString &txt1, QString &txt2, std::vector<row> &arr)
{
    //beat text to strings, compare and push them into vector
    row result;
    if(txt1!=txt2){
        result.id1=1;
        result.id2=1;
        result.key="rep";
        result.line="txt2";
    }

}

/*void UtilsMergeText::merge(QString &curtxt, QString &txt1, QString &txt2)
{
    //beat text to strings and push it to list
    //hardmade comparing system 1.0
    //hardmade comparing system 2.0
    //maybe 3.0...
    //return array of changes
}*/
