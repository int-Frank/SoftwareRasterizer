#ifndef SETTINGSPARSER_H
#define SETTINGSPARSER_H

#include <map>
#include <string>

class SettingsParser
{
public:

    bool Load(const std::string& file);
    bool GetValue(const std::string& tag, std::string& out) const;

private:
    std::map<std::string, std::string> m_values;

    static const char s_comment_char = '#';
};

//--------------------------------------------------------------------------------
//		Globals
//--------------------------------------------------------------------------------
namespace global
{
  extern SettingsParser *SETTINGS;
}

#endif