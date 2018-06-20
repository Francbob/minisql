#ifndef Condition_H
#define Condition_H

#include <iostream>
#include <sstream>        //used to transfer string and int
#include <string>
using namespace std;

class condition{
public:
	string value;        
	int type_compare;      //  attribute in the table    value
						   //0                        =
						   //1                        !=
						   //2                        <
						   //3                        <=
						   //4                        >
						   //5                        >=
	string Attribute_name; //Attribute name
	condition(int t, string A, string v);
	
	//There are compare functions of delete/insert conditions
	//"value" is the comparative value gived by users
	//Attribute_name is the attribute name in table needed to be compared
	//When you want to use conpare function, you just need to give the value of attribute named "Attribute_name"
	bool compare(int value_for_compare);
	bool compare(float value_for_compare);
	bool compare(string value_for_compare);
};

#endif 
