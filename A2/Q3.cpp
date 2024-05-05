#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

const int STATIONS_DISTANCE = 1;
const int CHANGE_LINE = 2;

struct Line
{
    int numOfStations;
    vector<int> stations;
};

struct Edge
{
    int to;
    int weight;
};

vector<Line> getInput(int numOfLines);
void getSrcAndDst(int &src, int &dst, vector<Line> LineList);
vector<vector<Edge>> makeGraph(const vector<Line> &LineList);
void makeChangeLine(vector<vector<Edge>> &graph, const vector<Line> &LineList);
vector<int> dijkstra(const vector<vector<Edge>> &graph, int src);
void dijkstra(const vector<vector<Edge>> &graph, vector<int> &dist, int from);

int main()
{
    int numOfLines;
    int src = 0;
    int dst = 0;
    cin >> numOfLines;
    vector<Line> LineList = getInput(numOfLines);
    getSrcAndDst(src, dst, LineList);
    vector<vector<Edge>> graph = makeGraph(LineList);
    vector<int> dist(graph.size(), INT_MAX);
    dijkstra(graph, dist, src);
    cout << dist[dst];
    return 0;
}

vector<Line> getInput(int numOfLines)
{
    int station;
    vector<Line> LineList(numOfLines);
    for (int i = 0; i < numOfLines; i++)
    {
        cin >> LineList[i].numOfStations;
        for (int j = 0; j < LineList[i].numOfStations; j++)
        {
            cin >> station;
            LineList[i].stations.push_back(station);
        }
    }
    return LineList;
}

void getSrcAndDst(int &src, int &dst, vector<Line> LineList)
{
    int srcLine, srcStation, dstLine, dstStation;
    cin >> srcLine >> srcStation >> dstLine >> dstStation;
    for (int i = 0; i < srcLine - 1; i++)
    {
        src += LineList[i].numOfStations;
    }
    src += srcStation - 1;
    for (int i = 0; i < dstLine - 1; i++)
    {
        dst += LineList[i].numOfStations;
    }
    dst += dstStation - 1;
}

vector<vector<Edge>> makeGraph(const vector<Line> &LineList)
{
    int V = 0;
    int stationNumber = 0;
    for (int i = 0; i < LineList.size(); i++)
    {
        V += LineList[i].numOfStations;
    }
    vector<vector<Edge>> graph(V);
    for (int i = 0; i < LineList.size(); i++)
    {
        if (i != 0)
            stationNumber += LineList[i - 1].numOfStations;
        for (int j = 1; j < LineList[i].numOfStations; j++)
        {
            graph[stationNumber].push_back({stationNumber + 1, STATIONS_DISTANCE});
            stationNumber++;
        }
        for (int j = 1; j < LineList[i].numOfStations; j++)
        {
            graph[stationNumber].push_back({stationNumber - 1, STATIONS_DISTANCE});
            stationNumber--;
        }
    }
    makeChangeLine(graph, LineList);
    return graph;
}

void makeChangeLine(vector<vector<Edge>> &graph, const vector<Line> &LineList)
{
    int stationCounter1 = 0;
    int stationCounter2 = 0;
    for (int i = 0; i < LineList.size(); i++)
    {
        for (int j = 0; j < LineList[i].numOfStations; j++)
        {
            if (LineList[i].stations[j] != 0)
            {
                for (int z = 0; z < LineList[(LineList[i].stations[j]) - 1].numOfStations; z++)
                {
                    if (LineList[(LineList[i].stations[j]) - 1].stations[z] == i + 1)
                    {
                        for (int x = 0; x < i; x++)
                        {
                            stationCounter1 += LineList[x].numOfStations;
                        }
                        stationCounter1 += j;
                        for (int x = 0; x < (LineList[i].stations[j]) - 1; x++)
                        {
                            stationCounter2 += LineList[x].numOfStations;
                        }
                        stationCounter2 += z;
                        graph[stationCounter1].push_back({stationCounter2, CHANGE_LINE});
                        stationCounter1 = stationCounter2 = 0;
                    }
                }
            }
        }
    }
}

void dijkstra(const vector<vector<Edge>> &graph, vector<int> &dist, int from)
{
    int to, weight;
    static priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    if (pq.empty())
    {
        dist.assign(graph.size(), INT_MAX);
        dist[from] = 0;
        pq.push({0, from});
    }
    if (!pq.empty())
    {
        from = pq.top().second;
        pq.pop();
        for (auto edge : graph[from])
        {
            to = edge.to;
            weight = edge.weight;
            if (dist[to] > dist[from] + weight)
            {
                dist[to] = dist[from] + weight;
                pq.push({dist[to], to});
                dijkstra(graph, dist, to);
            }
        }
    }
}