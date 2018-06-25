#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <cctype>

using namespace std;

Interpreter::Interpreter()
{
	delimiter = ';';
	quit = false;
}

void Interpreter::Interact()
{
	while (quit != true)
	{
		GetCommand();
		while (!commands.empty())
		{
			ParseCommand();
			if (errorAbort == true)
				while (!commands.empty()) commands.pop();
			errorAbort = false;
		}
	}
}

void Interpreter::GetCommand()
{
	string buffer;
	std::getline(cin, buffer, delimiter);
	commands.push(Command(buffer));
}

void Interpreter::ParseCommand()
{
	// variables
	string tableName;
	vector<string> varNames;
	vector<VarType> varTypes;
	vector<unsigned int> VarcharLengths;

	current_command = commands.front();
	commands.pop();
	vector<string> &words = current_command.words;
	/// if input quit, change variable 'quit' to true and return.
	if (words[0] == "quit")
	{
		quit = true;
		return;
	}
	else if (words[0] == "execfile") // exec script
	{
		// 判断参数是否合法
		if (words.size() > 2)
		{
			PrintErrorMsg("execfile can only receive one extra argument.");
			return;
		}
		string &filename = words[1];
		ifstream file_in(filename);
		// 判断文件是否存在
		if (!file_in.is_open())
		{
			PrintErrorMsg("Fail to open target file.");
			return;
		}
		while (!file_in.eof())
		{
			string buffer;
			getline(file_in, buffer, delimiter);
			commands.push(Command(buffer));
		}
	}
	else if (words[0] == "create")
	{
		if (words[1] == "table") // create table [tablename] ([name] [type], ..., primary key [name][type]))
		{
			tableName = words[2];
			if (words[3] != "(") // wrong format
			{
				PrintErrorMsg("Expected '(', received " + words[3]);
				return;
			}
			else
			{
				CreateTableStmt ct;
				ct.tableName = tableName;
				for (size_t i = 4; i < words.size(); )
				{
					string column_name = words[i];
					i += 1;
					// 查重
					if (ct.columns.find(column_name) != ct.columns.end())
					{
						PrintErrorMsg("Duplicate column names: " + column_name);
						return;
					}

					string column_type = words[i];
					i += 1;
					// 判断 primary key
					if (column_name == "primary" && column_type == "key")
					{
						// 查重
						if (find(ct.primaryKeys.begin(), ct.primaryKeys.end(), words[i + 1]) != ct.primaryKeys.end())
						{
							PrintErrorMsg("Detected duplicate primary key " + words[i + 1]);
							return;
						}
						else
							ct.primaryKeys.push_back(words[i + 1]);
						i += 4;
					}
					// 如果是正常的column
					else
					{
						ValueType type = getType(column_type, words[i + 1]);
						// 如果类型是char(n)
						if (type >= 1 && type <= 255)
							i += 3;
						if (words[i] == "unique")
						{
							i++;
							ct.unique[column_name] = true;
						}
						ct.columns[column_name] = type;
						i += 1;
					}
				}
				vector<Attribute> attributes;
				for (auto i : ct.columns)
				{
					attributes.push_back(Attribute(i.first, i.second, ct.unique.find(i.first) != ct.unique.end()));
				}
				api.API_CreateTable(ct.tableName, attributes, ct.primaryKeys);
			}
		}
		else if (words[1] == "index")
		{
			CreateIndexStmt ci;
			ci.indexName = words[2];
			if (words[3] != "on")
			{
				PrintErrorMsg("Expected 'on', now " + words[3]);
				return;
			}
			else if (words[5] != "(")
			{
				PrintErrorMsg("Expected '(', now " + words[5]);
				return;
			}
			else if (words[7] != ")")
			{
				PrintErrorMsg("Expected ')', now " + words[7]);
				return;
			}
			ci.tableName = words[4];
			ci.columnName = words[6];
			if (words.size() > 8)
			{
				PrintErrorMsg("Can't parse " + words[8]);
				return;
			}
			api.API_CreateIndex(ci.tableName, ci.columnName, ci.indexName);
		}
	}
	else if (words[0] == "drop")
	{
		if (words[1] == "table")
		{
			if (words.size() > 3)
			{
				PrintErrorMsg("Can't parse " + words[3]);
				return;
			}
			DropTableStmt dt;
			dt.tableName = words[2];
			api.API_DropTable(dt.tableName);
		}
		else if (words[1] == "index")
		{
			if (words.size() > 3)
			{
				PrintErrorMsg("Can't parse " + words[3]);
				return;
			}
			DropIndexStmt di;
			di.indexName = words[2];
			api.API_DropIndex(di.indexName);
		}
	}
	else if (words[0] == "insert")
	{
		if (words[1] == "into")
		{
			InsertStmt is;
			is.tableName = words[2];
			int i = 0;
			if (words[3] != "values")
			{
				PrintErrorMsg("Can't parse " + words[3]);
				return;
			}
			if (words[4] != "(")
			{
				PrintErrorMsg("Can't parse " + words[4]);
				return;
			}
			for (size_t i = 5; i < words.size(); )
			{
				ValueStruct value = ValueStruct(words[i]);
				is.data.push_back(value);
				i += 2;
			}
			api.API_Insert(is.tableName, is.data); // is.data: vector<ValueStruct>
		}
		else
		{
			PrintErrorMsg("Can't parse " + words[1]);
			return;
		}
	}
	else if (words[0] == "delete")
	{
		DeleteFromStmt df;
		if (words[1] != "from")
		{
			PrintErrorMsg("Can't parse " + words[1]);
			return;
		}
		df.tableName = words[2];
		if (words.size() > 3)
		{
			if (words[3] != "where")
			{
				PrintErrorMsg("Expected 'where', now " + words[3]);
				return;
			}
			else
			{
				df.hasWhereClause = true;
				for (size_t i = 4; i < words.size(); )
				{
					condition cond;
					cond.Attribute_name = words[i++];
					cond.type_compare = cond.getOpType(words[i++]);
					if (cond.type_compare == cond.unknown)
					{
						PrintErrorMsg("Expected an operator, now " + words[i]);
						return;
					}
					cond.value = ValueStruct(words[i++]);
					if (i < words.size())
					{
						if (words[i] == "and")
							i++;
						else
						{
							PrintErrorMsg("Expected 'and', now " + words[i]);
							return;
						}
					}
					df.whereClause.push_back(cond);
				}
			}
			api.API_Delete(df.tableName, df.whereClause);
		}
		else
		{
			df.hasWhereClause = false;
			api.API_Delete(df.tableName);
		}
	}
	else if (words[0] == "select")
	{
		SelectStmt sl;
		if (words[1] != "*")
		{
			PrintErrorMsg("Expected '*', now " + words[1]);
			return;
		}
		if (words[2] != "from")
		{
			PrintErrorMsg("Expected 'from', now " + words[2]);
			return;
		}
		sl.fromClause.push_back(words[3]);
		if (words.size() > 4)
		{
			if (words[4] != "where")
			{
				PrintErrorMsg("Expected 'where', now " + words[4]);
				return;
			}
			else
			{
				sl.hasWhereClause = true;
				for (size_t i = 5; i < words.size(); )
				{
					condition cond;
					cond.Attribute_name = words[i++];
					cond.type_compare = cond.getOpType(words[i++]);
					if (cond.type_compare == cond.unknown)
					{
						PrintErrorMsg("Expected an operator, now " + words[i]);
						return;
					}
					cond.value = ValueStruct(words[i++]);
					if (i < words.size())
					{
						if (words[i] == "and")
							i++;
						else
						{
							PrintErrorMsg("Expected 'and', now " + words[i]);
							return;
						}
					}
					sl.whereClause.push_back(cond);
				}
			}
			api.API_SelectAll(sl.fromClause[0], sl.whereClause);
		}
		else
		{
			sl.hasWhereClause = false;
			api.API_SelectAll(sl.fromClause[0]);
		}
	}
}

void Interpreter::DisplayResult()
{
	if (errorAbort == true)
		cout << "Execute failed due to previous error(s)" << endl;
	if (quit == true)
		cout << "Bye~" << endl;
	else
	{

	}
}

void Interpreter::ExecuteScript()
{

}

void Interpreter::PrintErrorMsg(string message)
{
	errorAbort = true;
	cout << "[Error] Command: " << current_command.text << "\n\tReason: " << message << endl;
}

vector<string> Interpreter::Split(string instruction)
{
	// 返回的词列表
	set<char> stopwords = { ' ', '\t', '\n', ';' };
	set<char> singles = { ',', '(', ')' };
	vector<string> words;
	// splitted用于维护是否切断
	bool shouldsplit = false;
	// 定义并初始化buffer
	char buffer[200];
	int buffercnt = 0;
	memset(buffer, 0, sizeof(buffer) / sizeof(char));
	for (auto i : instruction)
	{
		if (!isascii(i))
		{
			string errorMsg = "MiniSQL only supports ASCII chars.";
			PrintErrorMsg(errorMsg);
			return vector<string>();
		}
		if (stopwords.find(i) != stopwords.end())
		{
			if (shouldsplit == true)
			{
				words.push_back(string(buffer));
				memset(buffer, 0, sizeof(buffer) / sizeof(char));
				buffercnt = 0;
				shouldsplit = false;
			}
			else {}
		}
		else if (singles.find(i) != singles.end())
		{
			if (shouldsplit)
				words.push_back(string(buffer));
			// 将char转换成string的奇技淫巧
			words.push_back(string(1, i));
			memset(buffer, 0, sizeof(buffer) / sizeof(char));
			buffercnt = 0;
			shouldsplit = false;
		}
		else
		{
			string tmp;
			buffer[buffercnt++] = i;
			shouldsplit = true;
		}
	}

	// 如果buffer里还有东西，最后也写进words
	if (buffer[0] != 0 && shouldsplit)
		words.push_back(string(buffer));

	return words;
}

ValueType Interpreter::getType(string typestr, string count_str)
{
	int char_length;
	if (typestr == "int")
		return 0;
	if (typestr == "float")
		return 256;
	if (typestr == "char")
	{
		// 确认输入是否为整数
		for (auto i : count_str)
			if (i < '0' || i > '9')
			{
				PrintErrorMsg("Invalid input " + typestr + "(" + count_str + "). Expected an integer.");
				return -1;
			}
		char_length = std::stoi(count_str);
		if (char_length <= 0)
		{
			PrintErrorMsg("Invalid input " + typestr + "(" + count_str + "). Expected an integer.");
			return -1;
		}
		else
		{
			return char_length;
		}
	}
}