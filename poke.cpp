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
#include <cmath>
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
    pv() : parent(SIZE_MAX) {}
};
struct PrimDS
{
    // index these by vertex
    vector<kv> trueValues;
    vector<dv> distanceValues;
    vector<pv> parentValues;
};

struct currBest
{
    vector<size_t> output;
    double weight;
    currBest() : weight(positiveInfinity) {}
};
class Pokemon
{
    PrimDS prim;
    vector<Coordinate> coordinateList;
    size_t totalCnt = 0;

public:
    void readInputs(istream &cin, string mode)
    {
        Coordinate c;
        // there is probably a better way to do this
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
            prim.trueValues.resize(totalCnt);
            prim.parentValues.resize(totalCnt);
            prim.distanceValues.resize(totalCnt);
            // idk how/if this will works
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
        if (mode == "FASTTSP")
        {
            vector<size_t> soln;
            soln.resize(totalCnt);
            prim.trueValues.resize(totalCnt);
            currBest best;
            checkNode(coordinateList[0], soln);
        }
    }
    void createMST()
    {
        prim.distanceValues[0].distance = 0;
        prim.parentValues[0].parent = 0;

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

                if (prim.trueValues[i].visited == false)
                {
                    if (prim.distanceValues[i].distance < bestDist)
                    {
                        bestDist = prim.distanceValues[i].distance;
                        bestLoc = i;
                    }
                }
            }
            // 2. Set kv to true.
            prim.trueValues[bestLoc].visited = true;
            ++trueCntr;
            // 3. For each vertex w adjacent to v for which kw is false, test whether dw is
            // greater than distance(v,w). If it is, set dw to distance(v,w) and set pw to v.
            char t = coordinateList[bestLoc].terrain;
            for (size_t i = 0; i < coordinateList.size(); ++i)
            {
                // something wrong here
                if ((coordinateList[i].terrain == t || t == 'C' || coordinateList[i].terrain == 'C') && prim.trueValues[i].visited == false)
                {
                    // will probably need to fix this
                    double x = coordinateList[bestLoc].x_cord - coordinateList[i].x_cord;
                    double y = coordinateList[bestLoc].y_cord - coordinateList[i].y_cord;
                    double val = (x * x) + (y * y); // this might be where the bug is
                    if (prim.distanceValues[i].distance > val)
                    {
                        prim.distanceValues[i].distance = val; // curr stores squared distances, need to sqrt later
                        prim.parentValues[i].parent = bestLoc;
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
            if (prim.parentValues[i].parent != i)
            {
                distance += sqrt(prim.distanceValues[i].distance);
                pair<size_t, size_t> p;
                p.first = min(i, prim.parentValues[i].parent);
                p.second = max(i, prim.parentValues[i].parent);
                output.push_back(p);
            }
        }
        cout << distance << "\n";
        for (size_t i = 0; i < output.size(); ++i)
        {
            cout << output[i].first << " " << output[i].second << "\n";
        };
    }

    double B_C_createMST()
    {
        prim.distanceValues[0].distance = 0;
        prim.parentValues[0].parent = 0;

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

                if (prim.trueValues[i].visited == false)
                {
                    if (prim.distanceValues[i].distance < bestDist)
                    {
                        bestDist = prim.distanceValues[i].distance;
                        bestLoc = i;
                    }
                }
            }
            // 2. Set kv to true.
            prim.trueValues[bestLoc].visited = true;
            ++trueCntr;
            // 3. For each vertex w adjacent to v for which kw is false, test whether dw is
            // greater than distance(v,w). If it is, set dw to distance(v,w) and set pw to v.
            char t = coordinateList[bestLoc].terrain;
            for (size_t i = 0; i < coordinateList.size(); ++i)
            {
                if (prim.trueValues[i].visited == false)
                {
                    // will probably need to fix this
                    double x = coordinateList[bestLoc].x_cord - coordinateList[i].x_cord;
                    double y = coordinateList[bestLoc].y_cord - coordinateList[i].y_cord;
                    double val = (x * x) + (y * y); // this might be where the bug is
                    if (prim.distanceValues[i].distance > val)
                    {
                        prim.distanceValues[i].distance = val; // curr stores squared distances, need to sqrt later
                        prim.parentValues[i].parent = bestLoc;
                    }
                }
            }
        }
        double distance = 0;
        for (size_t i = 0; i < coordinateList.size(); ++i)
        {
            if (prim.parentValues[i].parent != i)
            {
                distance += sqrt(prim.distanceValues[i].distance);
            }
        }
    }

    void checkNode(Coordinate c, vector<size_t> soln)
    {
        // arbitrary insertion
        uint32_t trueCntr = 1;
        for (int i = 1; i < totalCnt; ++i)
        {
            if (prim.trueValues[i].visited == false)
            {
                Coordinate u = coordinateList[i];
                // this might cause a lot of problems...
                prim.trueValues[i].visited = true;
                ++trueCntr; // this might be rlly bad

                break;
            }
        }
        if (promising(c, trueCntr))
        {
            if (solution(trueCntr))
            {
                update(soln, )
            }
        }
    }

    void update(vector<size_t> o, double w)
    {
        best.output = o;
        best.weight = w;
    }

    bool promising(Coordinate c, uint32_t tc)
    {
        // need to check how to only run for the values not in the solution rn
        // need to check how to remove invalid solns, idk if there will be any tho
        prim.parentValues.resize(totalCnt);
        prim.distanceValues.resize(totalCnt);
        if (totalCnt - tc < 4)
        {
            return true;
        }
        else if (best.weight + B_C_createMST() < b.weight)
        {
            return true;
        }
        return false;
    }

    bool solution(uint32_t tc)
    {
        if (tc == totalCnt)
        {
            return true;
        }
        return false;
    }
};

/*
Algorithm checknode(Node v, Best currBest)
Node u
if (promising(v, currBest))
    if (solution(v)) then
        update(currBest)
else
    for each child u of v
        checknode(u, currBest)
return currBest

*/

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
            break;
        }
        case 'm':
        {
            mode = optarg;
            if (mode != "MST" && mode != "FASTTSP" && mode != "OPTTSP")
            {
                cerr << "Error: Invalid mode";
            }
            break;
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