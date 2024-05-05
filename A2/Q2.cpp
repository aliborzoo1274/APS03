#include <iostream>
#include <string>
#include <vector>
using namespace std;

const string mainDent = "|__";
const string firstDent = "   ";
const string secondDent = "|  ";
struct Input
{
    string name;
    int reference;
};
struct Data
{
    string name;
    string reference;
    vector<string> information;
};

vector<Data> setInformation(int n, vector<Input> InputList);

void printInformation(int n, int d, string dent,
                      const vector<Data> &DataList, vector<string> information);
int main()
{
    int n, d;
    string dent;
    cin >> n >> d;
    vector<Input> InputList(n);
    for (int i = 0; i < n; i++)
    {
        cin >> InputList[i].name >> InputList[i].reference;
    }
    vector<Data> DataList = setInformation(n, InputList);
    vector<string> rootsNames;
    for (int i = 0; i < n; i++)
    {
        if (DataList[i].reference == "root")
            rootsNames.push_back(DataList[i].name);
    }
    for (int i = 0; i < n; i++)
    {
        if (DataList[i].reference == "root")
        {
            cout << mainDent << DataList[i].name << endl;
            if (DataList[i].information.size() != 0)
            {
                if (DataList[i].name == rootsNames[rootsNames.size() - 1])
                    dent = firstDent;
                else
                    dent = secondDent;
                printInformation(n, d, dent, DataList, DataList[i].information);
            }
        }
    }
    return 0;
}

vector<Data> setInformation(int n, vector<Input> InputList)
{
    vector<Data> DataList(n);
    for (int z = 0; z < n; z++)
    {
        if (InputList[z].reference == 0)
            DataList[z].reference = "root";
        else
        {
            DataList[z].reference = InputList[(InputList[z].reference) - 1].name;
            (DataList[(InputList[z].reference) - 1].information).push_back(InputList[z].name);
        }
        DataList[z].name = InputList[z].name;
    }
    return DataList;
}

void printInformation(int n, int d, string dent,
                      const vector<Data> &DataList, vector<string> information)
{
    if ((dent.size() / mainDent.size()) < d)
    {
        for (int i = 0; i < information.size(); i++)
        {
            string dentSave = dent;
            cout << dent << mainDent << information[i] << endl;
            for (int j = 0; j < n; j++)
            {
                if (information[i] == DataList[j].name &&
                    DataList[j].information.size() != 0)
                {
                    if (information[i] == information[information.size() - 1])
                        dent += firstDent;
                    else
                        dent += secondDent;
                    printInformation(n, d, dent, DataList, DataList[j].information);
                }
            }
            dent = dentSave;
        }
    }
}