#include "stdafx.h"
#include "TextureHolder.h"

//include assert feature
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder()
{
	assert( m_s_Instance == nullptr );
	m_s_Instance = this;


}

Texture & TextureHolder::GetTexture(string const & filename)
{
	// get a referance to m_Textures using m_s_Instance
	auto& m = m_s_Instance->m_Textures;

	//auto is the equivalent of map<string,Texture>

	//create an iterator to hold a key-value-pair (kvp) and search for the kvp using passed in filename

	auto keyValuePair = m.find(filename);
	//auto is equivalent of map<string,texture>::iterator

	//did we find a match
	if (keyValuePair != m.end())
	{
		//found kvp, return texture which is second of part of kvp
		return keyValuePair->second;
	}
	else
	{
		//filename not found
		//crate a new kvp using filename
		auto& texture = m[filename];
		//load texture from filename
		texture.loadFromFile(filename);

		return texture;

	}

}
