#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <stack>
#include <algorithm>

using namespace std;


class SpaceSectorLLRBT {
public:
    Sector* root;
    std::unordered_map <std::string,Sector*> umap;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    Sector *insertion(Sector *root, Sector *node);

    vector<Sector *> findPath2(Sector *world, Sector *elara);
    vector<Sector *> findPath(Sector *world, Sector *elara);

    Sector* rotateLeft(Sector *h);
    Sector* rotateRight(Sector *h);

    vector<Sector *> findPath2(Sector *world);

    void destructorHelper2(Sector *node);
};

#endif // SPACESECTORLLRBT_H
