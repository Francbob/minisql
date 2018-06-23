#ifndef miniSQL_Attribute_H
#define miniSQL_Attribute_H

#include <iostream>
#include <string>

// 0: int, n=1-255: char(n), 256:float
typedef int ValueType;

/// Uses a union to represent three data types.
struct ValueStruct
{
	std::string CHAR_N;
	int INT;
	double FLOAT;
	ValueType TYPE;

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
};

class Attribute{
private:
	std::string name;
	ValueType type;
	int unique ;     //0:unique  1:not unique
public:
	Attribute(std::string n, ValueType t, int u);
	Attribute() = default;
	~Attribute() {};
	std::string get_name(){return name;}
	ValueType get_type(){return type;}
	int get_ifunique(){return unique;}
};

#endif
