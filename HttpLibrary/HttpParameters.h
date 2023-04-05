#pragma once
#include <unordered_map>
#include <string>

class HttpParametersBuilder;

class HttpParametersMap 
{
	private:
		std::unordered_map<std::string, std::string> params;

	public:
		std::string getParameter(const std::string& key) const;
		bool hasParameter(const std::string& key) const;

		friend class HttpParametersBuilder;
};