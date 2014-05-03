#include "Debugger.h"
#include "Text.h"
#include "Color.h"
#include "DgTypes.h"
#include "CommonGraphics.h"
#include "CommonMath.h"
#include "pugixml.hpp"
#include "ViewportHandler.h"
#include <regex>

//--------------------------------------------------------------------------------
//		Globals
//--------------------------------------------------------------------------------
Debugger DEBUGGER;


//--------------------------------------------------------------------------------
//		Load xml file
//--------------------------------------------------------------------------------
bool Debugger::Load(std::string file)
{
	//Load the document
	pugi::xml_document doc;
    if (!doc.load_file(file.c_str())) 
		return false;

	//Get the root node
    pugi::xml_node node = doc.child("default");

	//iterate through all attributes
    for (	pugi::xml_attribute_iterator ait = node.attributes_begin(); 
			ait != node.attributes_end(); ++ait)
    {
		//Get the name of the attribute
		std::string tag = ait->name();

		if (tag == "font")
		{
			font = ait->as_string();
		}
		else if (tag == "color")
		{
			DgString(ait->value()) >> color;
		}
		else if (tag == "size")
		{
			size = uint16(ait->as_uint());
		}
		else if (tag == "line_spacing")
		{
			line_spacing = ait->as_float();
		}
		else if (tag == "x")
		{
			x = ait->as_int();
		}
		else if (tag == "y")
		{
			y = ait->as_int();
		}
    }

	//Current line number
	float current_line = 0.0f;

	//iterate through all child nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

        if (tag == "line")
		{
			current_line = AddLine(*it, current_line);
		}
    }

	return true;
}	//End: Debugger::Load()


//--------------------------------------------------------------------------------
//		Add a single line of text to the document
//--------------------------------------------------------------------------------
float Debugger::AddLine(const pugi::xml_node& node, float current_line)
{
	//Run through attributes
	for (	pugi::xml_attribute_iterator ait = node.attributes_begin(); 
			ait != node.attributes_end(); ++ait)
    {
		//Get the name of the attribute
		std::string tag = ait->name();

		if (tag == "return")	//as if 'return' key has been pressed
		{
			current_line += DgFloor(ait->as_float());
		}
    }

	//iterate through all child nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

        if (tag == "text")
		{
			AddText(*it, current_line);
		}
		else if (tag == "output")
		{
			AddOutput(*it, current_line);
		}
    }

	//Move to next line
	return current_line += 1.0f;

}	//End: Debugger::AddText()


//--------------------------------------------------------------------------------
//		Add a single line of text to the document
//--------------------------------------------------------------------------------
void Debugger::AddText(const pugi::xml_node& node, float current_line)
{
	//Create new text element
	Text temp;

	//Get/set string
	std::string str = node.text().get();
	temp.SetString(str);

	//Set up the text object
	temp.SetFont(font);
	temp.SetSize(size);
	temp.SetColor(color);
	
	int32 indent = 0;

	//Run through attributes
	for (	pugi::xml_attribute_iterator ait = node.attributes_begin(); 
			ait != node.attributes_end(); ++ait)
    {
		//Get the name of the attribute
		std::string tag = ait->name();

		if (tag == "indent")
		{
			indent = ait->as_int();
		}
		else if (tag == "color")
		{
			DgString ss(ait->as_string());
			Color temp_color;
			ss >> temp_color;
			temp.SetColor(temp_color);
		}
    }

	//Set coordinates of the text object
	temp.SetPosition((indent + x), int32(current_line * (line_spacing + 1.0f) * int32(size)));

	//Add to descriptions container
	descriptions.push_back(temp);

}	//End: Debugger::AddText()


//--------------------------------------------------------------------------------
//		Add a single line of text to the document
//--------------------------------------------------------------------------------
void Debugger::AddOutput(const pugi::xml_node& node, float current_line)
{
	//Create new text element
	Text temp;

	//Set up the text object
	temp.SetFont(font);
	temp.SetSize(size);
	temp.SetColor(color);
	
	int32 indent = 0;
	std::string id = "";

	//Run through attributes
	for (	pugi::xml_attribute_iterator ait = node.attributes_begin(); 
			ait != node.attributes_end(); ++ait)
    {
		//Get the name of the attribute
		std::string tag = ait->name();

		if (tag == "indent")
		{
			indent = ait->as_int();
		}
		else if (tag == "color")
		{
			DgString ss(ait->as_string());
			Color temp_color;
			ss >> temp_color;
			temp.SetColor(temp_color);
		}
		else if (tag == "id")
		{
			id = ait->as_string();
		}
    }

	//Check if id has been set
	if (id == "")
		return;

	//Set coordinates of the text object
	temp.SetPosition((indent + x), int32(current_line * (line_spacing + 1.0f) * int32(size)));

	//Add to inputs container
	inputs.insert( std::pair<std::string, Text>(id, temp) );


}	//End: Debugger::AddText()


//--------------------------------------------------------------------------------
//		Set Position
//--------------------------------------------------------------------------------
void Debugger::SetPosition(int32 _x, int32 _y)
{
	//SetData
	int32 dx = _x - x;
	int32 dy = _y - y;
	x = _x;
	y = _y;

	//Set Position of each Text Object in descriptions
	for (uint32 i = 0; i < descriptions.size(); i++)
	{
		descriptions[i].SetPosition(
			descriptions[i].X()+dx, 
			descriptions[i].Y()+dy);
	}

	//Set Position of each Text Object in inputs
	std::map<std::string , Text>::iterator it;
	for (it = inputs.begin(); it != inputs.end(); ++it)
	{
		it->second.SetPosition(
			it->second.X()+dx, 
			it->second.Y()+dy);
	}
}	//End: textBLOCK::SetPosition()


//--------------------------------------------------------------------------------
//		Draw text block
//--------------------------------------------------------------------------------
void Debugger::Draw(viewportID id)
{
	//Draw descriptions
	for (uint32 i = 0; i < descriptions.size(); i++)
	{
		descriptions[i].Draw(id);
	}

	//Set Position of each Text Object in inputs
	std::map<std::string , Text>::iterator it;
	for (it = inputs.begin(); it != inputs.end(); ++it)
	{
		it->second.Draw(id);
	}
}	//End: Debugger::Draw()


//--------------------------------------------------------------------------------
//		Set all outputs to this string
//--------------------------------------------------------------------------------
void Debugger::SetAll(const std::string& str)
{
	std::map<std::string , Text>::iterator it;
	for (it = inputs.begin(); it != inputs.end(); ++it)
		it->second.SetString(str);

}	//End: Debugger::SetAll()