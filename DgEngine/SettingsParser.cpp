#include "SettingsParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>



bool SettingsParser::Load(const std::string& file)
{
    //Open file
    std::ifstream file_in(file);

    //Ensure file has opened
    if (!file_in)
    {
        std::cerr << "@DgUtility::GetFromFile() -> Failed to open file: " << file << std::endl;
        return false;
    }

    //Check first character


    //temp values
    std::string str;
    std::string tag;
    char chk;

    //Search file
    while (file_in >> chk)
    {
        //Check for comments
        if (chk == s_comment_char)
        {
            std::getline(file_in, str);
            continue;
        }
        else
        {
            file_in.putback(chk);
        }

        file_in >> tag;
        std::getline(file_in, str);

        m_values.insert(std::pair<std::string, std::string>(tag, str));
    }

    return true;
}


bool SettingsParser::GetValue(const std::string& tag, std::string& out) const
{
    std::map<std::string, std::string>::const_iterator it;
    it = m_values.find(tag);

    if (it == m_values.end())
    {
        return false;
    }

    out = it->second;
    return true;
}