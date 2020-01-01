/*
*   Testing all of the sets in the set hierarchy defined in sets.h file
*/


#include <iostream>
#include "sets.h"

using namespace std;

typedef enum{mon, tue, wed, thu, fri} weekday;

int main() {
/* TESTING */
    carray_simple_set<weekday>* V = new carray_simple_set<weekday>(tue, (weekday)5);
    *V += (wed);
    *V += (tue);
    *V -= (tue);
    *V -= (wed);
    if (V->contains(wed)){ //Should not be
        cout << "wed is in V\n";
    }else{
        cout << "wed is not in V\n";
    }


    hashed_simple_set<int, cast_to_int<int>>* H = new hashed_simple_set<int, cast_to_int<int>>(100);
    *H += 4;
    *H += 12;
    *H -= 12;

    if (H->contains(12)){ //Should not
        cout << "12 is in H\n";
    }else{
        cout << "12 is not in H\n";
    }

    carray_range_set<int>* X = new carray_range_set<int>(3, 20);
    *X += 6;
    *X += (3);
    *X += (4);
    *X -= (6);
    *X += range<int>(5,true,8,false);

    if (X->contains(6)){ //Should
        cout << "6 is in V\n";
    }else{
        cout << "6 is not in V\n";
    }

    hashed_range_set<int>* U = new hashed_range_set<int>(20);
    *U += 6;
    *U += 7;
    *U += 8;
    *U -= 8;
    *U += range<int>(4,true,18,true);
    *U -= range<int>(7,true,19,false);

    if (U->contains(8)){ //Should not
        cout << "8 is in U \n";
    }else{
        cout << "8 is not in U \n";
    }


    bin_search_range_set<int>* K = new bin_search_range_set<int>(30);
    *K += 9;
    *K -= 9;
    *K += 7;
    *K += 4;
    *K += 5;
    *K += 8;
    *K += 10;
    *K += 9;
    *K += 2;
    *K -= 2;
    if (K->contains(9)){ //Should
        cout << "9 is in K \n";
    }else{
        cout << "9 is not in K \n";
    }
}