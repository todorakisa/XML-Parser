#ifndef ELEMENTXML_H_
#define ELEMENTXML_H_

#include <iostream>
#include <string>
#include "attribute.h"

class elementXML{
	std::string name;
	std::string text;
	attribute* atr;
	elementXML* children;
	int size_atr;
	int size_children;

	void copy(const elementXML& other);

public:
	elementXML();
	elementXML(const elementXML& other);
	elementXML& operator=(const elementXML& other);
	void set_up(char* init);
	void add_child(const elementXML& other);
	attribute* find_atr_by_key(std::string key);
	void delete_atr_by_key(std::string key);
	bool has_key(std::string key);
	void change_atr_value(std::string key, std::string value);
	std::string get_id();
	void set_text(std::string text_);
	std::string get_text();
	bool have_text();
	void add_atr(std::string key,std::string value);
	void printAll(int spaces = 0);
	std::string get_name();
	attribute* get_attributes();
	elementXML* get_children();
	int get_size_attributes();
	int get_size_children();
	~elementXML();
};

#endif