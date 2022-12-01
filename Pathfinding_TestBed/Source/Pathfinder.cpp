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
    for (int i = 0; i < m_Nodes.size(); i++)
    {
        PathNode& _node = m_Nodes[i];
        _node.cost = FLT_MAX;
        _node.heuristic = FLT_MAX;
        _node.finalCost = FLT_MAX;
        _node.parentNodeIndex = i;
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
        int DeletionIndex = 0;
        //Iterate to find lowest F score
        for (int i = 0; i < OpenIndicies.size(); i++)
        {
            //Get current node
            PathNode& current_node = m_Nodes[OpenIndicies[i]];
            PathNode& open_node = m_Nodes[OpenNodeIndex];
            if (current_node.finalCost <= open_node.finalCost)
            {
                //Set current node to open node
                OpenNodeIndex = OpenIndicies[i];
                DeletionIndex = i;
            }
        }
        //Delete        
        OpenIndicies.erase(OpenIndicies.begin() + DeletionIndex);

        //If end point, return true!
        if (OpenNodeIndex == Tilemap::GetIndex(ex, ey))
        {
            return true;
        }

        //Mark closed
        m_Nodes[OpenNodeIndex].status = PathNodeStatus::Closed;

        //Add neighbours
        int node_x = OpenNodeIndex % m_pTilemap->GetWidth();
        int node_y = OpenNodeIndex / m_pTilemap->GetWidth();
        int neighbours[4] = {   m_pTilemap->GetTileWalkableAtTilePos(node_x + 1, node_y),
                                m_pTilemap->GetTileWalkableAtTilePos(node_x - 1, node_y),
                                m_pTilemap->GetTileWalkableAtTilePos(node_x, node_y + 1),
                                m_pTilemap->GetTileWalkableAtTilePos(node_x, node_y - 1) };
        //Loop neighbours
        for (int i = 0; i < 4; i++)
        {
            //Is valid
            if (neighbours[i] != -1 && m_Nodes[neighbours[i]].status != PathNodeStatus::Closed)
            {
                //Get coords
                int neighbour_x = neighbours[i] % m_pTilemap->GetWidth();
                int neighbour_y = neighbours[i] / m_pTilemap->GetWidth();
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
    //Create vector of indicies
    std::vector<int> path;

    //Get end node index and add to path
    int current_node_index = ex + ey * m_pTilemap->GetWidth();
    path.push_back(current_node_index);
    //Iterate the parents
    while (m_Nodes[current_node_index].parentNodeIndex != current_node_index)
    {
        PathNode& current_node = m_Nodes[current_node_index];
        int parnt = current_node.parentNodeIndex;
        //Add to path
        path.push_back(parnt);
        //Rejazzle the shmazzle
        current_node_index = parnt;
    }


    return path;
}
