#pragma once
#include <Imagine/Images.h>
#include <vector>
#include "pointDist.h"
using namespace Imagine;
using namespace std;

const float INF = INFINITY; // Infini en float

Image<float> fastMarching(const Image<float>& W, const vector<PointDist>& niv0);
void affiche(const Image<float>& D);
