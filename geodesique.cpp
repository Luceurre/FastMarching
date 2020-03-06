// Author: 

#include "fastMarching.h"
#include <iostream>

typedef FVector<float,2> FloatPoint2; // point avec coordonnees float

const float EPSILON=10.0f; // Spatial length parameter
const float TAU=0.8f; // Increment dans la direction opposee au gradient
const size_t MAX_LENGTH=1000; // Max length of a geodesic

// Remplis l'image des poids, fonction croissante de la distance en couleur
// du point courant a I(p).

float norm(Color c1, Color c2) {
    return abs(c1.r() - c2.r())+ abs(c1.g() - c2.g()) + abs(c1.b() - c2.b());
}

Image<float> remplisPoids(const Image<Color>& I, const IntPoint2& p) {
    Image<float> W(I.width(), I.height());

    for(int x = 0; x < I.width(); ++x) {
        for(int y = 0; y < I.height(); ++y) {
            W(x, y) = EPSILON + norm(I(p), I(x, y))/3;
        }
    }

    for(int x = 0; x < I.width(); ++x) {
        W(x, 0) = INF;
        W(x, W.height() - 1) = INF;
    }


    for(int y = 0; y < I.height(); ++y) {
        W(0, y) = INF;
        W(W.width() - 1, y) = INF;
    }

    return W;
}

IntPoint2 arrondi(const FloatPoint2& p) {
    return IntPoint2((int)(p[0]+0.5f), (int)(p[1]+0.5f));
}

// Calcule la geodesique de p1 a p2.
vector<FloatPoint2> geodesique(const Image<float>& D,
                               FloatPoint2 p1, FloatPoint2 p2) {
    vector<FloatPoint2> v;
    v.push_back(p2);

    for(int i = 0; i < MAX_LENGTH && D(v.back().x(), v.back().y()) > 1; ++i) {
        auto grad = gradient(D, Coords<2>(v.back().x(), v.back().y()));
        grad.normalize();
        v.push_back(v.back() - TAU * grad);
    }

    if(v.size() <= MAX_LENGTH) {
        cout << "Géodésique calculée avec succès." << endl;
    } else {
        cout << "Géodésique n'a pas convergée. (en moins de N points)" << endl;
        cout << D(v.back().x(), v.back().y()) << endl;
    }

    v.push_back(p1);

    return v;
}

void drawGeo(const vector<FloatPoint2>& geo) {
    for(auto p : geo) {
        drawPoint(arrondi(p), RED);
    }
}

int main() {
    Image<Color> I;
    if(! load(I, argc>1? argv[1]: srcPath("map.png"))) {
        std::cout << "Echec de lecture d'image" << std::endl;
        return 1;
    }
    openWindow(I.width(), I.height());
    display(I);

    cout << "Cliquez les points de depart et d'arrivee" << endl;
    IntPoint2 p1, p2;

    cout << "Point de départ : " << endl;
    while(getMouse(p1)!=1);
    fillCircle(p1, 3, GREEN);
    cout << "Point d'arrivé : " << endl;
    while(getMouse(p2)!=1);
    fillCircle(p2, 3, WHITE);
    click();

    cout << "Calcul du coût par rapport au départ." << endl;
    auto W = remplisPoids(I, p1);
    affiche(W);
    click();

    cout << "Calcul distance par rapport au départ." << endl;
    vector<PointDist> v;
    v.emplace_back(p1.y(), p1.x(), 0.0f);
    auto D = fastMarching(W, v);
    affiche(D);
    click();

    cout << "Calcul de la géodésique entre le départ et l'arrivé" << endl;
    auto geo = geodesique(D, p1, p2);
    display(I);
    drawGeo(geo);
    fillCircle(p2, 3, WHITE);
    fillCircle(p1, 3, GREEN);
    click();

    endGraphics();
    return 0;
}
