#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Text.h"
#include "DgTypes.h"
#include <vector>
#include <map>
#include <string>

namespace pugi{ class xml_node;}

//--------------------------------------------------------------------------------
//		Basic text display 
//--------------------------------------------------------------------------------
class Debugger
{
public:
	//Constructor/Destructor
	Debugger(): x(0), y(0), font("Sansation_Regular"),
		color(Color::WHITE), size(12), line_spacing(0.5){}
	~Debugger() {}

	//Copy operations
	Debugger(const Debugger&);
	Debugger& operator=(const Debugger&);
	
	//Load an xml debug file
	bool Load(std::string file);

	void SetPosition(int32 _x, int32 _y);	//Position of the text block
	void Draw(viewportID);				//Draw on surface

	//Manipulators
	void SetAll(const std::string&);

	//Add an object to the debug output
	template<class T>
	void Input(const T& obj, const std::string&);

private:
	//Data members
	std::vector<Text> descriptions;
	std::map<std::string , Text> inputs;	

	//Coordinates of the debug text
	int32 x, y;

	//Define default text attributes
	std::string font;
	Color color;
    uint16 size; 
	float line_spacing;

	//--------------------------------------------------------------------------------
	//		Functions
	//--------------------------------------------------------------------------------
	float AddLine(const pugi::xml_node&, float current_line);
	void AddText(const pugi::xml_node&, float current_line);
	void AddOutput(const pugi::xml_node&, float current_line);
};


//--------------------------------------------------------------------------------
//		Debugger for text output
//--------------------------------------------------------------------------------
extern Debugger DEBUGGER;


//--------------------------------------------------------------------------------
//		Add object to the debug output
//--------------------------------------------------------------------------------
template<class T>
void Debugger::Input(const T& obj, const std::string& id)
{
	//Get the input as a string
	DgString ss;
	ss << obj;

	//Set input string
	std::map<std::string , Text>::iterator it;
	for (it = inputs.begin(); it != inputs.end(); ++it)
	{
		if (it->first == id)
		{
			it->second.SetString(ss.str());
			return;
		}
	}
}

#endif