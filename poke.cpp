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
    uint64_t x_cord;
    uint64_t y_cord;
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
    pv(size_t a) : parent(a) {}
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
    void readInputs(istream &cin, string mode)
    {
        Coordinate c;
        cin >> c.x_cord >> c.y_cord;
        if (c.x_cord < 0 && c.y_cord < 0)
        {
            c.terrain = 'O';
        }
        else if (c.x_cord == 0 || c.y_cord == 0)
        {
            c.terrain = 'C';
        }
        else
        {
            c.terrain = 'L';
        }
        if (mode == "MST")
        {
            size_t a = coordinateList.size();
            prim.kv.resize(a);
            prim.pv.resize(a);
            prim.dv.resize(a);
        }
    }
    void createMST()
    {
        prim.dv[0].distance = 0;
        size_t bestLoc = 0; // might need to default initialize this
        double bestDist = positiveInfinity;
        size_t trueCntr = 0;
        // Set starting point dv to 0.
        //  Loop v times (until every kv is true):
        while (trueCntr < coordinateList.size())
        {
            // 1. From the set of vertices for which kv is false, select the vertex v having the
            // smallest tentative distance dv.
            for (int i = 0; i < coordinateList.size(); ++i)
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
            for (int i = 0; i < coordinateList.size(); ++i)
            {
                if (coordinateList[i].terrain == t && prim.kv[i].visited == false)
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

    void printMST(){
        //do something
        
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
    bool verbose = false;
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
            if (mode != "MST" || mode != "FASTTSP" || mode != "OPTTSP")
            {
                cerr << "Error: Invalid mode";
            }
        }
        }
    }

    if (mode == "MST")
    {
        // do something
    }
}