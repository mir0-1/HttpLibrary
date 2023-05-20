#pragma once
#include <string>

class HttpValue
{
	private:
		bool nullReference;
		std::string value;

	public:
		std::string getAsString() const;
		int getAsInt() const;
		bool getAsBool() const;
		bool isNull() const;

		HttpValue(const std::string& value);
		HttpValue();
};
