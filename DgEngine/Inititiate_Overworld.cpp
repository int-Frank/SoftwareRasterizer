#include "Overworld.h"
#include "Clipper.h"
#include "Dg_io.h"
#include "Systems.h"
#include "WindowManager.h"

//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
Overworld::Overworld()
{
	//Load from input file
	LoadXMLFile("gamma.xml", *this);

	//Load the help text
	helpBox.Load("help");

	//Assign cameras to viewports (use a SYSTEM)

	//Start timer
	timer.Start();

	//Grab mouse
	mousecontrol.Grab(WINDOW);
	
	fps.SetPosition(10, 10);

}	//End: Overworld::Overworld


//--------------------------------------------------------------------------------
//		Destructor
//--------------------------------------------------------------------------------
Overworld::~Overworld()
{
	//Reset mouse
	mousecontrol.Release(WINDOW);
}


//--------------------------------------------------------------------------------
//		Load the overworld from an xml file
//--------------------------------------------------------------------------------
void operator>>(pugi::xml_node& tool, Overworld& dest)
{
	//--------------------------------------------------------------------------------
	//		Try to find default camera
	//--------------------------------------------------------------------------------

	//iterate through all attributes
    for (	pugi::xml_attribute_iterator ait = tool.attributes_begin(); 
			ait != tool.attributes_end(); ++ait)
    {
		//Get the name of the attribute
		std::string tag = ait->name();

		if (tag == "id")
		{
			dest.id = ait->as_string();
		}

    }

	//iterate through all nodes
	for (pugi::xml_node_iterator it = tool.begin(); it != tool.end(); ++it)
    {
        //Get the name of the node
		std::string tag = it->name();

        if (tag == "skybox")
		{
			*it >> dest.skybox;
		}
		else if (tag == "game_data")
		{
			*it >> dest.gameData;
		}
		else if (tag == "camera_setups")
		{
			*it >> dest.viewport_events;
		}

    }


	//--------------------------------------------------------------------------------
	//		Run post processing system
	//--------------------------------------------------------------------------------

	
	SYSTEM_PostProcess(dest.gameData);
	

}	//End: operator>>()