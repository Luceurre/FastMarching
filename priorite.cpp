#include <iostream>
#include "priorite.h"

// Ajoute un element
void FilePriorite::push(PointDist d) {
    int element_index = v.size();
    v.push_back(d);
    while(element_index != 1 && v[(int)element_index / 2] < v[element_index]) {
        std::swap(v[element_index], v[(int)element_index / 2]);
        element_index /= 2;
    }
}

// Retire un element
PointDist FilePriorite::pop() {
    if(this->empty()) {
        throw std::out_of_range("La file de priorité est vide.");
    }

    std::swap(v[1], v[v.size() - 1]); // On échange le premier et le dernier élément

    PointDist prio =  v.back();
    v.pop_back();

    if(this->empty()) // Pour éviter un problème comme on retire l'élément.
        return prio;

    int element_index = 1;
    int swap_with = lazy_compare(element_index);

    while(swap_with != element_index) {
        std::swap(v[swap_with], v[element_index]);
        element_index = swap_with;
        swap_with = lazy_compare(element_index);
    }

    return prio;
}

FilePriorite::FilePriorite() {
    // Le zéro ignoré.
    v.emplace_back(0, 0, 0.);
}

bool FilePriorite::empty() {
    return v.size() <= 1;
}

bool FilePriorite::is_sorted() {
    for(int i = 1; 2 * i < v.size(); ++i) {
        if(v[i] < v[2*i]) {
            return false;
        }
        if(2 * i + 1 < v.size()) {
            if(v[i] < v[2*i + 1]) {
                return false;
            }
        }
    }
    return true;
}

int FilePriorite::lazy_compare(int i) {
    maxi_index(2 * i + 1, maxi_index(2 * i, i));
}

int FilePriorite::maxi_index(int i, int j) {
    if(i >= v.size() && j >= v.size()) {
        throw std::out_of_range("Comparaison impossible");
    }
    if(i >= v.size()) {
        return j;
    }
    if(j >= v.size()) {
        return i;
    }
    if(v[i] < v[j]) {
        return j;
    }

    return i;
}
