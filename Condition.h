#ifndef Condition_H
#define Condition_H

#include <iostream>
#include <sstream>        //used to transfer string and int
#include <string>
#include "Attribute.h"

class condition{
public:  
	enum operationType
	{
		equal = 0,
		not_equ,
		less,
		less_or_equ,
		greater,
		greater_or_equ,
		unknown
	} type_compare;
	condition(operationType t, std::string A, struct ValueStruct v);
	condition() {};

	std::string Attribute_name; //Attribute name
	struct ValueStruct value;
	operationType getOpType(std::string input);
	//There are compare functions of delete/insert conditions
	//"value" is the comparative value gived by users
	//Attribute_name is the attribute name in table needed to be compared
	//When you want to use conpare function, you just need to give the value of attribute named "Attribute_name"
	bool compare(std::string padString);

	bool compare(ValueStruct &value_to_compare);
};

#endif 
