#ifndef XPATH_H_
#define XPATH_H_

#include "elementXML.h"
#include <iostream>
#include <string>

class XPath{
	int size;
	elementXML* elems;
	std::string key;
public:
	XPath();
	XPath(elementXML* elem_);
	void oper_children(std::string name);
	void oper_index(int n);
	void oper_a(std::string str);
	void execute(std::string command);
	void print();
	~XPath();
};

#endif