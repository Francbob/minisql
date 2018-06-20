#include "Condition.h"

condition::condition(int t, string A, string v){
	type_compare = t;
	Attribute_name = A;
	value = v;
}

bool condition::compare(int value_for_compare){
	stringstream ss;
	ss<<value;
	int int_value;
	ss>>int_value;
	switch(type_compare){
		case 0:
			return value_for_compare == int_value;
		case 1:
			return value_for_compare != int_value;
		case 2:
			return value_for_compare <  int_value;
		case 3:
			return value_for_compare <= int_value;
		case 4:
			return value_for_compare >  int_value;
		case 5:
			return value_for_compare >= int_value; 
	}	
}

bool condition::compare(float value_for_compare){
	stringstream ss;
	ss<<value;
	float float_value;
	ss>>float_value;
	switch(type_compare){
		case 0:
			return value_for_compare == float_value;
		case 1:
			return value_for_compare != float_value;
		case 2:
			return value_for_compare <  float_value;
		case 3:
			return value_for_compare <= float_value;
		case 4:
			return value_for_compare >  float_value;
		case 5:
			return value_for_compare >= float_value;  
	}	
}

bool condition::compare(string value_for_compare){
	switch(type_compare){
		case 0:
			return value_for_compare == value;
		case 1:
			return value_for_compare != value;
		case 2:
			return value_for_compare <  value;
		case 3:
			return value_for_compare <= value;
		case 4:
			return value_for_compare >  value;
		case 5:
			return value_for_compare >= value; 
	}	
}



