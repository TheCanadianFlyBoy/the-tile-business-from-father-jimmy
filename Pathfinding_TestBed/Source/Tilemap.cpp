#include <stdlib.h>
#include "Tilemap.h"

void OutputMessage(const char* message, ...);

Tilemap::Tilemap()
{
    Create();
}

Tilemap::~Tilemap()
{
    delete m_Tiles;
}

void Tilemap::Create()
{
    m_Tiles = new unsigned char[10*10];

    for( int y=0; y<10; y++ )
    {
        for( int x=0; x<10; x++ )
        {
            int index = y*10 + x;

            if( rand()%10 == 0 )
                m_Tiles[index] = 1;
            else
                m_Tiles[index] = 0;
        }
    }
}

void Tilemap::Draw()
{
    OutputMessage( "\n" );
    OutputMessage( "Tilemap:\n" );
    OutputMessage( "\n" );

    for( int y=10-1; y>=0; y-- )
    {
        OutputMessage( "       " );
        for( int x=0; x<10; x++ )
        {
            OutputMessage( "%2d", m_Tiles[y*10 + x] );
        }

        OutputMessage( "\n" );
    }

    OutputMessage( "\n" );
}

bool Tilemap::IsTileWalkableAtTilePos(int x, int y)
{
    if( x < 0 || x >= 10 || y < 0 || y >= 10 )
        return false;

    return m_Tiles[y*10 + x] == 0 ? true : false;
}

int Tilemap::GetIndex(int x, int y)
{
    return y * 10 + x;
}
