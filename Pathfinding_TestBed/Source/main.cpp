#include <stdarg.h>
#include <stdio.h>
#include <iostream>

#include "Tilemap.h"
#include "Pathfinder.h"

void OutputMessage(const char* message, ...)
{
#define MAX_MESSAGE 1024
    char szBuff[MAX_MESSAGE];
    va_list arg;
    va_start(arg, message);
    vsnprintf_s( szBuff, sizeof(szBuff), _TRUNCATE, message, arg );
    va_end(arg);

    szBuff[MAX_MESSAGE-1] = 0; // vsnprintf_s might do this, but docs are unclear
    std::cout << szBuff;
}

int main()
{
    // Comment out this srand for a predictable layout.
    //srand( (unsigned int)time(0) );

    Tilemap tilemap;
    Pathfinder pathfinder( &tilemap );

    // Draw the tilemap.
    tilemap.Draw();

    // Find a path from (3,7) to (3,7).
    int startx = 3;
    int starty = 8;
    int endx = 1;
    int endy = 7;
    bool found = pathfinder.FindPath( startx, starty, endx, endy );

    OutputMessage( "Found = %d\n", found );

    // If a path was found, display it.
    if( found )
    {
        std::vector<int> path = pathfinder.GetPath( endx, endy );

        OutputMessage( "Length = %d\n", path.size() );
        for( int i=0; i<path.size(); i++ )
        {
            OutputMessage( "%d\n", path[i] );
        }
    }

    OutputMessage( "done\n" );
}
