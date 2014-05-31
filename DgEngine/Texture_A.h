//================================================================================
// @ Texture_A.h
// 
// Class: Texture_A
//
// Texture class for animated images. An aminated texture is essentially a
// list of frames (an image and a display time).
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef TEXTURE_A_H
#define TEXTURE_A_H

#include "Texture.h"
#include "Mipmap.h"
#include "DgTypes.h"
#include <vector>

class Mipmap;
class ImageManager;
namespace pugi{ class xml_node; }


//--------------------------------------------------------------------------------
//	@	Frame
//--------------------------------------------------------------------------------
namespace impl
{
	struct Frame
	{
		//Constructor
		Frame() : mipmapID(0) {}

		//Destructor
		~Frame() {}

		//Copy operations
		Frame(const Frame& f) : tme(f.tme), mipmapID(f.mipmapID) {}

		//Input
		friend bool Read(const pugi::xml_node&, Frame&, ImageManager&);

		//Data members
		uint32 mipmapID;		//The image
		uint32 tme;					//Display time of the image
	};
}


//--------------------------------------------------------------------------------
//	@	Texture_A
//--------------------------------------------------------------------------------
class Texture_A : public Texture
{
public:
	//Constructor/destructor
	Texture_A(): looptime(0) {}
	~Texture_A() {}

	//Copy operations
	Texture_A(const Texture_A&);
	Texture_A& operator=(const Texture_A&);

	//Input
	friend void Read(const pugi::xml_node& in, Texture_A& dest, ImageManager& resource);

	//Return functions
	uint32 GetMipmap(uint32 time = 0) const;

	//Create copy
	Texture_A* clone() const {return new Texture_A(*this);}

private:

	//Data members
	std::vector<impl::Frame> framelist;
	uint32 looptime;

	//Editing functions
	void Add(impl::Frame);	//Add frame and display time

};

#endif