#pragma once 
#include <GL/glew.h>
#include <stb_image.h>
//We had help from the Udemy OpenGl course for the Texture class, we also 
// used stb_image (License: https://raw.githubusercontent.com/nothings/stb/49cbedfab6623f38a8fa65b32d10ab4b2fefd872/LICENSE).
class Texture {
public:
	Texture();
	Texture(const char* fileLoc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;

};
