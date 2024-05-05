#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#define ENTER '\n'
#define COMMA ','
using namespace std;

typedef struct offered
{
    string course_name;
    int units;
    vector<string> prerequisites;
} offered;

typedef struct previous
{
    string course_name;
    int units;
    float grade;
} previous;

vector<offered> getOffered();
vector<previous> getPrevious();
vector<string> getRequested();
const float gpa(vector<previous> previous_list);
int units_check(const float GPA, int units);
void checkAndOut(vector<offered> offered_list, vector<previous> previous_list,
                 vector<string> requested_list, float GPA);

int main()
{
    vector<offered> offered_list = getOffered();
    vector<previous> previous_list = getPrevious();
    vector<string> requested_list = getRequested();
    const float GPA = gpa(previous_list);
    checkAndOut(offered_list, previous_list, requested_list, GPA);
    return 0;
}
vector<offered> getOffered()
{
    int m;
    string prerequisites;
    char buffer_input;
    cin >> m;
    vector<offered> offered_list(m);
    for (int i = 0; i < m; i++)
    {
        cin >> (offered_list[i]).course_name >> (offered_list[i]).units;
        getline(cin, prerequisites, ENTER);
        stringstream ss(prerequisites);
        while (getline(ss, prerequisites, COMMA))
        {
            ((offered_list[i]).prerequisites).push_back(prerequisites);
        }
        ((offered_list[i]).prerequisites[0]).erase(0, 1);
    }
    return offered_list;
}
vector<previous> getPrevious()
{
    int n;
    cin >> n;
    vector<previous> previous_list(n);
    for (int i = 0; i < n; i++)
    {
        cin >> (previous_list[i]).course_name >> (previous_list[i]).units >>
            (previous_list[i]).grade;
    }
    return previous_list;
}
vector<string> getRequested()
{
    int k;
    cin >> k;
    vector<string> requested_list(k);
    for (int i = 0; i < k; i++)
    {
        cin >> requested_list[i];
    }
    return requested_list;
}
const float gpa(vector<previous> previous_list)
{
    float total_sum = 0;
    int units_sum = 0;
    for (int i = 0; i < previous_list.size(); i++)
    {
        total_sum += ((previous_list[i]).units) * ((previous_list[i]).grade);
        units_sum += (previous_list[i]).units;
    }
    const float GPA = total_sum / units_sum;
    return GPA;
}
int units_check(const float GPA, int units)
{
    const float GPA_A = 17, GPA_C = 12, UNITS_A = 24, UNITS_B = 20, UNITS_C = 14;
    if (GPA >= GPA_A)
    {
        if (units <= UNITS_A)
            return 1;
        else
            return 0;
    }
    else if (GPA_C <= GPA < GPA_A)
    {
        if (units <= UNITS_B)
            return 1;
        else
            return 0;
    }
    else
    {
        if (units <= UNITS_C)
            return 1;
        else
            return 0;
    }
}
void checkAndOut(vector<offered> offered_list, vector<previous> previous_list,
                 vector<string> requested_list, float GPA)
{
    const int min_units = 12;
    const int min_grade = 10;
    int counter_offered = 0;
    int counter_previous_first = 0;
    int counter_previous_second = 0;
    int counter_prerequisites = 0;
    int units = 0;

    for (int i = 0; i < requested_list.size(); i++)
    {
        for (int j = 0; j < offered_list.size(); j++)
        {
            if (requested_list[i] == (offered_list[j]).course_name)
            {
                counter_offered++;
                units += (offered_list[j]).units;
            }
        }
    }
    if (counter_offered != requested_list.size())
        cout << "Course Not Offered!" << endl;
    else
    {
        if (units < min_units)
            cout << "Minimum Units Validation Failed!" << endl;
        else
        {
            if (!units_check(GPA, units))
                cout << "Maximum Units Validation Failed!"
                     << endl;
            else
            {
                for (int i = 0; i < requested_list.size(); i++)
                {
                    for (int j = 0; j < previous_list.size(); j++)
                    {
                        if (requested_list[i] == (previous_list[j]).course_name &&
                            (previous_list[j].grade >= min_grade))
                            counter_previous_first++;
                    }
                }
                if (counter_previous_first != 0)
                    cout << "Course Already Taken!" << endl;
                else
                {
                    for (int i = 0; i < requested_list.size(); i++)
                    {
                        for (int j = 0; j < offered_list.size(); j++)
                        {
                            if (requested_list[i] == (offered_list[j]).course_name)
                            {
                                for (int y = 0; y < (offered_list[j]).prerequisites.size(); y++)
                                {
                                    for (int z = 0; z < previous_list.size(); z++)
                                    {
                                        if (((offered_list[j]).prerequisites[y] ==
                                             (previous_list[z]).course_name) &&
                                            previous_list[z].grade >= min_grade)
                                            counter_previous_first++;
                                    }
                                }
                                if (counter_previous_first == ((offered_list[j]).prerequisites).size())
                                {
                                    counter_previous_second++;
                                }
                                counter_previous_first = 0;
                            }
                        }
                    }
                    if (counter_previous_second != requested_list.size())
                        cout << "Prerequisites Not Met!" << endl;
                    else
                        cout << "OK!" << endl;
                }
            }
        }
    }
}