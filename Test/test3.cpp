#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

int main()
{
    vector<string> v;
    string s;
    getline(cin, s, '\n');
    stringstream ss(s);
    while (getline(ss, s, ','))
    {
        v.push_back(s);
    }
    cout << "The vector contains: ";
    for (auto x : v)
    {
        cout << x << " " << endl;
    }
    cout << v.size() << endl << v[1];
    cout << endl;
    return 0;
}