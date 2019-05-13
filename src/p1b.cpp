// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <fstream>
#include <limits.h>
#include <time.h>
#include <algorithm>
#include <string>
#include <vector>

#include "../include/d_except.h"
//#include "../include/knapsack.h"
#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

int const NONE = -1;  // Used to represent a node that does not exist

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

vector< vector <int> > colorCombs;
vector < pair <int, int> > combConflicts;

int numColors = 0;
int numConflicts = 0;
string getNumObjects(int);
string getNodeColor(int);
string fileName;
void setNodeColors(Graph&, int, int);


//color definitions
#define RED 1
#define BLUE 2
#define GREEN 3
#define ORANGE 4
#define PURPLE 5
#define YELLOW 6
#define BROWN 7
#define WHITE 8
#define BLACK 9
#define GRAY 10



struct CGenerator
//Generates every possibilty of color and nodes (c^n)
{
   public:
       CGenerator(int s, int v)
           : cSlots(s)
           , cValues(v)
       {
           a = new int[s];

           for (int i = 0; i < cSlots - 1; i++) {
               a[i] = 1;
           }
           a[cSlots - 1] = 0;

           nextInd = cSlots;
       }

       ~CGenerator()
       {
           delete a;
       }

       bool doNext()
       {
           for (;;)
           {
               if (a[nextInd - 1] == cValues) {
                   nextInd--;
                   if (nextInd == 0)
                       return false;
               }
               else {
                   a[nextInd - 1]++;
                   while (nextInd < cSlots) {
                       nextInd++;
                       a[nextInd - 1] = 1;
                   }

                   return true;
               }
           }
       }

       void doPrint()
       {
           printf("(");
           for (int i = 0; i < cSlots; i++) {
               cout << " " << a[i];
           }
           printf(")");
       }

       void storeValues()
       {
         vector<int> newComb;
         for (int i = 0; i < cSlots; i++) {
             newComb.push_back(a[i]);
         }
         colorCombs.push_back(newComb);
       }

   private:
       int *a;
       int cSlots;
       int cValues;
       int nextInd;
};

bool sortbysec(const pair<int,int> &a,
              const pair<int,int> &b)
{
    return (a.second < b.second);
}

struct VertexProperties
{
   pair<int,int> cell; // maze cell (x,y) value
   Graph::vertex_descriptor pred;
   bool visited;
   bool marked;
   int weight;
   int color;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
   int weight;
   bool visited;
   bool marked;
};

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.
{
   int n, e, c;
   int j,k;

   fin >> c;
   numColors = c;
   fin >> n >> e; // n = nodes, e = edges
   Graph::vertex_descriptor v;

   // Add nodes.
   for (int i = 0; i < n; i++)
      v = add_vertex(g);

   for (int i = 0; i < e; i++)
   {
      fin >> j >> k; //an edge is identified by its nodes (j, k)
      add_edge(j,k,g);  // Assumes vertex list is type vecS
   }
}

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{
   pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

   for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
   {
      g[*vItr].weight = w;
   }
}

int exhaustiveColoring(Graph &g, int numColors, int t)
//Sets all the node colors based on the number of colors specified and running
//time through an exhautive algorithm
{
  clock_t startTime;
  startTime = clock();
  int diff;

    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

    cout << "Generating all combinations..." <<endl;


    CGenerator m(num_vertices(g), numColors);
    while (m.doNext()) {
      m.storeValues();
    }

    int CurrentNodeColor;
    for(int i = 0; i < colorCombs.size(); ++i)
    {
      Graph::vertex_iterator vItr = vItrRange.first;
      for(int j = 0; j < colorCombs[0].size(); ++j )
      {
           g[*vItr].color = colorCombs[i][j];
           ++vItr;
       }
       vItr = vItrRange.first;

       for(int k = 0; k < colorCombs[0].size(); ++k )
       {
           boost::graph_traits <Graph>::adjacency_iterator ai, a_end;
           CurrentNodeColor = g[*vItr].color;
           for (tie(ai, a_end) = adjacent_vertices(*vItr, g);
            ai != a_end; ++ai)
           {
              if( CurrentNodeColor == g[*ai].color )
               {
                 numConflicts++;
                }
            }
            ++vItr;

            diff = clock() - startTime;
            if(diff > t ) {
              break;
            }
        }
        combConflicts.push_back( make_pair(i, numConflicts) );
        numConflicts = 0;

      }


      sort(combConflicts.begin(), combConflicts.end(), sortbysec);

      for( int s = 0; s < combConflicts.size(); s++) {
        if(combConflicts[s].second != combConflicts[0].second)
        {
          combConflicts.erase(combConflicts.begin() + s, combConflicts.end());
          break;
        }
      }

  cout << "Runtime: " << (float) diff / CLOCKS_PER_SEC << endl;
  return 0;

}

void printSolution() {
  ofstream output;
  fileName = "../outputs1b/" + fileName + ".output";
  output.open(fileName.c_str());

  int p = combConflicts[0].first;
  int n;
  output << "Combination: ";
  for( int j = 0; j < colorCombs[0].size(); j++)
  {
     output << getNodeColor(colorCombs[p][j]) << " ";
  }
  output << endl;
  output << "Number of Conflicts: " << combConflicts[0].second << endl;
  output.close();
}

int main()
{
   char x;
   ifstream fin;

   // Read the name of the graph from the keyboard or
   // hard code it here for testing.

    cout << "Enter filename" << endl;
    cin >> fileName;
    string fileToOpen = "../inputs1b/" + fileName + ".input";
    fin.open( fileToOpen.c_str() );
    if (!fin)
    {
       cerr << "Cannot open " << fileName << endl;
       exit(1);
    }

    cout << "Reading graph" << endl;
    Graph g;
    initializeGraph(g,fin);

    cout << "Num colors: " << numColors << endl;
    cout << "Num nodes: " << num_vertices(g) << endl;
    cout << "Num edges: " << num_edges(g) << endl;
    cout << endl;

    exhaustiveColoring(g, numColors, 10);
    printSolution();

}

string getNodeColor(int c) {
  switch (c)
  {
    case RED:
      return "RED";
    case BLUE:
      return "BLUE";
    case GREEN:
      return "GREEN";
    case ORANGE:
      return "ORANGE";
    case PURPLE:
      return "PURPLE";
    case YELLOW:
      return "YELLOW";
    case BROWN:
      return "BROWN";
    case WHITE:
      return "WHITE";
    default:
      throw("Error! Color index assigned to this node is outside bounds.");
  }
}
