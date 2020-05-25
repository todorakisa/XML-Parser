#ifndef FILEXML_H_
#define FILEXML_H_

#include "elementXML.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

class FileXML{
	bool filled;
	std::string name;
	elementXML* tree;

	void _clearContent();

public:
	FileXML();
	void set_name(std::string name_);
	std::string get_name();
	bool is_filled();
	bool fill_content();
	void setup_ids();
	void rec_id_setup(std::vector<std::string> &ids, elementXML* elem, int index);
	void empty_content();
	elementXML* find_elem_by_id(std::string id,elementXML* start = nullptr);
	elementXML* get_n_child(std::string id, int n);
	void print();
	void save_as(std::string name_);
	void write_in_file(std::ofstream& stream, elementXML* elem, int spaces);
	~FileXML();
};

#endif