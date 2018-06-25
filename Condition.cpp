#include "Condition.h"
#include "Attribute.h"
#include <string>

using namespace std;

condition::condition(operationType t, string A, ValueStruct v) {
	type_compare = t;
	Attribute_name = A;
	value = v;
}

bool condition::compare(string padString)
{
	string curString = value.PadToString();
	switch (type_compare)
	{
	case equal:
		return curString == padString;
	case not_equ:
		return curString != padString;
	case less:
		return curString < padString;
	case less_or_equ:
		return curString <= padString;
	case greater:
		return curString > padString;
	case greater_or_equ:
		return curString >= padString;
	default:
		throw exception();
	}
}

bool condition::compare(ValueStruct &value_to_compare) {
	switch (type_compare) {
	case equal:
		return value == value_to_compare;
	case not_equ:
		return value != value_to_compare;
	case less:
		return value < value_to_compare;
	case less_or_equ:
		return value <= value_to_compare;
	case greater:
		return value > value_to_compare;
	case greater_or_equ:
		return value >= value_to_compare;
	default:
		throw exception();
	}
}

// ���ߺ���
condition::operationType condition::getOpType(std::string input)
{
	operationType retType;
	if (input == "=" || input == "==")
		retType = equal;
	else if (input == "<>")
		retType = not_equ;
	else if (input == "<")
		retType = less;
	else if (input == ">")
		retType = greater;
	else if (input == "<=")
		retType = less_or_equ;
	else if (input == ">=")
		retType = greater_or_equ;
	else
		retType = unknown;
	return retType;
}