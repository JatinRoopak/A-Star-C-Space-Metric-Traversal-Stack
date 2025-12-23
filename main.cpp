#include <iostream>
#include <vector>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

struct Node {
    int x;
    int y;
    bool is_obstacle;
    bool is_visited;
    bool is_closed;

    float g_cost;
    float h_cost;
    float f_cost;

    Node* parent;
    
    Node() {
        x = 0;
        y = 0;
        is_obstacle = false;
        is_visited = false;
        is_closed = false;
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

float calculateHeuristic(int x1, int y1, int x2, int y2) {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    };

bool robotSize(const vector<vector<Node>>& nodes, int cx, int cy, int rows, int cols) {
    int radius = 1; // assuming our robot is 3x3 matrix big in grid

    for (int y = cy-radius; y <= cy+radius; y++){ 
        for (int x = cx-radius; x<= cx+radius; x++){
            if(x<0 || x>= cols || y<0 || y>= rows){
                return true;
            }
            if (nodes[y][x].is_obstacle) {
                return true;
            }
        }
    }
    return false;
}


bool solveAStar(vector <vector<Node>>& nodes, Node* start, Node* goal, int rows, int cols) {
    
    vector<Node*> nodeList;

    start -> g_cost = 0;
    start -> h_cost = calculateHeuristic(start -> x, start -> y, goal -> x, goal -> y);
    start -> f_cost = start -> g_cost + start -> h_cost;
    nodeList.push_back(start);

    while (!nodeList.empty()) {
        Node* current = nodeList[0];
        int currentIndex = 0;

        for (size_t i = 1; i<nodeList.size(); i++){
            if (nodeList[i] -> f_cost < current -> f_cost){
                current = nodeList[i];
                currentIndex = i;
            }
        }

        // Remove current node from Open List which is processing now
        nodeList.erase(nodeList.begin() + currentIndex);
        current -> is_closed = true;

        if (current -> x == goal->x && current -> y== goal->y){
            cout << "Goal Reached" << endl;
            return true;
        }

        // Moving to neighbours
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for(int i = 0; i<4; i++) {
            int newX = current-> x + dx[i];
            int newY = current-> y + dy[i];

            if (newX < 0 || newX >= cols || newY < 0 || newY >= rows) continue;

            if (nodes[newY][newX].is_closed) continue; // Not hit the wall

            if (robotSize(nodes, newX, newY, rows, cols)) continue;; // check the fatness of robot so that it can fit 

            float newGCost = current -> g_cost + 1;

            Node* neighbour = &nodes[newY][newX];

            if (newGCost < neighbour -> g_cost) {
                neighbour->g_cost = newGCost;
                neighbour->h_cost = calculateHeuristic(newX, newY, goal->x, goal-> y);
                neighbour->f_cost = neighbour->g_cost+neighbour->h_cost;
                neighbour->parent = current;

                bool innodeList = false;
                for (Node* n: nodeList) {
                    if (n->x == newX && n->y == newY) {
                        innodeList = true;
                        break;
                    }
                }

                if (!innodeList){
                    nodeList.push_back(neighbour);
                }
            }
        }
    }   
    return false;
}


void savePath(Node* goal,const vector<vector<int>>& rawMap ,string filename) {
    ofstream file(filename);
    if (!file.is_open()){
        cout << "Could not open"<<endl;
        return;
    }

    int rows = rawMap.size();
    int cols = rawMap[0].size();

    vector<string> canvas(rows, string(cols, ' '));

    for (int y = 0; y< rows; y++){
        for (int x =0; x< cols; x++){
            if (rawMap[y][x] == 1){
                canvas[y][x] = '#';
            } else{
                canvas[y][x] = ' ';
            }
        }
    }

    Node* current = goal;
    while (current != nullptr){
        canvas[current->y][current->x] = '*';

        current = current->parent;
    }

    for (int i = 0; i< rows; i++){
        file << canvas[i] << endl;
    }

    file.close();
    cout << "output text is ready as: " << filename << endl;
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

    // cout << "--- Map Preview ---" << endl;
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

    Node* startNode = &nodes[9][12];
    Node* goalNode = &nodes[79][32];

    cout << "starting A*...." << endl;
    cout << "Start " << startNode->x << "," << startNode->y << endl;
    cout << "Goal " << goalNode->x << "," << goalNode->y << endl;

    if (solveAStar(nodes, startNode, goalNode, rows, cols)){
        savePath(goalNode, warehouseMap, "visual_grid.txt");
    } else{
        cout << "No path Found" << endl;
    }

    return 0;
    
}


