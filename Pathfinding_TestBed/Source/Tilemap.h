#pragma once

class Tilemap
{
public:
    unsigned char* m_Tiles;

public:
    Tilemap();
    virtual ~Tilemap();

    void Create();
    void Draw();

    bool IsTileWalkableAtTilePos(int x, int y);
    static int GetIndex(int x, int y);

    int GetWidth() { return 10; }
    int GetHeight() { return 10; }
};
