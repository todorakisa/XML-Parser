#ifndef XPATH_CPP_
#define XPATH_CPP_

#include "xpath.h"
#include "elementXML.h"
#include <iostream>
#include <string>

XPath::XPath():elems(nullptr),size(0){};

XPath::XPath(elementXML* elem_){
	size = 1;
	elems = new elementXML[1];
	elems[0] = *elem_;
}

void XPath::oper_children(std::string name){
	int new_size = 0;
	elementXML* new_elems = nullptr;
	for (int i = 0; i < size; ++i)
	{
		if(elems[i].get_size_children() > 0){
			elementXML* children = elems[i].get_children();
			for (int j = 0; j < elems[i].get_size_children(); ++j)
			{	
				if(name.compare(children[j].get_name()) == 0){
					elementXML* new_new_elems = new elementXML[new_size+1];
					for (int h = 0; h < new_size; ++h)
					{
						new_new_elems[h] = new_elems[h];
					}
					new_size++;
					new_new_elems[new_size-1] = children[j];
					delete[] new_elems;
					new_elems = new_new_elems;
				}
			}
		}	
	}
	delete[] elems;
	size = 0;
	elems = new_elems;
	size = new_size;
}

void XPath::oper_index(int n){
	if(n<size){
		elementXML* new_elem = new elementXML[1];
		new_elem[0] = elems[n];
		delete[] elems;
		elems = new_elem;
		size = 1;
	}	
}

void XPath::oper_a(std::string str){
	key = str;
}

void XPath::execute(std::string command){
	std::string name_of_elem;
	for (int i = 0; i < command.size(); ++i)
	{
		if(command.at(i) == '/' || command.at(i) == '(' || command.at(i) == '[' || i+1 == command.size()){
			if(i+1 == command.size()){
				name_of_elem = command.substr(0,i+1);
				command = command.substr(i+1);
			}else{
				name_of_elem = command.substr(0,i);
				command = command.substr(i);
			}
			oper_children(name_of_elem);
			break;
		}
	}
	while(true){
		if(command.size() == 0) return;
		char tmp = command[0];
		command = command.substr(1);
		switch(tmp){
			case '/':
				for (int z = 0; z < command.size(); ++z)
				{
					if(command.at(z) == '/' || command.at(z) == '(' || command.at(z) == '[' || z+1 == command.size()){
						if(z+1 == command.size()){
							name_of_elem = command.substr(0,z+1);
							command = command.substr(z+1);
						}else{
							name_of_elem = command.substr(0,z);
							command = command.substr(z);
						}
						oper_children(name_of_elem);
						break;
					}
				}
				break;
			case '(':
				for (int z = 0; z < command.size(); ++z)
				{
					if(command.at(z) == ')' || z+1 == command.size()){

						if(z+1 == command.size()){
							name_of_elem = command.substr(1,z-1);
							command = command.substr(z+1);
						}else{
							name_of_elem = command.substr(1,z-1);
							command = command.substr(z);
						}
						oper_a(name_of_elem);
						break;
					}
				}
				break;
			case '[':
				for (int z = 0; z < command.size(); ++z)
				{
					if(command.at(z) == ']' || z+1 == command.size()){
						name_of_elem = command.substr(0,z);
						if(z+1 == command.size()){
							command.clear();
						}else{
							command = command.substr(z);
						}
						oper_index(stoi(name_of_elem));
						break;
					}
				}
				break;
		}
	}
}

void XPath::print(){
	std::cout << "RESULT" << std::endl;
	if(key.size() > 0){
		for (int i = 0; i < size; ++i)
		{
			if(elems[i].has_key(key)){
				std::cout << "\"" << key << "\":" <<elems[i].find_atr_by_key(key)->get_value() << std::endl;
			}
		}
	}else{
		for (int i = 0; i < size; ++i)
		{
			std::cout << "Name of elem:" << elems[i].get_name() << std::endl; 
			std::cout << "Id of elem:" << elems[i].get_id() << std::endl; 
		}
	}
}

XPath::~XPath(){
	delete[] elems;
	size = 0;
}

#endif