// Author: 

#include "fastMarching.h"
#include <iostream>
#include <fenv.h>


const int w=256, h=256;

int main() {
    feenableexcept(FE_INVALID | FE_OVERFLOW);
    openWindow(w,h);
    Image<float> W(w,h);
    W.fill(1.0f);
    vector<PointDist> v;
    cout << "Cliquez des points origine, clic droit pour terminer" << endl;
    PointDist p(0,0,0);
    while(getMouse(p.j,p.i)==1) {
        v.push_back(p);
        fillCircle(p.j,p.i,2,BLUE);
    }
    Image<float> D = fastMarching(W,v);

    float t[w * h];

    for(int i = 0; i < w; ++i) {
        for(int j = 0; j < h; ++j) {
            t[i + w*j] = D(i, j);
        }
    }

    affiche(D);
    endGraphics();
    return 0;
}
