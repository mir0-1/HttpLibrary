#pragma once
#include <string>

class HttpParameterValue
{
	private:
		bool nullReference;
		std::string value;

	public:
		std::string getAsString() const;
		int getAsInt() const;
		bool getAsBool() const;
		bool isNull() const;

		HttpParameterValue(const std::string& value);
		HttpParameterValue();
};

