// 5949F553E20B650AB0FB2266D3C0822B13D248B0
#include <getopt.h>
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include "xcode_redirect.hpp"
#include <queue>
#include <cstdint>
#include <stdint.h>
#include <limits.h>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <limits>
#include <queue>
#include <iomanip>
using namespace std;
double positiveInfinity = std::numeric_limits<double>::infinity();

// double negativeInfinity = -std::numeric_limits<double>::infinity();

struct Coordinate
{
    int x_cord;
    int y_cord;
    char terrain; // Coast, Land, Ocean
};

struct kv
{
    bool visited;
    kv()
        : visited(false) {}
    // bool operator(visited){
    //     return visited;
    // }
};
struct dv
{
    double distance; // = positiveInfinity; // need to check this...
    dv() : distance(positiveInfinity) {}
};

struct pv
{
    size_t parent;
    // need to fix
    pv() : parent(100) {}
};
struct PrimDS
{
    // index these by vertex
    vector<kv> kv;
    vector<dv> dv;
    vector<pv> pv;
};
class Pokemon
{
    PrimDS prim;
    vector<Coordinate> coordinateList;

public:
    void readInputs(istream &cin, string mode)
    {
        Coordinate c;
        // there is probably a better way to do this
        size_t totalCnt = 0;
        size_t landCntr = 0;
        size_t coastCntr = 0;
        size_t oceanCntr = 0;
        coordinateList.resize(totalCnt);
        cin >> totalCnt;
        while (landCntr + coastCntr + oceanCntr < totalCnt)
        {
            cin >> c.x_cord >> c.y_cord;
            if (c.x_cord < 0 && c.y_cord < 0)
            {
                c.terrain = 'O';
                oceanCntr++;
            }
            else if (c.x_cord == 0 || c.y_cord == 0)
            {
                c.terrain = 'C';
                coastCntr++;
            }
            else
            {
                c.terrain = 'L';
                landCntr++;
            }
            coordinateList.push_back(c);
        }
        if (mode == "MST")
        {
            prim.kv.resize(totalCnt);
            prim.pv.resize(totalCnt);
            prim.dv.resize(totalCnt);
            if (landCntr > 1 && oceanCntr > 1 && coastCntr < 1)
            {
                cerr << "Cannot construct MST";
                exit(1);
            }
            else
            {
                createMST();
                printMST();
            }
        }
    }
    void createMST()
    {
        prim.dv[0].distance = 0;

        size_t trueCntr = 0;
        // Set starting point dv to 0.
        //  Loop v times (until every kv is true):
        while (trueCntr < coordinateList.size())
        {
            size_t bestLoc = 0; // might need to default initialize this
            double bestDist = positiveInfinity;
            // 1. From the set of vertices for which kv is false, select the vertex v having the
            // smallest tentative distance dv.
            for (size_t i = 0; i < coordinateList.size(); ++i)
            {

                if (prim.kv[i].visited == false)
                {
                    if (bestDist > prim.dv[i].distance)
                    {
                        bestDist = prim.dv[i].distance;
                        bestLoc = i;
                    }
                }
            }
            // 2. Set kv to true.
            prim.kv[bestLoc].visited = true;
            ++trueCntr;
            // 3. For each vertex w adjacent to v for which kw is false, test whether dw is
            // greater than distance(v,w). If it is, set dw to distance(v,w) and set pw to v.
            char t = coordinateList[bestLoc].terrain;
            for (size_t i = 0; i < coordinateList.size(); ++i)
            {
                if ((coordinateList[i].terrain == t || t == 'C') && prim.kv[i].visited == false)
                {
                    // will probably need to fix this
                    double x = coordinateList[bestLoc].x_cord - coordinateList[i].x_cord;
                    double y = coordinateList[bestLoc].y_cord - coordinateList[i].y_cord;
                    double val = (x * x) + (y * y);
                    if (prim.dv[i].distance > val)
                    {
                        prim.dv[i].distance = val; // curr stores squared distances, need to sqrt later
                        prim.pv[i].parent = bestLoc;
                    }
                }
            }
        }
    }

    void printMST()
    {
        double distance = 0;
        vector<pair<size_t, size_t>> output;
        for (size_t i = 0; i < coordinateList.size(); ++i)
        {
            distance += sqrt(prim.dv[i].distance);
            for (size_t j = 0; j < coordinateList.size(); ++j)
            {
                if (prim.pv[j].parent == i)
                {
                    pair<size_t, size_t> p;
                    p.first = min(i, j);
                    p.second = max(i, j);
                    output.push_back(p);
                    break;
                }
            }
        }
        cout << distance << "\n";
        for (size_t i = 0; i < output.size(); ++i)
        {
            cout << output[i].first << " " << output[i].second << "\n";
        };
    }
};

int main(int argc, char *argv[])
{
    cout << std::setprecision(2); // Always show 2 decimal places
    cout << std::fixed;           // Disable scientific notation for large numbers
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;     // for every getopt
    int index = 0;  // for every getopt
    string mode;
    // Only open if a filename is given

    option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"mode", required_argument, nullptr, 'm'},
        {nullptr, 0, nullptr, '\0'},
    }; // long options

    while ((choice = getopt_long(argc, argv, "hm:", long_options, &index)) != -1)
    {
        switch (choice)
        {
        case 'h':
        {
            cout << "Usage: " << argv[0] << " hm: \n";
            cout << "This is a program to that uses tress with pokemon";
        }
        case 'm':
        {
            mode = optarg;
            if (mode != "MST" && mode != "FASTTSP" && mode != "OPTTSP")
            {
                cerr << "Error: Invalid mode";
            }
        }
        }
    }

    Pokemon poke;
    if (mode == "MST")
    {
        poke.readInputs(cin, mode);
        // do something
    }
}