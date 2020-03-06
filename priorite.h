#pragma once
#include <vector>
#include "pointDist.h"

class FilePriorite {
    std::vector<PointDist> v;
    int lazy_compare(int i);
    int maxi_index(int i, int j);
public:
    FilePriorite();

    void push(PointDist d);
    PointDist pop();
    bool empty();

    bool is_sorted();
};
