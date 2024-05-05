#include <iostream>
using namespace std;

int transformation(long long number);
int main()
{
    long long number;
    int result;
    cin >> number;
    result = transformation(number);
    cout << result;
    return 0;
}
int transformation(long long number)
{
    long long reachToNum = 1;
    long long from;
    long long to;
    long long distanceFrom;
    long long distanceTo;
    long long usedNum;
    long long nextNum;
    int result = 0;
    while (reachToNum < number)
    {
        reachToNum = reachToNum * 10 + 1;
    }
    from = (reachToNum - 1) / 10;
    to = reachToNum;
    distanceFrom = number - from;
    distanceTo = to - number;
    if (distanceFrom <= distanceTo)
        usedNum = from;
    else
        usedNum = to;
    nextNum = number - usedNum;
    while (usedNum > 0)
    {
        usedNum /= 10;
        result += 1;
    }
    if (nextNum == 0)
        return result;
    else
        result += transformation(abs(nextNum));
    return result;
}