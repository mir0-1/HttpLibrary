#pragma once

class HttpParameter
{
	private:
		char* value;

	public:
		const char* getValue() const;
		int getValueAsInt() const;
		bool getValueAsBool() const;

		HttpParameter(char* value);
};