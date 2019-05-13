// Project 1a: Solving knapsack using exhaustive search
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include <time.h>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

void exhaustiveKnapsack(knapsack& k, int timeLimit) {
    stack<int> currSubset;
    stack<int> bestSet;
    int bestValue = 0;
    
    //exhaustively check all 2^n subsets
    for (int x = 0; x < (1 << k.getNumObjects()); x++) {
        //cost and value of each subset
        int totalCost = 0;
        int totalValue = 0;
        
        //iterate through all subset's elements
        for (int y = 0; y < k.getNumObjects(); y++) {
            if (((x >> y) & 1) == 1) {
                totalCost += k.getCost(y);
                totalValue += k.getValue(y);
                
                cout << y << " ";
                currSubset.push(y);
            }
        }
        
        //check if subset's cost is within limit and value is better than already found
        if ((totalCost <= k.getCostLimit()) && (totalValue > bestValue)) {
            bestValue = totalValue;
            
            while(!bestSet.empty()) bestSet.pop();
            while(!currSubset.empty()) {
                bestSet.push(currSubset.top());
                currSubset.pop();
            }
        }
        
        while(!currSubset.empty()) currSubset.pop();
        
        cout << endl;
    }
    
    while (!bestSet.empty()) {
        k.select(bestSet.top());
        bestSet.pop();
    }
}

int main()
{
    char x;
    ifstream fin;
    stack <int> moves;
    string fileName;
    
    // Read the name of the file from the keyboard or
    // hard code it here for testing.
    
    fileName = "/Users/danielnguyen/workspace/knapsack/knapsack/inputs1a/knapsack16.input";
    
    // cout << "Enter filename" << endl;
    // cin >> fileName;
    
    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }
    
    try
    {
        cout << "Reading knapsack instance" << endl;
        knapsack k(fin);
        
        exhaustiveKnapsack(k, 600);
        
        cout << endl << "Best solution" << endl;
        k.printSolution();
        
    }
    
    catch (indexRangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError &ex)
    {
        cout << ex.what() << endl; exit(1);
    }
}
