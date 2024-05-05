#include <iostream>
#include <vector>
using namespace std;

struct str
{
    int num1;
    int num2;
};

int main()
{
    vector<str> list;
    int number1 = 65;
    int number2 = 78;
    list.push_back({number1, number2});
    for (int i = 0; i < list.size(); i++)
    {
        cout << list[i].num1 << endl;
        cout << list[i].num2 << endl;
    }
    return 0;
}