#include "Mesh_List.h"
#include "DgError.h"


//--------------------------------------------------------------------------------
//		Return pointer to a base object
//--------------------------------------------------------------------------------
Mesh* Mesh_List::operator[](std::string str)
{
	std::list<Mesh>::iterator it;

	for(it = OList.begin(); it != OList.end(); it++)
	{
		if (it->tag == str)
			return &(*it);
	}

	return Load(str);
}	//End: Mesh_List::Get()


//--------------------------------------------------------------------------------
//		Erase element
//--------------------------------------------------------------------------------
void Mesh_List::Erase(std::string str)
{
	std::list<Mesh>::const_iterator it;

	for(it = OList.begin(); it != OList.end(); it++)
	{
		if (it->tag == str)
		{
			OList.erase(it);
		}
	}

}	//End: Mesh_List::Erase()


//--------------------------------------------------------------------------------
//		Load an object into the list
//--------------------------------------------------------------------------------
Mesh* Mesh_List::Load(std::string tag)
{
	//Temp objects
	std::string str;

	//Add a new, empty base object to the list
	OList.push_back(Mesh());

	//Get pointer to last object
	std::list<Mesh>::iterator it = OList.end();
	it--;

	//Load data into current base object
	str = Mesh_List::folder + tag + "." + 
		Mesh_List::file_extension;
	LoadFile(str,*it);

	//Ensure the file name matches the object tag in the file.
	if (it->tag != tag)
	{
		ERROR_OUT("Mesh_List::Load() -> object tag differs to the file name: filename: " +
			tag + ". tag: " + it->tag);
		it->tag = tag;
	}

	return &(*it);
}	//End: Mesh_List::Load()
