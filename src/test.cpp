
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


using namespace std;

vector< vector <int> > colorCombs;

struct CGenerator
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
               //printf("%d", a[i]);
           }
           printf(")");
       }

       void storeValues()
       {
         printf("(");
         vector<int> newComb;
         for (int i = 0; i < cSlots; i++) {
             newComb.push_back(a[i]);
             cout <<  a[i];
         }
         printf(")");
         colorCombs.push_back(newComb);
       }

   private:
       int *a;
       int cSlots;
       int cValues;
       int nextInd;
};


int main()
{

   CGenerator g(5, 3);

   while (g.doNext()) {
       //g.doPrint();
       g.storeValues();
   }

    for(int i = 0; i < colorCombs.size(); ++i)
    {
      for(int j = 0; j < colorCombs[0].size(); ++j )
      {
        cout << " " << colorCombs[i][j];
      }
      cout << endl;
    }

   return 0;
}
