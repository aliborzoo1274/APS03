#include <iostream>
using namespace std;

class test
{
    private:
        int time = 0;
        int time1 = 2;
    public:
        void increase(int value) {time =+ value;}
        int get_time() {return time;}
        int get_time1() {return time1;}
};

int main()
{
    test test;
    test.increase(5);
    cout << test.get_time() << ' ' << test.get_time1() << endl;
    return 0;
}