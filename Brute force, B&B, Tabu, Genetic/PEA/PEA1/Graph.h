#include <string>

class Graph {

private:

    int** matrix = nullptr;
    int vert_count = 0;
    std::string description = "";

public:

    std::string getDescription();
    std::string toString();
    bool readGraph();
    bool readGraphSmall();
    int getCount();
    int** getMatrix();
    Graph();
    ~Graph();
};
