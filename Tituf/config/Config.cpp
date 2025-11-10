#include "tfpch.h"
#include "Config.h"

namespace Tituf
{

    bool Config::Load(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "Could not open config file: " << filepath << std::endl;
            return false;
        }

        std::string line, currentSection;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == ';' || line[0] == '#')
                continue; // skip comments

            if (line.front() == '[' && line.back() == ']')
            {
                currentSection = line.substr(1, line.size() - 2);
                continue;
            }

            size_t eq = line.find('=');
            if (eq == std::string::npos)
                continue;

            std::string key = currentSection + "." + line.substr(0, eq);
            std::string value = line.substr(eq + 1);

            data[key] = value;
        }

        return true;
    }

    int Config::GetInt(const std::string& section, const std::string& key, int defaultValue)
    {
        std::string combined = section + "." + key;
        if (data.find(combined) != data.end())
            return std::stoi(data[combined]);
        return defaultValue;
    }

    std::string Config::GetString(const std::string& section, const std::string& key, const std::string& defaultValue)
    {
        std::string combined = section + "." + key;
        if (data.find(combined) != data.end())
            return data[combined];
        return defaultValue;
    }
    std::wstring Config::GetWString(const std::string& section, const std::string& key, const std::wstring& defaultValue)
    {
        std::string combined = section + "." + key;
        auto it = data.find(combined);
        if (it != data.end())
        {
            // Convert std::string (assume UTF-8/ASCII) to std::wstring
            std::wstring wvalue(it->second.begin(), it->second.end());
            return wvalue;
        }
        return defaultValue;
    }

}