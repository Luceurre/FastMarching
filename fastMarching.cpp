#define _USE_MATH_DEFINES
#include <cmath>
#include "fastMarching.h"
#include "priorite.h"
#include <algorithm>

// chaque voisin contient le tuple (i, j)
const int voisin[4][2] = { {-1,0}, {+1,0}, {0,-1}, {0,+1} };

bool isingrid(int i, int j, int w, int h) {
    return (0 <= i && i < h && 0 <= j && j < w);
}


// Met dans dx et dy les plus petites valeurs des voisins du pixel (x,y).
void minVoisins(const Image<float>& D, int i, int j, float& d1, float& d2) {
    d1 = INF;
    d2 = INF;

    for(auto d : voisin) {
        int I = i + d[0];
        int J = j + d[1];
        if(isingrid(I, J, D.width(), D.height())) {
            if(d[0] != 0) {
                d2 = (D(J, I) < d2) ? D(J, I) : d2;
            }
            else{
                d1 = (D(J, I) < d1) ? D(J, I) : d1;
            }
        }
    }
}

// Met a jour et renvoie la distance D(x,y) en fonction des voisins.
float calcDistance(Image<float>& D, const Image<float>& W, int x, int y) {
    float d1, d2;

    minVoisins(D, y, x, d1, d2);

    if(d1 == INF || d2 == INF) {
        return min(d1, d2) + W(x, y);
    } else {
        float delta = 2 * pow(W(x, y), 2) - pow(d2 - d1, 2);
        if (delta < 0) {
            return min(d1, d2) + W(x, y);
        } else {
            return 0.5 * (d1 + d2 + pow(delta, 0.5));
        }
    }
}

// Fast Marching: carte de distance a partir des points de niv0, qui sont a
// distance 0 par definition.
Image<float> fastMarching(const Image<float>& W, const vector<PointDist>& niv0){
    const int w=W.width(), h=W.height();

    // Initialisation
    Image<float> D(w,h); D.fill(INF);
    Image<bool>  E(w,h); E.fill(false);
    FilePriorite F;

    for(auto point : niv0) {
        F.push(point);
        D(point.j, point.i) = 0;
        E(point.j, point.i) = true;
    }

    while(!F.empty()) {
        PointDist p = F.pop();
        for(auto d : voisin) {
            int I = d[0] + p.i;
            int J = d[1] + p.j;
            if(isingrid(I, J, w, h)) {
                if(!E(J, I)) {
                    D(J, I) = calcDistance(D, W, J, I);
                    E(J, I) = true;
                    F.push(PointDist(I, J, -D(J, I)));
                }
            }
        }
    }

    return D;
}

// Affiche en couleur l'image des distances.
// bleu=0, rouge=maximum, vert=infini
void affiche(const Image<float>& I) {
    Image<float> D=I.clone();
    for(int i=0; i<D.height(); i++)
        for(int j=0; j<D.width(); j++)
            if(D(j,i)==INF)
                D(j,i)=-1.0f;
    float M = *max_element(D.begin(), D.end());
    M = max(1.0f,M);
    Image<Color> B(D.width(), D.height());
    for(int i=0; i<D.height(); i++)
        for(int j=0; j<D.width(); j++) {
            if(D(j,i)>=0) {
                float angle=M_PI/2*D(j,i)/M;
                B(j,i) = Color(255*sin(angle), 0, 255*cos(angle));
            }
            else B(j,i) = GREEN;
        }
    display(B);
}
