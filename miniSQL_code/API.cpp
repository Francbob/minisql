#include "API.h"
#include "RecordManager.h"
#include "IndexManager.h"
#include "CatalogManager.h"

RecordManager *ptrRecord;
CatalogManager *ptrCatalog;
IndexManager *ptrIndex;

//Function to create a table.
void API::API_CreateTable(string Table_name, vector<Attribute> set_Attribute, vector<string> PrimaryKey){
	//Find whether there's already a table named "table_name"
	if(ptrCatalog->FindTable(Table_name)){
		cout<<"There's already a table named "<<Table_name<<endl;
		return;
	}else{
		
		//Create table
		int flag;
		flag = ptrRecord->CreateTable(Table_name,set_Attribute, PrimaryKey);
		if(flag==0){
			cout<<"Failed to create a table"<< endl;
			return;
		}
		
		//There need to be changed!!!!
		//We need to build an index of primary key.
		if(flag==0){
			//cout<<"We have created the table successfully, but failed to create an index on "<<PrimaryKey<<endl;
			return;
		}else{
			cout<<"Create a table succeed!"<<endl; 
		}
	}
	return;
}

//Function of dropping a table
void API::API_DropTable(string Table_name){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl;
		return ;
	}else{
		//Find indexs in this table
		vector<vector<string> > Index_name;
		int flag;
		Index_name = ptrCatalog->FindIndex(Table_name);
		
		//Drop all indexs in this table
		flag = ptrIndex->DropIndex(Table_name, Index_name);
		if(flag==0){
			cout<<"Dropping the table failed because we failed in droping indexs on it"<<endl;
			return;
		}
		
		//Drop table
		flag = ptrRecord->DropTable(Table_name);
		if(flag==0){
			cout<<"Succeed in dropping indexs on this table but drop the table failed"<<endl;
		    return; 
		}else{
			cout<<"Drop the table succeed!"<<endl;
			return;
		}
	}
}

//Selecting whithout any conditions(for example "select *")
void API::API_Select(vector<string> select_Attribute, string Table_name){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's no table named "<<Table_name<<endl;
		return;
	}else{
		//Select all tuples in this table.
		int flag;
		flag = ptrRecord->SelectAll(Table_name, select_Attribute);
		if(flag==0){
			cout<<"Failed to get the selecting result, maybe there's something wrong with attributes you give"<<endl;
			return; 
		}else{
			cout<<"Selecting succceed!"<<endl;
			return;
		}
	}
}

//Selecting with some conditions
void API::API_Select(vector<string> select_Attribute, string Table_name, vector<condition> select_condition){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl;
		return; 
	}else{
		int flag, flag_select, i;
		vector<string> FindInIndex;
		
		//Find attribute_name in select_condition
		for(i=0; i<select_condition.size(); i++){
			FindInIndex.push_back(select_condition[i].Attribute_name);
		}
		
		//Find whether there's an index of these attributes
		if(ptrCatalog->WhetherIndex(Table_name, FindInIndex)){
			//Find with index, in this part we're sure there's an index on these attributes.
			flag_select = ptrRecord->SelectWithIndex(select_Attribute, Table_name, select_condition);
			if(flag_select==1){
				cout<<"Selecting succeed!"<<endl;
				return;
			}
		}
		//If there's something wrong with 'Find with index', we'll try to find using conventional method
		flag_select = ptrRecord->Select(select_Attribute, Table_name, select_condition);
		if(flag_select==0){
			cout<<"Failed to select!"<<endl;
			return;  
		}else{
			cout<<"Select succeed!"<<endl;
			return;
		}
	}
}

//Insert some tuples into this table
void API::API_Insert(string Table_name, vector<map<string, string> > insert_vector){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		int flag;
		flag = ptrRecord->Insert(Table_name, insert_vector);
		if(flag==0){
			cout<<"Failed to insert. Maybe there's something wrong with attributes and their values you give"<<endl;
		}else{
			//Maybe need to add some items into index
			flag = ptrIndex->AddIndex(Table_name, insert_vector);
			if(flag==0){
				cout<<"Insert succeed but failed to add items into index"<<endl;
			}else{
				cout<<"Insert succeed!"<<endl;
			}
		}
	}
	return;
}

//Delete all tuples
void API::API_Delete(string Table_name){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		int flag;
		//Delete all tuples in this table
		flag = ptrRecord->DeleteWhole(Table_name);
		if(flag==0){
			cout<<"Failed to delete all the tuples"<<endl;
		}else{
			//Delete all items in all indexs
			flag = ptrIndex->DeleteWhole(Table_name);
			if(flag==0){
				cout<<"Delete succeed, but failed to update indexs in this table."<<endl;
			}else{
				cout<<"Delete succeed!"<<endl;
			}
		}
	}
	return;
}

//Delete some tuples in the table with som conditions
void API::API_Delete(string Table_name, vector<condition> delete_vector){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		int flag;
		flag = ptrRecord->Delete(Table_name, delete_vector);
		if(flag==0){
			cout<<"Failed to delete!"<<endl;
		}else{
			vector<map<string, string> > ForDeleteIndex;
			
			//Find attributes and values of attributes, in order to delete items in indexs
			ForDeleteIndex = ptrRecord->FindA_and_V(Table_name, delete_vector);
			
			//Delete some items in index
			flag = ptrIndex->Delete(Table_name, ForDeleteIndex);
			if(flag==0){
				cout<<"Delete suceed but failed to delete items in indexs."<<endl;
			}else{
				cout<<"Delete succeed!"<<endl;
			}
			
		}
	}
	return;
}

//Create an index of the giving set of attributes
void API::API_CreateIndex(string Table_name, vector<Attribute> index_Attribute){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		//Find whether there's already an index of these attributes
		vector<string> FindIndex;
		int i, flag; 
		for(i=0; i<index_Attribute.size(); i++){
			FindIndex.push_back(index_Attribute[i].get_name());
		}
		if(ptrCatalog->WhetherIndex(Table_name, FindIndex)){
			cout<<"There's already an index of these attributes."<<endl;
			return;
		}
		
		//Create index
		flag = ptrIndex->CreateIndex(Table_name, index_Attribute);
		if(flag==0){
			cout<<"Creating index fails."<<endl;
		}else{
			cout<<"Created index succeed!"<<endl;
		}
	}
	return;
}

//Drop an index of the giving set of attributes
void API::API_DropIndex(string Table_name, vector<Attribute> index_Attribute){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		
		//Find whether there's an index of the giving set of attributes
		vector<string> FindIndex;
		int i, flag; 
		for(i=0; i<index_Attribute.size(); i++){
			FindIndex.push_back(index_Attribute[i].get_name());
		}
		if(!ptrCatalog->WhetherIndex(Table_name, FindIndex)){
			cout<<"There's not an index of these attributes."<<endl;
		}else{
			
			//Do this job to satisfy the requests of function DropIndex
			vector<vector<string> > ForDeleteIndex;
			ForDeleteIndex.push_back(FindIndex);
			
			//Drop index
			flag = ptrIndex->DropIndex(Table_name, ForDeleteIndex);
			if(flag==0){
				cout<<"Failed to drop index of these attributes."<<endl;
			}else{
				cout<<"Drop index succeed!"<<endl;
			}
		}
	}
	return;
}

//Show all indexs of this table
void API::Show_Index(string Table_name){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		int flag;
		flag = ptrIndex->ShowIndex(Table_name);
		if(flag==0){
			cout<<"Failed to show all indexs in this table."<<endl;
		}else{
			cout<<"Show index succeed!"<<endl;
		}
	}
	return;
} 








