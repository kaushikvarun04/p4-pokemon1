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
    uint32_t trueCntr;
    currBest() : weight(positiveInfinity), trueCntr(1) {}
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
            printFast();

       
            // vector<size_t> soln = {0};
            // currBest best;
            // double currWeight = 0;
            // checkNode(coordinateList[0], soln, best, currWeight);
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
        return distance;
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to calculate Euclidean distance between two coordinates
    double distance(const Coordinate& a, const Coordinate& b) {
        double x = a.x_cord - b.x_cord;
        double y = a.y_cord - b.y_cord;
        return sqrt(x * x + y * y);
    }

    // Function to find the best place to insert a vertex into a partial tour
    void insertVertex(std::vector<size_t>& partialTour, size_t k) {
        double minCostIncrease = positiveInfinity;
        size_t bestInsertIndex = 0;

        for (size_t n = 0; n < partialTour.size() - 1; ++n) {
            size_t i = partialTour[n];
            size_t j = partialTour[n + 1];
            double currentCostIncrease = distance(coordinateList[i], coordinateList[k]) +
                                        distance(coordinateList[k], coordinateList[j]) -
                                        distance(coordinateList[i], coordinateList[j]);

            if (currentCostIncrease < minCostIncrease) {
                minCostIncrease = currentCostIncrease;
                bestInsertIndex = n + 1; // Insert after `u`
            }
        }

        // Insert vertex into the best position found
        partialTour.insert(partialTour.begin() + static_cast<int>(bestInsertIndex), k);
    }

    // Function to compute the total cost of a given tour
    double computeTourCost(const std::vector<size_t>& tour) {
        double totalCost = 0.0;
        for (size_t i = 0; i < tour.size() - 1; ++i) {
            totalCost += distance(coordinateList[tour[i]], coordinateList[tour[i + 1]]);
        }
        return totalCost;
    }

    // Arbitrary Insertion Branch-and-Bound TSP Solver
    vector<size_t> solveTSP() {
        if (totalCnt < 2) return {}; // Handle edge cases

        // Step 1: Initialize a partial tour with the first vertex
        // Step 2: Add an arbitrary second vertex
        std::vector<size_t> partialTour = {0,1,0};


        // Step 3-5: Insert remaining vertices into the partial tour
        for (size_t i = 2; i < totalCnt; ++i) {
            insertVertex(partialTour, i);
        }

        return partialTour;
    }


    void printFast(){
        vector<size_t> output = solveTSP();
        double cost = computeTourCost(output);
        cout<<cost<<"\n";
        for(size_t i = 0; i < totalCnt; ++ i){
            cout<<output[i] << " ";
        }
        
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




























/*
1. Initialize a partial tour with a vertex , chosen arbitrarily (you can just start with the first vertex available).
2. Choose another arbitrary vertex  and set the initial partial tour to  i→ j →i .
3. Arbitrarily select a vertex  that is currently not in the partial tour.
4. Find the best place to insert vertex  into the partial tour to minimize cost. To do this, first identify the edge (, ) in the partial path
such that  + − is minimal (where  denotes the distance between  and ), and then insert  between  and . Notice that
 + − represents the change in overall cost after vertex  is added in between vertices  and , since you are removing the cost
between  and  and adding the costs incurred by connecting  with  and .
5. Once all the vertices have been added to the path, the algorithm completes. Otherwise, repeat steps 3-5 until all vertices have been added.

*/


    //THIS IS PART C!!!
    // try to really understand how the recurssion here is suppsoed to work
    // void checkNode(Coordinate c, vector<size_t> &soln, currBest &best, double currWeight)
    // {
    //     // need to create an inital loop with just 0,1,0 and then add these to that...
    //     //  arbitrary insertion
    //     if (soln.size() <= totalCnt)
    //         for (size_t i = 1; i < totalCnt; ++i)
    //         {
    //             //            if (prim.trueValues[i].visited == false) //check if the value we're inserting is in the soln
    //             if (find(soln.begin(), soln.end(), i) == soln.end())
    //             {
    //                 // size_t prev = soln.back();
    //                 Coordinate u = coordinateList[i];
    //                 soln.push_back(i);
    //                 double x = u.x_cord - c.x_cord;
    //                 double y = u.y_cord - c.y_cord;
    //                 double edgeweight = sqrt((x * x) + (y * y));
    //                 if (promising(soln, currWeight + edgeweight, best))
    //                 {
    //                     currWeight += edgeweight;
    //                     if (solution(soln))
    //                     {
    //                         update(soln, currWeight + edgeweight, best);
    //                     }else
    //                     {
    //                         for (size_t i = 0; i < totalCnt; ++i)
    //                         {
    //                             if (find(soln.begin(), soln.end(), i) == soln.end())
    //                             {
    //                                 size_t prev = soln.back();
    //                                 soln.push_back(i);
    //                                 Coordinate test = coordinateList[i];
    //                                 double x = test.x_cord - coordinateList[prev].x_cord;
    //                                 double y = test.y_cord - coordinateList[prev].y_cord;
    //                                 double edgeweight = sqrt((x * x) + (y * y));
    //                                 checkNode(test, soln, best, currWeight + edgeweight);
    //                                 soln.pop_back(); 
    //                                 //currWeight -= edgeWeight;
    //                             }
    //                         }
    //                     }
    //                 }else{
    //                     soln.pop_back();
    //                     //currWeight-=edgeWeight
    //                 }
    //             } 
    //         }
    // }

    // void update(vector<size_t> o, double w, currBest &best)
    // {
    //     best.output = o;
    //     best.weight = w;
    // }

    // bool promising(vector<size_t> soln, double currWeight, currBest best)
    // {
    //     // need to check how to only run for the values not in the solution rn
    //     // need to check how to remove invalid solns, idk if there will be any tho
    //     prim.parentValues.resize(totalCnt);
    //     prim.trueValues.resize(totalCnt);
    //     prim.distanceValues.resize(totalCnt);
    //     if (totalCnt - soln.size() < 4)
    //     {
    //         return true;
    //     }
    //     else if (currWeight + B_C_createMST() < best.weight)
    //     {
    //         return true;
    //     }
    //     return false;
    // }

    // bool solution(vector<size_t> soln)
    // {
    //     if (soln.size() >= totalCnt)
    //     {
    //         return true;
    //     }
    //     return false;
    // }
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
    poke.readInputs(cin, mode);
    // do something
}