#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <unordered_map>

#include "Sector.h"
using namespace std;
class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    unordered_map<string, Sector*> umap;
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    Sector *insert(Sector *root, Sector *node);

    std::vector<Sector *> findPath(Sector *world, Sector *elara);

    Sector *deleteSectorHelper(Sector *root, const string &sector_code);

    Sector *findMin(Sector *node);

    Sector *deleteSectorHelper(Sector *root, Sector *node);

    Sector *deleteWholeTree(Sector *node);

    void destructorHelper(Sector *node);
};

#endif // SPACESECTORBST_H
