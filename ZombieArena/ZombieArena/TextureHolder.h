#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include<SFML\Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class TextureHolder
{
private:
	//map container from the STL
	std::map<std::string, Texture>m_Textures;
	//a pointer of same type of calss
	static TextureHolder* m_s_Instance;
	
public:
	TextureHolder();
	static Texture& GetTexture(string const& filename);


};

#endif