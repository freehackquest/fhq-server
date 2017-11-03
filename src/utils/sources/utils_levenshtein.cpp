#include <utils_levenshtein.h>
#include <QVector>
int UtilsLevenshtein::find_min(int a,int b, int c)
{
        if(a < b && a < c)
                return a;
        else if(b < a && b < c)
                return b;
        else
                return c;
};
int UtilsLevenshtein::distance(const QString &one, const QString &two){
        int len_one = one.length(), len_two = two.length();
        QVector<int> distances_one; //[len_two + 1];
        QVector<int> distances_two; //[len_two + 1];
        int substitution_cost;

        for (int i = 0; i < len_two; i++)
        {
                distances_one.push_back(i);
        };
        for (int i = 0; i < len_one - 1; i++) {
                distances_two.push_back(i + 1);
                for (int j = 0; j < len_two - 1; j++) {
                        if (one[i] == two[j]) {
                                substitution_cost = 0;
                        } else {
                                substitution_cost = 1;
                        }
                        distances_two[j + 1] = find_min(distances_two[j] + 1, distances_one[j + 1] + 1, distances_one[j] + substitution_cost);
                };
        };
        return distances_two[len_two];
};
