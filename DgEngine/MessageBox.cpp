/*!
 * @file MessageBox.cpp
 *
 * @author Frank Hart
 * @date 22/01/2014
 *
 * Class definitions: MessageBox
 */

#include "MessageBox.h"
#include "Image.h"
#include "DgRect.h"
#include "CommonGraphics.h"
#include "pugixml.hpp"

//--------------------------------------------------------------------------------
//	@	MessageBox::MessageBox()
//--------------------------------------------------------------------------------
//		Constructor
//--------------------------------------------------------------------------------
MessageBox::MessageBox(): borderWidth(10), active(true)
{
	text.SetPosition( borderWidth, borderWidth);
	background.SetBox(DgRect(0,0,1,1));
}


//--------------------------------------------------------------------------------
//	@	MessageBox::SetPosition()
//--------------------------------------------------------------------------------
//		Set the position of the box
//--------------------------------------------------------------------------------
void MessageBox::SetPosition(int32 x, int32 y)
{
	text.SetPosition(x + borderWidth, y + borderWidth);
	background.SetPosition(x, y);
}


//--------------------------------------------------------------------------------
//	@	MessageBox::Draw()
//--------------------------------------------------------------------------------
//		Draw to image
//--------------------------------------------------------------------------------
void MessageBox::Draw(Image& dest) const
{
	background.Draw(dest);
	text.Draw(dest);
}


//--------------------------------------------------------------------------------
//	@	MessageBox::SetBackground()
//--------------------------------------------------------------------------------
//		Set the background color
//--------------------------------------------------------------------------------
void MessageBox::SetBackground()
{
	DgRect temp(text.GetBounds());

	temp.x -= borderWidth;
	temp.y -= borderWidth;
	temp.w += 2 * borderWidth;
	temp.h += 2 * borderWidth;

	background.SetBox(temp);
}

//--------------------------------------------------------------------------------
//	@	MessageBox::Load()
//--------------------------------------------------------------------------------
//		Load a message from the master file
//--------------------------------------------------------------------------------
bool MessageBox::Load(const std::string& str)
{
	//Open file
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(messageBoxFile.c_str());

	//Make sure it opened
	if (!result)
	{
		std::cerr << "MessageBox::Load() -> Failed to open file: " << messageBoxFile << std::endl;

		return false;
	}

	//Get the root node
	pugi::xml_node rootNode = doc.document_element();

	//Find the class in the class document
	pugi::xml_node node = rootNode.find_child_by_attribute(
		"message", "id", str.c_str());

	//Does the class exist?
	if (!node)
	{
        std::cerr << "MessageBox::Load() -> Message does not exist: " << str << std::endl;
		return false;
	}

	//Loop through all nodes
	for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		//Get the name of the node
		std::string tag = it->name();

		if (tag == "string")
		{
			DgString in(it->child_value());
			std::string newString = std::string((std::istreambuf_iterator<char>(in)),
				(std::istreambuf_iterator<char>()));
			SetString(newString);
		}
		else if (tag == "size")
		{
			uint32 size;
			if (!StringToNumber(size, it->child_value(), std::dec))
			{
                std::cerr << "MessageBox::Load() -> Failed to read size." << std::endl;
			}
			else
			{
				SetSize(size);
			}
		}
		else if (tag == "textColor")
		{
			Color temp;
			DgString(it->child_value()) >> temp;
			SetTextColor(temp);
		}
		else if (tag == "backgroundColor")
		{
			Color temp;
			DgString(it->child_value()) >> temp;
			SetBackgroundColor(temp);
		}
		else if (tag == "position")
		{
			int32 x, y;
			DgString ss(it->child_value());
			ss >> x >> y;
			if (!ss)
			{
                std::cerr << "MessageBox::Load() -> Failed to read position." << std::endl;
			}
			else
			{
				SetPosition(x, y);
			}
		}
		else if (tag == "active")
		{
			SetActive(ToBool(it->child_value()));
		}
		else if (tag == "borderWidth")
		{
			uint32 size;
			if (!StringToNumber(size, it->child_value(), std::dec))
			{
                std::cerr << "MessageBox::Load() -> Failed to read border width." << std::endl;
			}
			else
			{
				SetBorder(size);
			}
		}
		else if (tag == "font")
		{
			SetFont(it->child_value());
		}
		else if (tag == "spacing")
		{
			float size;
			if (!StringToNumber(size, it->child_value(), std::dec))
			{
                std::cerr << "MessageBox::Load() -> Failed to read spacing." << std::endl;
			}
			else
			{
				SetSpacing(size);
			}
		}
		else if (tag == "tab")
		{
			float size;
			if (!StringToNumber(size, it->child_value(), std::dec))
			{
                std::cerr << "MessageBox::Load() -> Failed to read tab size." << std::endl;
			}
			else
			{
				SetTab(size);
			}
		}
	}

	return true;
}