#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef struct train_a
{
    string train_name, source, distination;
    int hh, mm;
    int remaining_capacity;
} train_a;

typedef struct train_w
{
    string source, destination;
    int hh, mm;
    int count;
} train_w;

int main()
{
    int n, m;
    char colon;
    string train_name, source, distination;
    int hh, mm;
    int remaining_capacity;
    int count;
    cin >> n >> m;
    vector<train_a> train_a_list(n);
    vector<train_w> train_w_list(n);
    for (int i = 0; i < n; i++)
    {
        cin >> train_name >> source >> distination >> hh >> colon >> mm >> remaining_capacity;
        (train_a_list[i]).train_name = train_name;
        (train_a_list[i]).source = source;
        (train_a_list[i]).distination = distination;
        (train_a_list[i]).hh = hh;
        (train_a_list[i]).mm = mm;
        (train_a_list[i]).remaining_capacity = remaining_capacity;
    }
    for (int i = 0; i < m; i++)
    {
        cin >> source >> distination >> hh >> colon >> mm >> count;
        (train_w_list[i]).source = source;
        (train_w_list[i]).destination = distination;
        (train_w_list[i]).hh = hh;
        (train_w_list[i]).mm = mm;
        (train_w_list[i]).count = count;
    }
    for (int j = 0; j < m; j++)
    {
        for (int i = 0; i < n; i++)
        {
            if (((train_a_list[i]).source == (train_w_list[j]).source) &&
                ((train_a_list[i]).distination == (train_w_list[j]).destination))
            {
                if ((train_w_list[j]).count <= (train_a_list[i]).remaining_capacity)
                {
                    if ((train_w_list[j]).hh < (train_a_list[i]).hh)
                    {
                        cout << (train_a_list[i]).train_name << " ";
                        if ((train_a_list[i].hh) < 10)
                        {
                            cout << "0";
                        }
                        cout << (train_a_list[i]).hh << ":";
                        if (train_a_list[i].mm < 10)
                        {
                            cout << "0";
                        }
                        cout << (train_a_list[i]).mm << " " << (train_a_list[i]).remaining_capacity
                             << endl;
                    }
                    else if ((train_w_list[j]).hh == (train_a_list[i]).hh)
                    {
                        if ((train_w_list[j]).mm < (train_a_list[i]).mm)
                        {
                            cout << (train_a_list[i]).train_name << " ";
                            if ((train_a_list[i].hh) < 10)
                            {
                                cout << "0";
                            }
                            cout << (train_a_list[i]).hh << ":";
                            if (train_a_list[i].mm < 10)
                            {
                                cout << "0";
                            }
                            cout << (train_a_list[i]).mm << " " << (train_a_list[i]).remaining_capacity
                                 << endl;
                        }
                    }
                }
            }
        }
        cout << "----------" << endl;
    }
    return 0;
}