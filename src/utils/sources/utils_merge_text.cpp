#include <utils_merge_text.h>
#include <QStringList>

void UtilsMergeText::compare(QString &txt1, QString &txt2, std::vector<row *> &arr)
{
    QStringList list1 = txt1.split("/");
    QStringList list2 = txt2.split("/");
    QStringList sWord = (QStringList() << "add" << "del" << "rep");
    int len1 = list1.size();
    int len2 = list2.size();
    int iAdd=0, jAdd=0;
    for(int i=0;i+iAdd<len1;i++){
        if(list1[i+iAdd]!=list2[i+jAdd]){
            for(int j=i;j+jAdd<len2;j++){
                if(list1[i+iAdd]==list2[j+jAdd]){
                    arr.push_back(new row(i+iAdd, j+jAdd, sWord.at(0), list2.at(j+jAdd)));
                    ++jAdd;
                    goto exit;
                }
            }
            for(int j=i;j+iAdd<len2;j++){
                if(list1[j+iAdd]==list2[i+jAdd]){
                    arr.push_back(new row(j+iAdd, i+jAdd, sWord.at(1), list1.at(j+iAdd)));
                    ++iAdd;
                    goto exit;
                }
            }
            arr.push_back(new row(i+iAdd, i+jAdd, sWord.at(2), list2.at(i+jAdd)));
            exit:;
        }
    }
}

void UtilsMergeText::merge(QString &curtxt, QString &txt1, QString &txt2, std::vector<row *> &arr1, std::vector<row *> &arr2){
    compare(txt1, txt2, arr1);
    compare(txt1, curtxt ,arr2);
    //TO DO compare
    arr1.resize(arr1.size()+arr2.size());
    std::merge(arr1.begin(), arr1.end(), arr2.begin(), arr2.end(), arr1.begin());
    //std::sort(arr1.begin(), arr1.end(), rowCmp());
}
