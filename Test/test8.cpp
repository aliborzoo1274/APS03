#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    float num = 250;
    float num1 = num/6;
    cout << num1 << endl;
    num1 = floor(num1 * 100)/100;
    cout << num1 << endl;
    return 0;
}