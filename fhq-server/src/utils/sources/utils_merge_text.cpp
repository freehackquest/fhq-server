#include <utils_merge_text.h>
#include <QStringList>

void UtilsMergeText::compare(QString &txt1, QString &txt2, std::vector<row *> &arr)
{
    QStringList list1 = txt1.split("/");
    QStringList list2 = txt2.split("/");
    QStringList sWord = (QStringList() << "!add" << "!del");
    int len1 = list1.size();
    int len2 = list2.size();
    int i=0, j=0;
    //main comparisons
    while((i<len1)&&(j<len2)){
        if(list1[i]!=list2[j])
        {
            //checkout for added rows
            for(int k=j+1;k<len2;++k) if(list1[i]==list2[k])
            {
                while(j<k)
                {
                    arr.push_back(new row(j, sWord.at(0), list2.at(j)));
                    j++;
                }
                goto exit;
            }
            //checkout for deleted rows
            for(int k=i+1;k<len1;++k) if(list1[k]==list2[j])
            {
                while(i<k)
                {
                    arr.push_back(new row(i, sWord.at(1), list1.at(i)));
                    i++;
                }
                goto exit;
            }
            arr.push_back(new row(i, list1.at(i), list2.at(j)));
            exit:;
        }
        i++, j++;
    }
    //work with the end of the texts
    while(j<len2){
        arr.push_back(new row(j, sWord.at(0), list2.at(j)));
        j++;
    }
    while(i<len1){
        arr.push_back(new row(i, sWord.at(1), list1.at(i)));
        i++;
    }
}

void UtilsMergeText::merge(QString &curtxt, QString &txt1, QString &txt2, std::vector<row *> &arr1, std::vector<row *> &arr2){
    compare(txt1, txt2, arr1);
    compare(txt1, curtxt, arr2);
    for(unsigned int i=0;i<arr2.size();++i){
        for(unsigned int j=0;j<arr1.size();++j){
            //delete of matches and 'del'/'add' overlays from the first vector
            bool bLinesEqual = arr2.at(i)->line==arr1.at(j)->line;
            bool bKeysEqual = arr2.at(i)->key==arr1.at(j)->line;
            QString sKey1 = arr1.at(j)->key;
            QString sKey2 = arr2.at(i)->key;
            if ((bLinesEqual && (sKey1 == sKey2 || sKey1 == "!add"))
                    || (bKeysEqual && (sKey1 == "!del")))
            {
                arr1.erase(arr1.begin()+j);
                break;
            }
        }
    }
    for(unsigned int i=0;i<arr1.size();++i){
        for(unsigned int j=0;j<arr2.size();++j){
            //delete of del overlays from the second vector and update of priority
            bool bLinesEqual = arr1.at(i)->key==arr2.at(j)->line;
            bool bKeysEqual = arr1.at(i)->key==arr2.at(j)->key;
            QString sKey = arr2.at(j)->key;
            if((bLinesEqual && (sKey == "!del"))
                    || (bKeysEqual && (sKey != "!add") && (sKey != "!del")))
            {
                arr2.erase(arr2.begin()+j);
                break;
            }
        }
    }
    //merge and sort vectors
    arr1.reserve(arr1.size()+arr2.size());
    arr1.insert(arr1.end(),arr2.begin(),arr2.end());
    for(unsigned int i=0;i<arr1.size();++i)
        for(unsigned int j=arr1.size()-1;j>i;--j)
            if(arr1.at(j-1)->id > arr1.at(j)->id)
            {
                std::swap(arr1.at(j-1)->id,arr1.at(j)->id);
                std::swap(arr1.at(j-1)->key,arr1.at(j)->key);
                std::swap(arr1.at(j-1)->line,arr1.at(j)->line);
            }
}
