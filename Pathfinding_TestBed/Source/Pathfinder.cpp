#include "Tilemap.h"
#include "Pathfinder.h"
#include <vector>

typedef PathNode::PathNodeStatus PathNodeStatus;

Pathfinder::Pathfinder(Tilemap* pTilemap)
{
    m_pTilemap = pTilemap;
    m_MapWidth = pTilemap->GetWidth();
    m_MapHeight = pTilemap->GetHeight();

    m_Nodes.resize( m_MapWidth * m_MapHeight );
}

Pathfinder::~Pathfinder()
{
}

bool Pathfinder::FindPath(int sx, int sy, int ex, int ey)
{
    //CATCH: unwalkable ending node/starting node
    if (!m_pTilemap->IsTileWalkableAtTilePos(ex, ey))
    {
        return false;
    }

    //Reset all nodes
    for (PathNode &_node : m_Nodes)
    {
        _node.cost = FLT_MAX;
        _node.heuristic = FLT_MAX;
        _node.finalCost = FLT_MAX;
        _node.parentNodeIndex = -1;
        _node.status = PathNodeStatus::Unchecked;
    }

    std::vector<int> OpenIndicies;

    //Get initial node
    float cost = 0;
    float heuristic = fabsf(ex - sx) + fabsf(ey - sy);
    float finalCost = heuristic + cost;
    int parent = Tilemap::GetIndex(sx, sy);
    //Create node
    PathNode FirstNode = PathNode{ parent, PathNode::PathNodeStatus::Open, cost, heuristic, finalCost };
    PathNode FinalNode = PathNode{ Tilemap::GetIndex(ex, ey), PathNodeStatus::Unchecked, heuristic, cost, finalCost };
    OpenIndicies.push_back(FirstNode.parentNodeIndex);
    m_Nodes[FirstNode.parentNodeIndex] = FirstNode;
    
    //LOOP
    while (!OpenIndicies.empty())
    {
        //Get open node
        int OpenNodeIndex = 0;
        //Iterate to find lowest F score
        for (int i = 0; i < OpenIndicies.size(); i++)
        {
            //Get current node
            PathNode& current_node = m_Nodes[OpenIndicies[i]];
            PathNode& open_node = m_Nodes[OpenNodeIndex];
            if (OpenNodeIndex == -1 || current_node.finalCost < open_node.finalCost)
            {
                //Set current node to open node
                OpenNodeIndex = OpenIndicies[i];
                OpenIndicies.erase(OpenIndicies.begin() + i);
                break;
            }
        }


        //If end point, return true!
        if (m_Nodes[OpenNodeIndex].parentNodeIndex == Tilemap::GetIndex(ex, ey))
        {
            return true;
        }

        //Mark closed
        m_Nodes[OpenNodeIndex].status = PathNodeStatus::Closed;

        //Add neighbours
        int node_x = OpenNodeIndex % 10;
        int node_y = OpenNodeIndex / 10;
        int neighbours[4] = { OpenNodeIndex + 1, OpenNodeIndex - 1, OpenNodeIndex + 10, OpenNodeIndex - 10 };
        //Loop neighbours
        for (int i = 0; i < 4; i++)
        {
            //Get coords
            int neighbour_x = neighbours[i] % 10;
            int neighbour_y = neighbours[i] / 10;
            //Is valid
            if (m_pTilemap->IsTileWalkableAtTilePos(neighbour_x, neighbour_y))
            {
                //Check if in list already
                bool add = true;
                for (int j = 0; j < OpenIndicies.size(); j++)
                {
                    if (OpenIndicies[j] == neighbours[i]) add = false; //Is in list, do not add
                }
                //Push
                if (add) OpenIndicies.push_back(neighbours[i]);
                //Calculate g
                float cost = m_Nodes[OpenNodeIndex].cost + 1.f;
                float heuristic = fabsf(ex - neighbour_x) + fabsf(ey - neighbour_y);
                float finalCost = cost + heuristic;
                m_Nodes[neighbours[i]].cost = cost;
                m_Nodes[neighbours[i]].heuristic = heuristic;
                m_Nodes[neighbours[i]].finalCost = finalCost;
                m_Nodes[neighbours[i]].parentNodeIndex = OpenNodeIndex;
            }
        }
    }


    return false;
}

std::vector<int> Pathfinder::GetPath(int ex, int ey)
{
    std::vector<int> path;

    return path;
}
