#ifndef SKYBOX_H
#define SKYBOX_H

#include "Quaternion.h"
#include "DgArray.h"
#include <string>

class Image;
class Viewport;
class Mesh;
namespace pugi{class xml_node;}

class Skybox
{
public:
	//Constructor / destructor
	Skybox();
	~Skybox() {}

	//Copy operations
	Skybox(const Skybox&);
	Skybox& operator= (const Skybox&);
	
	//Create object from input
	friend pugi::xml_node& operator>>(pugi::xml_node& node, Skybox& dest);

	//Texture
	void SetTexture(const std::string&);

	//Transform
	void SetOrientation(const Quaternion&);
	void OrientateCubeToCamera(const Quaternion&);

	//Send skybox to master polygon list
	void SendToRenderer(Viewport*) const;

private:
	//--------------------------------------------------------------------------------
	//		Data
	//--------------------------------------------------------------------------------

	//Images
	const Image* top;		
	const Image* bottom;
	const Image* left;
	const Image* right;
	const Image* front;
	const Image* back;

	//The cube
	Mesh* cube;

	//Orientation of the Skybox
	Quaternion Q_WLD_OBJ;

	//--------------------------------------------------------------------------------
	//		Statics
	//--------------------------------------------------------------------------------
	const static std::string obj_file;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
	void NewVQS() {}

	void init(const Skybox&);
};

#endif