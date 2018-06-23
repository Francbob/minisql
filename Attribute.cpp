#include "Attribute.h"
#include <string>

using namespace std;

Attribute::Attribute(string n, ValueType t, int u)
{
	name = n;
	type = t;
	unique = u;
}