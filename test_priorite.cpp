#include "priorite.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

const int n=20;

int main() {
    std::srand((unsigned int)std::time(0));
    std::vector<PointDist> v;
    FilePriorite fp;

    v.reserve(n);
    for(int i=0; i<n; i++)
        v.emplace_back(i, 0, (double) i);

    while(!v.empty()) {
        int index = rand() % v.size();
        fp.push(v[index]);
        v.erase(v.begin() + index);
    }

    while(!fp.empty()) {
        cout << fp.pop().dist << endl;
    }

    return 0;
}
