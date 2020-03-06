// Author: 

#include "fastMarching.h"
#include <iostream>

const int w=512, h=512;
const float TOL=0.1f; // Seuil de tolerance d'imprecision numerique

// Selection a la souris d'un rectangle.
bool selectionRect(int& x1, int& y1, int& x2, int& y2, const Image<Color>& I) {
    Event e;
    do {
        getEvent(0, e);
        if(e.type==EVT_BUT_ON) {
            x1 = x2 = e.pix[0];
            y1 = y2 = e.pix[1];
            if(e.button==3)
                return false;
        }
        if(e.type==EVT_MOTION) {
            x2 = e.pix[0];
            y2 = e.pix[1];
            display(I);
            fillRect(std::min(x1,x2), std::min(y1,y2), abs(x1-x2), abs(y1-y2),
                     RED);
        }
    } while(e.type!=EVT_BUT_OFF || abs(x1-x2)<5 || abs(y1-y2)<5);
    if(x1>x2) std::swap(x1,x2);
    if(y1>y2) std::swap(y1,y2);
    return true;
}

bool selectionSource(int& x, int& y, const Image<Color>& I) {
    int key;
    do {
        key = getMouse(x, y);
    } while(key != 1);

    return I(x, y) != RED;
}

void calculOmbre(const Image<float>& Davec, const Image<float>& Dsans, Image<Color>& I) {
    for(int x = 0; x < w; ++x) {
        for(int y = 0; y < h; ++y) {
            if(I(x, y) != RED) {
                if(Davec(x, y) > Dsans(x, y) + TOL) {
                    I(x, y) = BLUE;
                }
            }
        }
    }
}

int main() {
    int x1, x2, y1, y2;

    openWindow(w, h);
    Image<Color> I(w,h);
    I.fill(WHITE);

    cout << "Dessinez des rectangles, clic droit pour terminer" << endl;
    while(selectionRect(x1, y1, x2, y2, I)) {
        int x = std::min(x1, x2);
        int y = std::min(y1, y2);
        for(int dx = 0; dx < abs(x1 - x2); ++dx) {
            for(int dy = 0; dy < abs(y1 - y2); ++dy) {
                I(x + dx, y + dy) = RED;
            }
        }
    }

    cout << "Cliquez la position de la source lumineuse" << endl;
    vector<PointDist> v;
    while(!selectionSource(x1, y1, I));
    v.emplace_back(y1, x1, 0.0f);

    // Remplissage des cartes de cout avec et sans obstacles
    Image<float> Wavec(w,h), Wsans(w,h);
    Wavec.fill(1.0f); Wsans.fill(1.0f);

    for(int x = 0; x < w; ++x) {
        for(int y = 0; y < h; ++y) {
            Wavec(x, y) = (I(x, y) == RED) ? INF : 1.0f;
        }
    }

    cout << "Calcul" << endl;
    Image<float> Davec = fastMarching(Wavec, v);
    Image<float> Dsans = fastMarching(Wsans, v);

    calculOmbre(Davec, Dsans, I);
    display(I);
    fillCircle(x1, y1, 3, GREEN);

    Window davec_w = openWindow(w, h, "Davec");
    setActiveWindow(davec_w);
    affiche(Davec);

    Window dsans_w = openWindow(w, h, "Ssans");
    setActiveWindow(dsans_w);
    affiche(Dsans);

    cout << "Click on Dsans window to terminate." << endl;

    click();

    endGraphics();
    return 0;
}
