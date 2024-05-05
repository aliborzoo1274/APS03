#include <iostream>
using namespace std;

int main()
{
    int hh, mm;
    char colon;
    cout << "Please enter the time:" << endl;
    cin >> hh >> colon >> mm;
    cout << "The time is " << hh << colon << mm << endl;
    return 0;
}