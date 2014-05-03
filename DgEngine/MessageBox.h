/*!
 * @file MessageBox.h
 *
 * @author Frank Hart
 * @date 22/01/2014
 *
 * Class declaration: MessageBox
 */

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include "Text.h"
#include "Rectangle.h"

class Image;

/*!
 * @ingroup graphics_text
 *
 * @class MessageBox
 *
 * @brief A Message box
 *
 * [detailed description]
 *
 * @author Frank Hart
 * @date
 */
class MessageBox
{
public:

	MessageBox();
	~MessageBox() {}

	MessageBox(const MessageBox&);
	MessageBox& operator= (const MessageBox&);

	void SetFont(std::string s)		{ text.SetFont(s); SetBackground(); }
	void SetSize(uint16 s)			{ text.SetSize(s); SetBackground(); }
	void SetTextColor(Color c)		{ text.SetColor(c);}
	void SetString(std::string s)	{ text.SetString(s); SetBackground(); }
	void SetSpacing(float s)		{ text.SetSpacing(s); SetBackground(); }
	void SetTab(float t)			{ text.SetTab(t); SetBackground(); }
	void SetBorder(uint32 b)		{ borderWidth = b; SetBackground(); }

	void SetBackgroundColor(Color c) { background.SetColor(c.i); }
	void SetPosition(int32 x, int32 y);

	void SetActive(bool b) { active = b; }
	void Activate() { active = true; }
	void Deactivate() { active = false; }
	void ToggleActive() { active = !active; }
	bool IsActive() const { return active; }

	//Load a specific message box from the master file.
	bool Load(const std::string&);

	void Draw(Image&) const;

private:

	Text text;
	Rectangle background;

	//Distance from the text to the edge of the background
	uint32 borderWidth;

	bool active;

	//Functions
	void SetBackground();

private:

	static const std::string messageBoxFile;
};

#endif