#ifndef miniSQL_Attribute_H
#define miniSQL_Attribute_H

#include <iostream>
#include <string>

// 0: int, n=1-255: char(n), 256:float
typedef size_t ValueType;

#ifndef TYPE_FLOAT
#define TYPE_FLOAT 256
#endif // !FLOAT
#ifndef TYPE_INT
#define TYPE_INT 0
#endif // !INT

const char PadChar = '0';

/// Uses a union to represent three data types.
struct ValueStruct
{
	std::string CHAR_N;
	int INT;
	double FLOAT;
	ValueType TYPE;

	ValueStruct() {};

	ValueStruct(std::string s, int i, double f, ValueType t) { CHAR_N = s; INT = i; FLOAT = f; TYPE = t; };

	// 在Interpreter读的时候用
	ValueStruct(std::string input)
	{
		if (input[0] == '\'' && input[input.size() - 1] == '\'')
		{
			std::string char_n = input.substr(1, input.size() - 2);
			TYPE = char_n.size();
			CHAR_N = char_n;
		}
		else if (input.find('.') != std::string::npos)
		{
			TYPE = TYPE_FLOAT;
			FLOAT = std::stod(input);
		}
		else
		{
			TYPE = TYPE_INT;
			INT = std::stoi(input);
		}
	}

	// 知道Type的时候用
	ValueStruct(std::string input, ValueType type)
	{
		if (type == TYPE_INT)
		{
			TYPE = TYPE_INT;
			INT = std::stoi(input);
		}
		else if (type == TYPE_FLOAT)
		{
			TYPE = TYPE_FLOAT;
			FLOAT = std::stod(input);
		}
		else
		{
			TYPE = type;
			std::string char_n = input.substr(0, type);
			CHAR_N = char_n;
		}
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
			char result[9];
			sprintf_s(result, "%08d", INT);
			return result;
		}
		else if (TYPE == 256)
		{
			char result[17];
			sprintf_s(result, "%016.7lf", FLOAT);
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
				return CHAR_N.insert(CHAR_N.size(), TYPE - CHAR_N.size(), PadChar);
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

	void print()
	{
		if (TYPE == 0)
			std::cout << INT;
		else if (TYPE == 256)
			std::cout << FLOAT;
		else
			std::cout << CHAR_N;
	}
};


class Attribute {
private:
	std::string name;
	ValueType type;
	int unique;     //0:unique  1:not unique
public:
	Attribute(std::string n, ValueType t, int u);
	Attribute() = default;
	~Attribute() {};
	std::string get_name() { return name; }
	ValueType get_type() { return type; }
	int get_ifunique() { return unique; }
	int get_length();
};

#endif
