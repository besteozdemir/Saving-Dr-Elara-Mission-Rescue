#include "SpaceSectorBST.h"

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

void SpaceSectorBST::destructorHelper(Sector *node) {
    if(node == nullptr) return;
    destructorHelper(node->left);
    destructorHelper(node->right);
    delete node;
}

SpaceSectorBST::~SpaceSectorBST() {
    destructorHelper(root);
    root = nullptr;
}

vector<string> readInputFile(const string &fileName) {
    vector<string> lines;
    ifstream inputFile(fileName);

    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();
    return lines;
}

Sector *SpaceSectorBST::insert(Sector *root, Sector *node) {
    if (root == nullptr) {
        return node;
    }
    if (node->x < root->x) {
        root->left = insert(root->left, node);
    } else if (node->x > root->x) {
        root->right = insert(root->right, node);
    } else if(node->x == root->x) {
        if(node->y < root->y) {
            root->left = insert(root->left, node);
        } else if(node->y > root->y) {
            root->right = insert(root->right, node);
        } else if(node->y == root->y) {
            if(node->z < root->z) {
                root->left = insert(root->left, node);
            } else if(node->z > root->z) {
                root->right = insert(root->right, node);
            }
        }
    }
    return root;
}
vector<Sector *> SpaceSectorBST::findPath(Sector *world, Sector *elara) {
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
            } else {
                path.clear();
            }
        } else {
            path.clear();
        }
    } else {
        path.clear();
    }

    return path;
}


Sector *SpaceSectorBST::deleteSectorHelper(Sector *curr, Sector *node) {
    if (curr == nullptr) {
        return curr;
    }

    if (node->x < curr->x) {
        curr->left = deleteSectorHelper(curr->left, node);
    } else if (node->x > curr->x) {
        curr->right = deleteSectorHelper(curr->right, node);
    } else if (node->y < curr->y) {
        curr->left = deleteSectorHelper(curr->left, node);
    } else if (node->y > curr->y) {
        curr->right = deleteSectorHelper(curr->right, node);
    } else if (node->z < curr->z) {
        curr->left = deleteSectorHelper(curr->left, node);
    } else if (node->z > curr->z) {
        curr->right = deleteSectorHelper(curr->right, node);
    } else {
        if (curr->left == nullptr) {
            Sector *temp = curr->right;
            delete curr;
            return temp;
        } else if (curr->right == nullptr) {
            Sector *temp = curr->left;
            delete curr;
            return temp;
        }

        Sector *temp = findMin(curr->right);

        curr->x = temp->x;
        curr->y = temp->y;
        curr->z = temp->z;
        curr->sector_code = temp->sector_code;
        curr->distance_from_earth = temp->distance_from_earth;
        umap[temp->sector_code] = curr;

        curr->right = deleteSectorHelper(curr->right, temp);
    }

    return curr;
}


Sector* SpaceSectorBST::findMin(Sector* node){
    while(node->left != nullptr) node = node->left;
    return node;
}

void SpaceSectorBST::readSectorsFromFile(const std::string &filename) {
    vector<string> lines = readInputFile(filename);
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

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    auto *pSector = new Sector(x, y, z);

    if (root == nullptr) {
        root = pSector;
        umap[pSector->sector_code] = pSector;
    } else {
        insert(root, pSector);
        umap[pSector->sector_code] = pSector;
    }
}

void SpaceSectorBST::deleteSector(const std::string &sector_code) {
    Sector *node = umap[sector_code];
    if(node != nullptr) {
        root = deleteSectorHelper(root , node);
    }
}

void SpaceSectorBST::displaySectorsInOrder() {
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

        cout << curr->sector_code << endl;
        curr = curr->right;
    }
    cout<< endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    cout << "Space sectors preorder traversal:" << endl;
    if (root == nullptr)
        return;

    stack<Sector*> st;
    Sector* curr = root;

    while (!st.empty() || curr != nullptr) {
        while (curr != nullptr) {
            cout << curr->sector_code << endl;

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

void SpaceSectorBST::displaySectorsPostOrder() {
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
        cout << node->sector_code << endl;
    }
    cout<< endl;
}

std::vector<Sector *> SpaceSectorBST::getStellarPath(const std::string &sector_code) {
    std::vector<Sector *> path;
    Sector* world = umap["0SSS"];
    Sector* elara = umap[sector_code];

    path = findPath(world , elara);

    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector *> &path) {
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