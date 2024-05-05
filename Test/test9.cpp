#include <iostream>
#include <string>
using namespace std;

int main()
{
    string str1 = "Amir";
    string str2 = "Borna";
    string size = str1;
    if (str1.size() < str2.size()) size = str2;
    for (int i = 0; i < size.size(); i++)
    {
        if (str1[i] == str2[i]) continue;
        else if (str1[i] < str2[i])
        {
            cout << str1 << endl;
            break;
        }
        else
        {
            cout << str2 << endl;
            break;
        }
    }
    if (str1 == str2)
        cout << "Yes" << endl;
    else cout << "No" << endl;
    return 0;
}