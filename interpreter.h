#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <map>

#ifndef TYPE_FLOAT
#define TYPE_FLOAT 256
#endif // !FLOAT
#ifndef TYPE_INT
#define TYPE_INT 0
#endif // !INT



class CommandFormatException : std::exception {};
class OpenFileException : std::exception {};
class OutOfBoundsException : std::exception {};

// 0: int, n=1-255: char(n), 256:float
typedef int ValueType;

/// Uses a union to represent three data types.
struct ValueStruct
{
	std::string CHAR_N;
	int INT;
	double FLOAT;
	ValueType TYPE;
};

struct Condition
{
	std::string columnName;
	enum operationType
	{
		greater = 0,
		greater_or_equ,
		less,
		less_or_equ,
		equal,
		not_equ,
		unknown
	} opType;
	/// uses template. Not sure whether it works.
	ValueStruct value;
	operationType getOpType(std::string input);
};

struct SelectStmt
{
	/// whether the statement has a "where ..." clause
	bool hasWhereClause;
	/// Not used. Designed for further extensibility
	std::list<std::string> targetList;
	/// Only one element will be inside. Designed as a 'list' for further extensibility
	std::list<std::string> fromClause;
	/// Only "and" supported, but easy to extend
	std::list<Condition> whereClause;
	/// Not used. Designed for further extensibility
	std::list<std::string> groupbyClause;
	/// Not used. Designed for further extensibility
	std::list<Condition> havingClause;
};

struct CreateTableStmt
{
	/// Table name
	std::string tableName;
	/// The columns
	std::map<std::string, ValueType> columns;
	/// uniqueness
	std::map<std::string, bool> unique;
	/// Only one element will be inside. Designed as a 'list' for further extensibility
	std::list<std::string> primaryKeys;
	/// Not used. Designed for further extensibility
	std::list<std::string> foreignKeys;
};

struct DropTableStmt
{
	/// table name
	std::string tableName;
};

struct CreateIndexStmt
{
	/// index name
	std::string indexName;
	/// table name
	std::string tableName;
	/// column name
	std::string columnName;
};

struct DropIndexStmt
{
	/// index name
	std::string indexName;
};

struct InsertStmt
{
	/// table name
	std::string tableName;
	/// Only one list will be inside. Designed as a 'list<list>' for further extensibility
	std::vector<ValueStruct> data;
};

struct DeleteFromStmt
{
	bool hasWhereClause;
	/// table name
	std::string tableName;
	/// Only "and" supported, but easy to extend
	std::list<Condition> whereClause;
};

struct Command
{
	std::string text;
	std::vector<std::string> words;
	enum Type
	{
		CREATE_TABLE = 0,
		DROP_TABLE,
		CREATE_INDEX,
		DROP_INDEX,
		SELECT,
		INSERT,
		DELETE,
		QUIT,
		EXECFILE
	}commandType;

	Command(std::string _text)
	{
		text = _text;
		words = Split(_text);
	}

	Command() {	}

	std::vector<std::string> Split(std::string instruction);
};

class Interpreter
{
public:
	Interpreter();			// 默认构造函数
	void Interact();
private:
	void GetCommand();		// 在main中获取语句
	void ParseCommand();	// 分析语句
	void DisplayResult();	// 返回结果
	void ExecuteScript();	// 执行脚本
	void PrintErrorMsg(std::string message);	// 打印错误信息

	// 工具函数
	ValueType getType(std::string typestr, std::string count_str);
	ValueStruct getValue(std::string input);
	bool quit;				// 是否退出
	bool errorAbort;		// 是否遇到错误
	char delimiter;	// 分隔符，默认为';'
	std::queue<Command> commands;	// 储存command
	Command current_command;

	enum VarType
	{
		FLOAT,
		VARCHAR,
		INT
	};
};