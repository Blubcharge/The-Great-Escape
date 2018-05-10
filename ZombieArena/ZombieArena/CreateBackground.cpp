#include "stdafx.h"
#include "zombieArena.h"

int createBackground(VertexArray& rVA, IntRect arena)
{
	//any changes to rVA will be changed in background in the main function
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	//set perimitive type
	rVA.setPrimitiveType(Quads);

	//set size of vertex array
	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);
	int currentVertex = 0;

	//prepare vertex array
	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
			//position each vertex in current quad
			rVA[currentVertex + 0].position = Vector2f(w*TILE_SIZE, h*TILE_SIZE);
			rVA[currentVertex + 1].position = Vector2f(w*TILE_SIZE + TILE_SIZE, h*TILE_SIZE);
			rVA[currentVertex + 2].position = Vector2f(w*TILE_SIZE + TILE_SIZE, h*TILE_SIZE + TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f(w*TILE_SIZE, h*TILE_SIZE + TILE_SIZE);

			//define position in the texture for curret quAD
			//grass, stone, bush,wall
			if (h==0 || h==worldHeight-1|| w==0 || w == worldWidth-1)
			{
				//uise wall texture as we are at the edge
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else
			{
				//random floor texture
				srand((int)time(0) + h * w - h);
				int TileSelected = 2;
				int verticalOffeset = TileSelected *TILE_SIZE;

				//set random tile types
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffeset);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffeset);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffeset);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffeset);
			}


			//position for next vertex
			currentVertex = currentVertex + VERTS_IN_QUAD;

		}

	}



	return TILE_SIZE;

}

