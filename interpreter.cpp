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
	while (quit != true && errorAbort != true)
	{
		GetCommand();
		while (!commands.empty())
		{
			ParseCommand();
			DisplayResult();
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
			PrintErrorMsg("execfile can only receive one extra argument.");
		string &filename = words[1];
		ifstream file_in(filename);
		// 判断文件是否存在
		if (!file_in)
			PrintErrorMsg("Fail to open target file.");
		while (file_in.eof())
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
				for (int i = 4; i < words.size(); )
				{
					string column_name = words[i];
					i += 1;
					// 查重
					if (ct.columns.find(column_name) != ct.columns.end())
						PrintErrorMsg("Duplicate column names: " + column_name);

					string column_type = words[i];
					i += 1;
					// 判断 primary key
					if (column_name == "primary" && column_type == "key")
					{
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
			}
		}
		else if (words[1] == "index")
		{
			CreateIndexStmt ci;
			ci.indexName = words[2];
			if (words[3] != "on")
				PrintErrorMsg("Expected 'on', now " + words[3]);
			else if (words[5] != "(")
				PrintErrorMsg("Expected '(', now " + words[5]);
			else if (words[7] != ")")
				PrintErrorMsg("Expected ')', now " + words[7]);
			ci.tableName = words[4];
			ci.columnName = words[6];
			if (words.size() > 8)
				PrintErrorMsg("Can't parse " + words[8]);
		}
	}
	else if (words[0] == "drop")
	{
		if (words[1] == "table")
		{
			if (words.size() > 3)
				PrintErrorMsg("Can't parse " + words[3]);
			DropTableStmt dt;
			dt.tableName = words[2];
		}
		else if (words[1] == "index")
		{
			if (words.size() > 3)
				PrintErrorMsg("Can't parse " + words[3]);
			DropIndexStmt di;
			di.indexName = words[2];
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
				PrintErrorMsg("Can't parse " + words[3]);
			if (words[4] != "(")
				PrintErrorMsg("Can't parse " + words[4]);
			for (int i = 5; i < words.size(); )
			{
				ValueStruct value = getValue(words[i]);
				is.data.push_back(value);
				i += 2;
			}
		}
		else
			PrintErrorMsg("Can't parse " + words[1]);
	}
	else if (words[0] == "delete")
	{
		DeleteFromStmt df;
		if (words[1] != "from")
			PrintErrorMsg("Can't parse " + words[1]);
		df.tableName = words[2];
		if (words.size() > 3)
		{
			if (words[3] != "where")
				PrintErrorMsg("Expected 'where', now " + words[3]);
			else
			{
				df.hasWhereClause = true;
				for (int i = 4; i < words.size(); )
				{
					Condition cond;
					cond.columnName = words[i++];
					cond.opType = cond.getOpType(words[i++]);
					if (cond.opType == cond.unknown)
						PrintErrorMsg("Expected an operator, now " + words[i]);
					cond.value = getValue(words[i++]);
					if (i < words.size())
					{
						if (words[i] == "and")
							i++;
						else
							PrintErrorMsg("Expected 'and', now " + words[i]);
					}
					df.whereClause.push_back(cond);
				}
			}
		}
		else 
		{
			df.hasWhereClause = false;
		}
	}
	else if (words[0] == "select")
	{
		SelectStmt sl;
		if (words[1] != "*")
			PrintErrorMsg("Expected '*', now " + words[1]);
		if (words[2] != "from")
			PrintErrorMsg("Expected 'from', now " + words[2]);
		sl.fromClause.push_back(words[3]);
		if (words.size() > 4)
		{
			if (words[4] != "where")
				PrintErrorMsg("Expected 'where', now " + words[4]);
			else
			{
				sl.hasWhereClause = true;
				for (int i = 5; i < words.size(); )
				{
					Condition cond;
					cond.columnName = words[i++];
					cond.opType = cond.getOpType(words[i++]);
					if (cond.opType == cond.unknown)
						PrintErrorMsg("Expected an operator, now " + words[i]);
					cond.value = getValue(words[i++]);
					if (i < words.size())
					{
						if (words[i] == "and")
							i++;
						else
							PrintErrorMsg("Expected 'and', now " + words[i]);
					}
					sl.whereClause.push_back(cond);
				}
			}
		}
		else
		{
			sl.hasWhereClause = false;
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
	cout << "[Error] Command: " << current_command.text << "\n\tReason: " << message << endl;
}

// 工具函数
Condition::operationType Condition::getOpType(std::string input)
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

vector<string> Command::Split(string instruction)
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
			PrintErrorMsg("Invalid input " + typestr + "(" + count_str + "). Expected an integer.");
		else
		{
			return char_length;
		}
	}
}

ValueStruct Interpreter::getValue(string input)
{
	ValueStruct value;
	if (input[0] == '\'' && input[input.size() - 1] == '\'')
	{
		string char_n = input.substr(1, input.size() - 2);
		value.TYPE = char_n.size();
		value.CHAR_N = char_n;
	}
	else if (input.find('.') != string::npos)
	{
		value.TYPE = TYPE_FLOAT;
		value.FLOAT = std::stod(input);
	}
	else
	{
		value.TYPE = TYPE_INT;
		value.INT = std::stoi(input);
	}
	return value;
}