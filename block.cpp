//
// Created by bob on 2018/6/23.
//

#include "block.h"
#include <cassert>
#include <sstream>
#include <iostream>
#include "buffermanager.h"
const int MAX_SIZE = 4096;
static block* nullblock = new block();
const int EXAMPLE = 0;
/*
* Constructor for new block
* !**********stupid design, I think it will be modified letter**********!
*/
block::block(int index_, int size_, int attr_n, block::attribute info)
	: index(index_),
	size(size_),
	attr_num(attr_n)
{
	bitmap = new bool[size];
	for (int i = 0; i < size; ++i) {
		//at the very beginning, all empty
		bitmap[i] = true;
	}

	attr_info = new ATTR[attr_num];
	for (int j = 0; j < attr_num; ++j) {
		attr_info[j] = ATTR(info[j].attr_len, info[j].attr_offset);
	}

	unsigned int data_space = 0;
	for (int k = 0; k < attr_num; ++k) {
		data_space += attr_info[k].attr_len;
	}
	data_space *= size;
	data_.append(data_space, '0');
}
/*
* Constructor to read data from existed block
*/
block::block(int index_, const string &TB, const string &get) {
	string line;
	stringstream fptr;
	fptr << get;
	//analyse data scale in the block file
	getline(fptr, line, ' ');
	size = stoi(line);

	//get the empty bitmap
	bitmap = new bool[size];
	char c;
	for (int j = 0; j < size; ++j) {
		fptr >> c;
		switch (c) {
		case '0': bitmap[j] = false;
			break;
		case '1': bitmap[j] = true;
			break;
		default:
			break;
		}
	}
	fptr >> attr_num;
	getline(fptr, line, ' ');
	//attr_num = stoi(line);
	//analyse attribute information
	attr_info = new ATTR[attr_num];
	for (int i = 0; i < attr_num; ++i) {
		getline(fptr, line, ' ');
		attr_info[i].attr_len = stoi(line);
		getline(fptr, line, ' ');
		attr_info[i].attr_offset = stoi(line);
	}

	//read the content data
	while (!fptr.eof()) {
		fptr >> c;
		data_ += c;
	}
}
block::block(int index_, const string &TB, buffermanager& bm)
	: index(index_)
{
	string line;
	if (bm.get_table() != TB) {
		bm.changeTB(TB);
	}
	//use string stream to make code simple
	string get = bm.get_block(index_);
	stringstream fptr;
	fptr << get;
	//analyse data scale in the block file
	getline(fptr, line, ' ');
	size = stoi(line);

	//get the empty bitmap
	bitmap = new bool[size];
	char c;
	for (int j = 0; j < size; ++j) {
		fptr >> c;
		switch (c) {
		case '0': bitmap[j] = false;
			break;
		case '1': bitmap[j] = true;
			break;
		default:
			break;
		}
	}
	fptr >> attr_num;
	getline(fptr, line, ' ');
	//attr_num = stoi(line);
	//analyse attribute information
	attr_info = new ATTR[attr_num];
	for (int i = 0; i < attr_num; ++i) {
		getline(fptr, line, ' ');
		attr_info[i].attr_len = stoi(line);
		getline(fptr, line, ' ');
		attr_info[i].attr_offset = stoi(line);
	}

	//read the content data
	while (!fptr.eof()) {
		fptr >> c;
		data_ += c;
	}
}

block::block(int index_, int attr_n, int *attr_len)
	: index(index_),
	attr_num(attr_n)
{
	//calculate the size
	int space_per_record = sizeof(char);
	for (int i = 0; i < attr_n; ++i) {
		space_per_record += attr_len[i];
	}
	int data_space = space_per_record - 1;

	int free_space = MAX_SIZE - sizeof(int)*(2 + attr_n * 2);
	//roughly calculating
	size = free_space / space_per_record;
	//??????????????????????????????????
	data_.append(data_space*size, '0');
	//space allocate
	bitmap = new bool[size];
	attr_info = new ATTR[attr_n];
	for (int j = 0; j < size; ++j) {
		bitmap[j] = true;
	}
	int base_index = 0;
	for (int k = 0; k < attr_n; ++k) {
		attr_info[k].attr_len = attr_len[k];
		attr_info[k].attr_offset = base_index;
		base_index += attr_info[k].attr_len*size;
	}
}

void block::show_record(int record_index) const {
	assert(!bitmap[record_index]);

	for (int i = 0; i < attr_num; ++i) {
		cout << get_attr(record_index, i) << " | ";
	}
	cout << endl;
}

string block::get_attr(int record_index, int attr_index) const
{
	assert(!bitmap[record_index]);
	auto pos = (unsigned)(attr_info[attr_index].attr_offset
		+ record_index * attr_info[attr_index].attr_len);
	auto len = (unsigned)(attr_info[attr_index].attr_len);
	string ai = data_.substr(pos, len);
	return ai;
}

bool block::insert_record(const string& record)
{
	//find a place to insert
	int free_index = -1;
	for (int i = 0; i < size; ++i) {
		if (bitmap[i]) {
			free_index = i;
			bitmap[i] = false;
			break;
		}
	}
	if (free_index == -1) {
		return false;
	}
	for (int j = 0; j < attr_num; ++j) {
		insert_attr(j, free_index, record);
	}
	return true;
}

void block::insert_attr(int attr_index, int record_index, const string& record)
{
	int attr_begin = 0;
	for (int i = 0; i < attr_index; ++i) {
		attr_begin += attr_info[i].attr_len;
	}
	auto begin_pos = (unsigned)(attr_info[attr_index].attr_offset
		+ attr_info[attr_index].attr_len*record_index);
	auto len = (unsigned)attr_info[attr_index].attr_len;
	auto substring = record.substr(attr_begin, attr_info[attr_index].attr_len);
	data_.replace(begin_pos, len, substring);
}

//write the block to the disk
int block::write2buffer(buffermanager* bm) {
	stringstream fptr;

	fptr << size << ' ';
	for (int i = 0; i < size; ++i) {
		if (bitmap[i])
			fptr << '1';
		else
			fptr << '0';
	}
	fptr << ' ';
	fptr << attr_num << ' ';
	for (int j = 0; j < attr_num; ++j) {
		fptr << attr_info[j].attr_len << ' ' << attr_info[j].attr_offset << ' ';
	}
	fptr << data_;

	string push;
	while (!fptr.eof())
	{
		string line;
		getline(fptr, line);
		push += line;
	}
	bm->insert_block(index, push);
	return true;
}