#pragma once
#include <unordered_map>
#include <string>


namespace Tituf
{
	class Config
	{
	public:
		Config() = default;
		bool Load(const std::string& filepath);
		int GetInt(const std::string& section, const std::string& key, int defaultValue);
		std::string GetString(const std::string& section, const std::string& key, const std::string& defaultValue);
		std::wstring GetWString(const std::string& section, const std::string& key, const std::wstring& defaultValue);


	private:
		std::unordered_map<std::string, std::string> data;
	};

}

	 