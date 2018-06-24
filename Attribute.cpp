#include "Attribute.h"
#include <string>

using namespace std;

Attribute::Attribute(string n, ValueType t, int u)
{
	name = n;
	type = t;
	unique = u;
}

int Attribute::get_length()
{
	//int
	if (type == 0)
		return 8;
	else if (type == 256)
		return 16;
	else
		return type;
}