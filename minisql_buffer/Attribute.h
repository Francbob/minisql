#ifndef miniSQL_Attribute_H
#define miniSQL_Attribute_H

#include <iostream>
#include <string>
using namespace std;

class Attribute{
private:
	string name;
	int type;        //0:int  1:float  2:fixed length string 3:not fixed length string
	int length;      //type=2, length means the fixed length of the string
					 //type=3, length means the upper bound of the length of the string 
	int unique ;     //0:unique  1:not unique
public:
	Attribute(string n, int t, int l, int u);
	~Attribute();
	string get_name(){return name;}
	int get_type(){return type;}
	int get_length(){return length;}
	int get_ifunique(){return unique;}
};

#endif
