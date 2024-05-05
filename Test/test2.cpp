#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<int> array(n);
    for (int i = 0; i < n; i++)
    {
        array[i] = i;
    }
    for (int i = 0; i < n; i++)
    {
        cout << array[i] << endl;
    }
    return 0;
}