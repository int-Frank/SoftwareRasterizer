#ifndef MESH_LIST_H
#define MESH_LIST_H

#include <list>
#include <string>
#include "Mesh.h"

//--------------------------------------------------------------------------------
//		Class for containing all Object_BASE objects
//--------------------------------------------------------------------------------
class Mesh_List
{
public:
	//Constructor/Destructor
	Mesh_List() {}
	~Mesh_List() {}

	//Return
	Mesh* operator[](std::string);
	
	//Clear contents
	void Erase(std::string);
	void ClearAll() {OList.clear();}

private:
	//Data members
	std::list<Mesh> OList;	//All base objects are stored here
	static const std::string folder;
	static const std::string file_extension;

	//Load a base object, returns pointer to last object
	Mesh* Load(std::string);

	//DISALLOW Copy operations
	Mesh_List(const Mesh_List&);
	Mesh_List& operator=(const Mesh_List&);
};


//--------------------------------------------------------------------------------
//		Global declarations
//--------------------------------------------------------------------------------
namespace global
{
  extern Mesh_List *MESH_MANAGER;
}


#endif