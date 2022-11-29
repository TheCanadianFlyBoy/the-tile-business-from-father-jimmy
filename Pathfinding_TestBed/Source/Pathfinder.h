#pragma once

#include <vector>

struct PathNode
{
    enum class PathNodeStatus
    {
        Unchecked,
        Open,
        Closed,
    };

    int parentNodeIndex;
    PathNodeStatus status;

    float cost;      // Cost to get to this node.
    float heuristic; // Heuristic: manhatten distance to destination.
    float finalCost; // Sum of the 2 values above.
};

class Pathfinder
{
protected:
    Tilemap* m_pTilemap;
    int m_MapWidth;
    int m_MapHeight;

    std::vector<PathNode> m_Nodes;

public:
    Pathfinder(Tilemap* pTilemap);
    virtual ~Pathfinder();

    // Start a search, supply the starting tile x,y and ending tile x,y.
    // Returns true if path found, false otherwise.
    bool FindPath(int sx, int sy, int ex, int ey);

    // Retrieve the final path.
    std::vector<int> GetPath(int ex, int ey);
};
