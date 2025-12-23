#include <iostream>
#include <vector>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Node {
    int x;
    int y;
    bool is_obstacle;

    float g_cost;
    float h_cost;
    float f_cost;

    Node* parent;
    
    Node() {
        x = 0;
        y = 0;
        is_obstacle = false;
        g_cost = 1e9;
        h_cost = 1e9;
        f_cost = 1e9;
        parent = nullptr;
    }
};

vector <vector<int>> loadMap(const string& filename) {
    vector <vector<int>> grid;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return grid;
    }

    string line;
    int value;

    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);

        while (ss >> value) {
            row.push_back(value);
        }

        if (!row.empty()) {
            grid.push_back(row);
        }
    }

    file.close();
    return grid;
}

void printNode(const Node& n) {
    cout << "x = " << n.x << endl;
    cout << "y = " << n.y << endl;
    cout << "is_obstacle = " << n.is_obstacle << endl;  
    cout << "g_cost = " << n.g_cost << endl;
    cout << "h_cost = " << n.h_cost << endl;
    cout << "f_cost = " << n.f_cost << endl;
}

int main() {
    string mapFile = "binary_grid.txt";
    cout << "loading map from " << mapFile << "....." << endl;

    vector <vector<int>> warehouseMap = loadMap(mapFile);

    if (warehouseMap.empty()) {
        cerr << "Failed to load map or map is empty." << endl;
        return -1;
    }

    int rows = warehouseMap.size();
    int cols = warehouseMap[0].size();

    cout << "Map loaded successfully with dimensions: " << rows << "x" << cols << endl;

    // cout << "\n--- Map Preview ---" << endl;

    // for (int i = 0; i<rows; i++) {
    //     for (int j = 0; j<cols; j++) {
    //         cout << warehouseMap[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // return 0;


    vector <vector<Node>> nodes(rows, vector<Node>(cols));

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            nodes[y][x].x = x;
            nodes[y][x].y = y;
            nodes[y][x].is_obstacle = (warehouseMap[y][x] == 1); 
        }
    }

    cout << "Map converted to Node grid successfully." << endl; 

    // Optional: Print out the data in each node for verification
    // for (int y = 0; y < rows; y++) {
    //     for (int x = 0; x < cols; x++) {
    //         printNode(nodes[y][x]);
    //     }
    //     cout << endl;
    // }

    // Verification: Check a known wall (optional)
    // cout << "Node is obstacle " << nodes[0][0].is_obstacle << endl;

    return 0;
}


