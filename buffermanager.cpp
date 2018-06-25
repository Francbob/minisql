//
// Created by bob on 2018/6/23.
//
/*
* 现在需要处理的bug:
* 1. 构造函数应该读取已有的数据
* 2. 编号 文件的读写应该用个编号数组来掌握
*  读的时候找名字
*  写的时候找名字
*/
#include <fstream>
#include "buffermanager.h"
testCM tcm;
static const int MAX_BLOCK = 4096;
static const int MAX_BUFFER = 100;
bool buffermanager::changeTB(string TB) {
	table = TB;
	return File_manager.setTable(TB);
}

bool buffermanager::changeDB(string DB) {
	database = DB;
	File_manager.setDB(DB);
}
int buffermanager::insert_block(int index, const string& block) {
	int target = find_name(index);
	Buffer.buffer[target] = block;
	Buffer.blockMap[target] = true;
	Buffer.frequency[target]++;
	Buffer.dirty[target] = true;
	return true;
}
int buffermanager::insert_block(string block) {
	for (int i = 0; i < Buffer.buffer.size(); ++i) {
		if (Buffer.blockMap[i] == false)
		{
			Buffer.buffer[i] = block;
			Buffer.blockMap[i] = true;
			Buffer.fullMap[i] = true;
			Buffer.frequency[i] = 1;
			Buffer.dirty[i] = true;

			return Buffer.name[i];
		}
	}
	if (Buffer.buffer.size() < MAX_BUFFER) {
		Buffer.buffer.push_back(block);
		Buffer.blockMap.push_back(true);
		Buffer.fullMap.push_back(true);
		Buffer.frequency.push_back(1);
		Buffer.dirty.push_back(true);
		Buffer.name.push_back(tcm.get() + 1);
		tcm.add();
		return Buffer.name[Buffer.buffer.size() - 1];
	}
	int target = LRU();
	write2disk(target);
	Buffer.buffer[target] = block;
	Buffer.blockMap[target] = true;
	Buffer.frequency[target] = 1;
	Buffer.fullMap[target] = true;
	Buffer.dirty[target] = true;
	Buffer.name[target] = tcm.get() + 1;
	tcm.add();
	return Buffer.name[target];
}

int buffermanager::LRU() {
	if (Buffer.buffer.size() <= MAX_BUFFER)
		return -1;
	int target = 0;
	for (int i = 0; i < Buffer.buffer.size(); ++i) {
		if (Buffer.frequency[i] < Buffer.frequency[target])
			target = i;
	}
	return target;
}
//??
int buffermanager::insert_piece(string piece) {
	for (int i = 0; i < Buffer.buffer.size(); ++i) {
		if (Buffer.blockMap[i] && !Buffer.fullMap[i])
		{
			if (Buffer.buffer[i].size() + piece.size() > MAX_BLOCK)
				continue;
			if (Buffer.buffer[i].size() + piece.size() == MAX_BLOCK)
				Buffer.fullMap[i] = true;
			Buffer.buffer[i].append(piece);
			Buffer.dirty[i] = true;
			Buffer.frequency[i]++;
			return i;
		}
	}
	if (Buffer.buffer.size() < MAX_BUFFER) {
		Buffer.buffer.push_back(piece);
		Buffer.blockMap.push_back(true);
		Buffer.fullMap.push_back(false);
		Buffer.frequency.push_back(1);
		Buffer.dirty.push_back(true);
		Buffer.name.push_back(tcm.get() + 1);
		tcm.add();
		return Buffer.buffer.size() - 1;
	}
	int target = LRU();
	write2disk(target);
	Buffer.buffer.push_back(piece);
	Buffer.blockMap.push_back(true);
	Buffer.fullMap.push_back(false);
	Buffer.frequency.push_back(1);
	Buffer.dirty.push_back(true);
	Buffer.name.push_back(tcm.get() + 1);
	tcm.add();
	return target;
}

bool buffermanager::write2disk(int index) {
	if (!Buffer.dirty[index])
		return true;
	string path = File_manager.get_path(Buffer.name[index]);
	ofstream fptr;
	fptr.open(path);
	fptr << Buffer.buffer[index] << endl;
	fptr.close();
	Buffer.dirty[index] = false;
	return true;
}

bool buffermanager::Allwrite2disk() {
	for (int i = 0; i < Buffer.dirty.size(); ++i) {
		write2disk(i);
	}
	return true;
}

string buffermanager::get_block(int index) {
	for (int i = 0; i < Buffer.buffer.size(); ++i) {
		if (Buffer.name[i] == index)
		{
			if (!Buffer.blockMap[i])
				return nullptr;
			Buffer.frequency[i]++;
			return Buffer.buffer[i];
		}
	}
	if (index > tcm.get())
		return nullptr;
	int target = LRU();
	if (target == -1) {
		target = Buffer.buffer.size();
		Buffer.buffer.push_back(get_e_block(index));
		Buffer.blockMap.push_back(true);
		Buffer.frequency.push_back(1);
		Buffer.fullMap.push_back((Buffer.buffer.end() - 1)->size() >= MAX_BLOCK);
		Buffer.dirty.push_back(false);
		Buffer.name.push_back(index);
		return Buffer.buffer[target];
	}
	write2disk(target);
	Buffer.buffer[target] = get_e_block(index);
	Buffer.blockMap[target] = true;
	Buffer.frequency[target] = 1;
	Buffer.fullMap[target] = (Buffer.buffer[target].size() >= MAX_BLOCK);
	Buffer.dirty[target] = false;
	Buffer.name[target] = index;
	return Buffer.buffer[target];
}

int buffermanager::read_disk() {
	int num = tcm.get();
	if (num >= MAX_BUFFER)
		num = MAX_BUFFER;
	for (int i = 1; i <= num; ++i) {
		get_block(i);
	}
}

string buffermanager::get_next_block(int index) {
	if (index > tcm.get())
		return nullptr;
	else
		return get_block(index + 1);
}

int buffermanager::find_name(int name) {

	for (int i = 0; i < Buffer.name.size(); ++i) {
		if (Buffer.name[i] == name)
			return i;
	}
	if (name > tcm.get())
		return 0;
	int target = LRU();
	if (target == -1) {
		target = Buffer.buffer.size();
		Buffer.buffer.push_back(get_e_block(name));
		Buffer.blockMap.push_back(true);
		Buffer.frequency.push_back(1);
		Buffer.fullMap.push_back((Buffer.buffer.end() - 1)->size() >= MAX_BLOCK);
		Buffer.dirty.push_back(false);
		Buffer.name.push_back(name);
		return target;
	}
	write2disk(target);
	Buffer.buffer[target] = get_e_block(name);
	Buffer.blockMap[target] = true;
	Buffer.frequency[target] = 1;
	Buffer.fullMap[target] = (Buffer.buffer[target].size() >= MAX_BLOCK);
	Buffer.dirty[target] = false;
	Buffer.name[target] = name;
	return target;

}