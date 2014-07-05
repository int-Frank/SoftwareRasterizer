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
#include "dg_vector_s.h"

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
		Frame() : mipmapID(ImageManager::DEFAULT_MM), tme(10) {}

		//Destructor
		~Frame() {}

		//Copy operations
		Frame(const Frame& f) : tme(f.tme), mipmapID(f.mipmapID) {}

		//Input
		friend void Read(const pugi::xml_node&, Frame&, ImageManager&);

		//Data members
		uint32_t mipmapID;		//The image
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
	uint32_t GetMipmap(uint32) const;

	//Create copy
	Texture_A* clone() const {return new Texture_A(*this);}

private:

	//Data members
	Dg::vector_s<impl::Frame> framelist;
	uint32 looptime;

	//Editing functions
	void Add(const impl::Frame&);	//Add frame and display time

};

#endif