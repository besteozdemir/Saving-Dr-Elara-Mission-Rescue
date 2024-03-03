#include "SpaceSectorLLRBT.h"

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

vector<string> readInput(const string &fileName) {
    vector<string> lines;
    ifstream inputFile(fileName);

    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();
    return lines;
}

bool isRed(Sector *node) {
    if(node == nullptr) return false;
    return node->color;
}

Sector * SpaceSectorLLRBT::rotateLeft(Sector *h) {
    Sector *x = h->right;
    h->right = x->left;

    if (x->left != nullptr) {
        x->left->parent = h;
    }

    x->left = h;
    x->color = h->color;
    h->color = RED;

    x->parent = h->parent;
    h->parent = x;

    return x;
}

Sector * SpaceSectorLLRBT::rotateRight(Sector *h) {
    Sector *x = h->left;
    h->left = x->right;

    if (x->right != nullptr) {
        x->right->parent = h;
    }

    x->right = h;
    x->color = h->color;
    h->color = RED;

    x->parent = h->parent;
    h->parent = x;

    return x;
}

void flipColors(Sector* node) {
    node->color = RED;
    node->left->color = BLACK;
    node->right->color = BLACK;

    if (node->left != nullptr) {
        node->left->parent = node;
    }

    if (node->right != nullptr) {
        node->right->parent = node;
    }
}


Sector * SpaceSectorLLRBT::insertion(Sector *curr , Sector *node) {
    if(curr == nullptr) {
        node->color = RED;
        return node;
    }
    node->parent = curr;

    if (node->x < curr->x) {
        curr->left = insertion(curr->left, node);
    } else if (node->x > curr->x) {
        curr->right = insertion(curr->right, node);
    } else if(node->x == curr->x) {
        if(node->y < curr->y) {
            curr->left = insertion(curr->left, node);
        } else if(node->y > curr->y) {
            curr->right = insertion(curr->right, node);
        } else if(node->y == curr->y) {
            if(node->z < curr->z) {
                curr->left = insertion(curr->left, node);
            } else if(node->z > curr->z) {
                curr->right = insertion(curr->right, node);
            }
        }
    }

    if (isRed(curr->right) && !isRed(curr->left)) {
        curr = rotateLeft(curr);
    }
    if (isRed(curr->left) && isRed(curr->left->left)) {
        curr = rotateRight(curr);
    }
    if (isRed(curr->left) && isRed(curr->right)) {
        flipColors(curr);
    }

    return curr;

}

vector<Sector *> SpaceSectorLLRBT::findPath(Sector *world, Sector *elara) {
    vector<Sector *> path;

    if (world == nullptr || elara == nullptr) {
        return path;
    }

    path.push_back(world);

    if (elara->sector_code == world->sector_code) {
        return path;
    } else if (elara->x < world->x) {
        auto leftPath = findPath(world->left, elara);
        path.insert(path.end(), leftPath.begin(), leftPath.end());
    } else if (elara->x > world->x) {
        auto rightPath = findPath(world->right, elara);
        path.insert(path.end(), rightPath.begin(), rightPath.end());
    } else if (elara->x == world->x) {
        if (elara->y < world->y) {
            auto leftPath = findPath(world->left, elara);
            path.insert(path.end(), leftPath.begin(), leftPath.end());
        } else if (elara->y > world->y) {
            auto rightPath = findPath(world->right, elara);
            path.insert(path.end(), rightPath.begin(), rightPath.end());
        } else if (elara->y == world->y) {
            if (elara->z < world->z) {
                auto leftPath = findPath(world->left, elara);
                path.insert(path.end(), leftPath.begin(), leftPath.end());
            } else if (elara->z > world->z) {
                auto rightPath = findPath(world->right, elara);
                path.insert(path.end(), rightPath.begin(), rightPath.end());
            }
        }
    }

    return path;
}

std::vector<Sector *> SpaceSectorLLRBT::findPath2(Sector *world , Sector *elara) {
    std::vector<Sector*> path;

    // Traverse up from elara until reaching the root
    while (elara != nullptr) {
        path.insert(path.begin(), elara);

        if (elara == world) {
            // Found the target node, stop traversing
            return path;
        }
        elara = elara->parent;
    }
    return path;

}

std::vector<Sector*> findCommonPrefix(vector<Sector*> vec1, vector<Sector*> vec2) {
    std::vector<Sector*> commonPrefix;
    for (int i = 0; i < vec1.size(); ++i) {
        for (int j = 0; j < vec2.size(); ++j) {
            if(vec1[i]->sector_code == vec2[j]->sector_code) {
                for (int k = 0; k < i; ++k) {
                    commonPrefix.push_back(vec1[k]);
                }
                for (int l = j; l >= 0; --l) {
                    commonPrefix.push_back(vec2[l]);
                }
                return commonPrefix;
            }
        }
    }

    return commonPrefix;
}
void SpaceSectorLLRBT::destructorHelper2(Sector *node) {
    if(node == nullptr) return;
    destructorHelper2(node->left);
    destructorHelper2(node->right);
    delete node;

}


void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
    vector<string> lines = readInput(filename);
    queue<int> values;
    for (int i = 1; i < lines.size(); ++i) {
        istringstream ss(lines[i]);
        string token;

        while (getline(ss, token, ',')) {
            int value = std::stoi(token);
            values.push(value);
        }
    }
    while (!values.empty()) {
        int x = values.front();
        values.pop();
        int y = values.front();
        values.pop();
        int z = values.front();
        values.pop();
        insertSectorByCoordinates(x, y, z);
    }
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    destructorHelper2(root);
    root = nullptr;
    // TODO: Free any dynamically allocated memory in this class.
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
    auto *pSector = new Sector(x, y, z);



    if (root == nullptr) {
        root = pSector;
        umap[pSector->sector_code] = pSector;
    } else {
        root = insertion(root, pSector);
        umap[pSector->sector_code] = pSector;
        root->color = BLACK;
    }


}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    stack<Sector *> s;
    Sector *curr = root;

    while (curr != nullptr || !s.empty()) {

        while (curr != nullptr) {
            s.push(curr);
            curr = curr->left;
        }

        curr = s.top();
        s.pop();

        string col;
        if(curr->color) {
            col = "RED";
        } else {
            col = "BLACK";
        }
        cout << col << " sector: " << curr->sector_code << endl;
        curr = curr->right;
    }
    cout<< endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    if (root == nullptr)
        return;

    stack<Sector*> st;
    Sector* curr = root;

    while (!st.empty() || curr != nullptr) {
        while (curr != nullptr) {
            string col;
            if(curr->color) {
                col = "RED";
            } else {
                col = "BLACK";
            }
            cout<< col << " sector: "  << curr->sector_code << endl;

            if (curr->right)
                st.push(curr->right);

            curr = curr->left;
        }
        if (!st.empty()) {
            curr = st.top();
            st.pop();
        }
    }
    cout<< endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    if (root == nullptr)
        return;

    stack<Sector *> s1, s2;

    s1.push(root);
    Sector *node;

    while (!s1.empty()) {
        node = s1.top();
        s1.pop();
        s2.push(node);

        if (node->left)
            s1.push(node->left);
        if (node->right)
            s1.push(node->right);
    }

    while (!s2.empty()) {
        node = s2.top();
        s2.pop();
        string col;
        if(node->color) {
             col = "RED";
        } else {
            col = "BLACK";
        }
        cout<< col << " sector: "  << node->sector_code << endl;
    }
    cout<< endl;
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    vector<Sector *> worldToRoot;
    vector<Sector *> elaraToRoot;
    Sector* world = umap["0SSS"];
    Sector* elara = umap[sector_code];
    //path = findPath2(world , elara);

    while (world != nullptr) {
        worldToRoot.push_back(world);
        world = world->parent;
    }
    while (elara != nullptr) {
        elaraToRoot.push_back(elara);
        elara = elara->parent;
    }
    path = findCommonPrefix(worldToRoot, elaraToRoot);



    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
    if(path.empty()) {
        cout << "A path to Dr. Elara could not be found." << endl;
    } else {
        cout<< "The stellar path to Dr. Elara: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i]->sector_code;

            if (i < path.size() - 1) {
                cout << "->";
            }
        }
        cout << endl;
    }
}