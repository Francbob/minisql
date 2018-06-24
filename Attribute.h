#ifndef miniSQL_Attribute_H
#define miniSQL_Attribute_H

#include <iostream>
#include <string>

// 0: int, n=1-255: char(n), 256:float
typedef int ValueType;

const char PadChar = '0';

/// Uses a union to represent three data types.
struct ValueStruct
{
	std::string CHAR_N;
	int INT;
	double FLOAT;
	ValueType TYPE;

	ValueStruct(std::string s, int i, double f, ValueType t) { CHAR_N = s; INT = i; FLOAT = f; TYPE = t; };
	ValueStruct(std::string input)
	{

	}

	bool operator== (ValueStruct &value_to_compare)
	{
		if (TYPE == 0)
			return INT == value_to_compare.INT;
		if (TYPE == 256)
			return FLOAT == value_to_compare.FLOAT;
		else
			return CHAR_N == value_to_compare.CHAR_N;
	}

	bool operator!= (ValueStruct &value_to_compare)
	{
		if (TYPE == 0)
			return INT != value_to_compare.INT;
		if (TYPE == 256)
			return FLOAT != value_to_compare.FLOAT;
		else
			return CHAR_N != value_to_compare.CHAR_N;
	}

	bool operator<= (ValueStruct &value_to_compare)
	{
		if (TYPE == 0)
			return INT <= value_to_compare.INT;
		if (TYPE == 256)
			return FLOAT <= value_to_compare.FLOAT;
		else
			return CHAR_N <= value_to_compare.CHAR_N;
	}

	bool operator< (ValueStruct &value_to_compare)
	{
		if (TYPE == 0)
			return INT < value_to_compare.INT;
		if (TYPE == 256)
			return FLOAT < value_to_compare.FLOAT;
		else
			return CHAR_N < value_to_compare.CHAR_N;
	}

	bool operator>= (ValueStruct &value_to_compare)
	{
		if (TYPE == 0)
			return INT >= value_to_compare.INT;
		if (TYPE == 256)
			return FLOAT >= value_to_compare.FLOAT;
		else
			return CHAR_N >= value_to_compare.CHAR_N;
	}

	bool operator> (ValueStruct &value_to_compare)
	{
		if (TYPE == 0)
			return INT > value_to_compare.INT;
		if (TYPE == 256)
			return FLOAT > value_to_compare.FLOAT;
		else
			return CHAR_N > value_to_compare.CHAR_N;
	}

	// 生成可供buffer使用的字符串
	std::string PadToString()
	{
		if (TYPE == 0)
		{
			char result[8];
			sprintf(result, "%08d", INT);
			return result;
		}
		else if (TYPE == 256)
		{
			char result[16];
			sprintf(result, "%16lf", FLOAT);
			return result;
		}
		else
		{
			// 如果超出
			if (CHAR_N.size() > TYPE)
			{
				return CHAR_N.substr(0, TYPE);
			}
			else if (CHAR_N.size() == TYPE)
			{
				return CHAR_N;
			}
			else
			{
				CHAR_N.insert(CHAR_N.size(), TYPE - CHAR_N.size(), PadChar);
			}
		}
	}

	int get_length()
	{
		if (TYPE == 0)
			return 8;
		else if (TYPE == 256)
			return 16;
		else
			return TYPE;
	}
};

class Attribute{
private:
	std::string name;
	ValueType type;
	int unique;     //0:unique  1:not unique
public:
	Attribute(std::string n, ValueType t, int u);
	Attribute() = default;
	~Attribute() {};
	std::string get_name(){return name;}
	ValueType get_type(){return type;}
	int get_ifunique(){return unique;}
	int get_length();
};

#endif
