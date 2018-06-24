#include "API.h"
#include "RecordManager.h"
#include "IndexManager.h"
#include "CatalogManager.h"
#include "algorithm"

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
		int flag, i;
		flag = ptrRecord->CreateTable(Table_name,set_Attribute);
		if(flag==0){
			cout<<"Failed to create a table"<< endl;
			return;
		}

		vector<Attribute> AddIndex;
		flag = set_Attribute.size();
		for(i=0; i<flag; i++){
			vector<int>::iterator it;
			string temp = set_Attribute[i].get_name();
			it = find(PrimaryKey.begin(), PrimaryKey.end(), temp);
			if(it!=PrimaryKey.end()){
				AddIndex.push_back(set_Attribute[i]);
			}
		}
		flag = ptrIndex->CreatePrimaryIndex(Table_name, AddIndex);

		if(flag==0){
			cout<<"We have created the table successfully, but failed to create an index on primary key"<<endl;
			return;
		}else{
			flag = ptrCatalog->CreateTable(Table_name, set_Attribute, PrimaryKey);
			if(flag==1){
				cout<<"Create table succeed!"<<endl;
			}
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
		vector<string> Index_name;
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
			flag = ptrCatalog->DropTable(Table_name);
			if(flag==1){
				cout<<"Drop the table succeed!"<<endl;
				return;
			}
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

void API::API_SelectAll(string Table_name){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl;
		return; 
	}else{
		int flag;
		flag = ptrRecord->Select(Table_name);
		if(flag==0){
			cout<<"Failed to select!"<<endl;
		}
	}
	return;
}

int API::API_IfAllIndex(string Table_name, vector<condition> select_condition, vector<condition> &Condition_with_index, vector<condition> &Condition_no_index){
		vector<string> Index_name;
		int i, flag;
		Index_name = ptrCatalog->FindIndex(Table_name);
		size_condition = select_condition.size();
		for(i=0; i<size_index; i++){
			string temp = select_condition[i].Attribute_name;
			it = find(Index_name.begin(), Index_name.end(), temp);
			if(it!=Index_name.end()){
				Condition_with_index.push_back(select_condition[i]);
			}else{
				Condition_no_index.push_back(select_condition[i]);
			}
		}
		if(Condition_with_index.size()==select_condition.size()){
			return 0;
		}
		if(Condition_no_index.size()==select_condition.size()){
			return 1;
		}
		return 2;
}

void API::API_SelectAll(string Table_name, vector<condition> select_condition){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl;
		return; 
	}else{
		vector<condition> Condition_with_index, Condition_no_index;
		vector<string> Judge;
		map<int, int> Index_find;
		int i, flag, flag_select;

		//Judge attributes
		for(i=0; i<select_condition.size(); i++){
			Judge.push_back(select_condition[i].Attribute_name);
		}
		flag = ptrCatalog->JudgeAttrSelect(Table_name, Judge);
		if(flag==0){
			cout<<"The attributes you give don' fit this table!"<<endl;
			return;
		}

		flag = API_IfAllIndex(Table_name, select_condition, Condition_with_index, Condition_no_index);
		if(flag==0 || flag==2){
			Index_find = ptrIndex->Select(Table_name, Condition_with_index);
			if(flag==0){
				flag_select = ptrRecord->Select(Table_name, Index_find);
			}else if(flag==2){
				flag_select = ptrRecord->Select(Table_name, Index_find, Condition_no_index);
			}
		}else{
			flag_select = ptrRecord->Select(Table_name, Condition_no_index);
		}
	}
	if(flag_select == 0){
		cout<<"Select failed"<<endl;
	}
	return;
}


//Insert some tuples into this table
void API::API_Insert(string Table_name, vector<string> insert_vector){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		int flag;
		//Judge records
		flag = ptrCatalog->JudgeAttrValue(Table_name, insert_vector);
		if(flag == 0){
			cout<<"Your giving attributes don't fit this table"<<endl;
			return;
		}
		//insert records
		bool flag_bool;
		flag = ptrRecord->Insert(Table_name, insert_vector);
		if(flag==0){
			cout<<"Failed to insert"<<endl;
		}else{
			map<string, string> Insert_index;
			Insert_index = ptrCatalog->JudgeInsertIndex(Table_name, insert_vector);
			//Add record number in catalog
			//Record+=1;
			flag_bool = ptrCatalog->InsertRecord(Table_name);
			//Add index
			flag = ptrIndex->AddIndex(Table_name, Insert_index);
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
		bool flag_bool;
		//Delete all tuples in this table
		flag = ptrRecord->Delete(Table_name);
		if(flag==0){
			cout<<"Failed to delete all the tuples"<<endl;
		}else{
			//Delete all record in catalog "Table_Information"
			flag_bool = ptrCatalog->DeleteRecord(Table_name);
			
			//Delete all items in all indexs

			vector<string> DeleteIndex = ptrCatalog->FindIndex(Table_name);
			flag = ptrIndex->Delete(Table_name, DeleteIndex);
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
//RecordManager call ptrCatalog->DeleteRecord
void API::API_Delete(string Table_name, vector<condition> delete_condition){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		vector<condition> Condition_with_index, Condition_no_index;
		vector<string> Judge;
		map<int, int> Index_find;
		map<int, int> Delete_record;
		int i, flag, flag_delete;

		//Judge records
		for(i=0; i<delete_condition.size(); i++){
			Judge.push_back(delete_condition[i].Attribute_name);
		}
		flag = ptrCatalog->JudgeAttrSelect(Table_name, Judge);
		if(flag==0){
			cout<<"The attributes you give don' fit this table!"<<endl;
			return;
		}

		bool flag_bool;
		flag = API_IfAllIndex(Table_name, delete_condition, Condition_with_index, Condition_no_index);

		//Delete records
		if(flag==0 || flag==1){
			Index_find = ptrIndex->Select(Table_name, Condition_with_index);
			if(flag==0){
				flag_delete = ptrRecord->Delete(Table_name, Index_find, &Delete_record);
			}else if(flag == 1){
				flag_delete = ptrRecord->Delete(Table_name, Index_find, Condition_no_index, &Delete_record);
			}
		}else if(flag==2){
			flag_delete = ptrRecord->Delete(Table_name, Condition_no_index, &Delete_record);
		}

		//Delete records in catalog

		flag_bool = ptrCatalog->DeleteRecord(Table_name, flag_delete);

		//Delete index

		flag = ptrIndex->Delete(Table_name, Delete_record);

		if(flag==0){
			cout<<"Delete records succeed, but failed to delete index on it"<<endl;
		}else{
			cout<<"Delete succeed!"<<endl;
		}
	}
	return;
}

//Create an index of the giving set of attributes
void API::API_CreateIndex(string Table_name, Attribute index_Attribute){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		//Find whether there's already an index of these attributes
		string FindIndex;
		int i, flag; 
		bool flag_bool;
		FindIndex = index_Atribute.name;
		if(ptrCatalog->WhetherIndex(Table_name, FindIndex)){
			cout<<"There's already an index of this attribute."<<endl;
			return;
		}
		
		//Create index
		flag = ptrIndex->CreateIndex(Table_name, index_Attribute);
		if(flag==0){
			cout<<"Creating index fails."<<endl;
		}else{
			cout<<"Created index succeed!"<<endl;
		}
		
		//Add index in catalog "Indexs"
		flag_bool = ptrCatalog->AddIndex(Table_name, FindIndex);
	}
	return;
}

//Drop an index of the givingattribute
void API::API_DropIndex(string Table_name, string index_Attribute){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		
		//Find whether there's an index of the giving set of attributes
		int i, flag; 
		bool flag_bool;
		if(!ptrCatalog->WhetherIndex(Table_name, index_Attribute)){
			cout<<"There's no index of this attribute."<<endl;
			return;
		}else{
			//Drop index
			flag = ptrIndex->DropIndex(Table_name, index_Attribute);
			if(flag==0){
				cout<<"Failed to drop index of these attributes."<<endl;
			}else{
				cout<<"Drop index succeed!"<<endl;
			}
			
			//Drop inde in catalog "Indexs"
			flag_bool = ptrCatalog->DeleteIndex(Table_name, index_Attribute);
		}
	}
	return;
}

//Show all indexs of this table
void API::Show_Index(string Table_name){
	if(!ptrCatalog->FindTable(Table_name)){
		cout<<"There's not a table named "<<Table_name<<endl; 
	}else{
		bool flag;
		flag = ptrCatalog->ShowIndex(Table_name);
		if(!flag){
			cout<<"Failed to show all indexs in this table."<<endl;
		}else{
			cout<<"Show index succeed!"<<endl;
		}
	}
	return;
} 








