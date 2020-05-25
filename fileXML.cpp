#ifndef FILEXML_CPP_
#define FILEXML_CPP_

#include "fileXML.h"
#include "elementXML.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

void FileXML::_clearContent(){
	delete tree;
	tree = nullptr;
}

FileXML::FileXML():filled(false),tree(nullptr){}

void FileXML::set_name(std::string name_){
	this->name = name_;
};

std::string FileXML::get_name(){
	return name;
}

bool FileXML::is_filled(){
	return filled;
}

bool FileXML::fill_content(){
	if(this->name.size() == 0){
		return filled;
	}
	if(tree!=nullptr){
		_clearContent();
	}
	std::ifstream original_file(this->name);
	if(!original_file){
		std::ofstream new_file(this->name);
		new_file.close();
		filled = false;
		tree = nullptr;
	}else if(original_file.is_open()){		

		char ch;
		std::string text;
		int start_pos;
		int end_pos;
		std::vector<int> child_order;
		int size = 0;
		elementXML* content = nullptr;

		while(original_file.get(ch)){		
			if(ch == '<'){
				start_pos = original_file.tellg();
				original_file.get(ch);
				if(ch == '/'){
					while(original_file.get(ch)){
						if(ch == '>'){
							break;
						}
					};
					if(child_order.size()==1){
						continue;
					}
					elementXML last(content[child_order.back()]);
					child_order.pop_back();
					content[child_order.back()].add_child(last);
					continue;
				}

				while(original_file.get(ch)){
					if(ch == '>'){
						break;
					}
				};
				end_pos = original_file.tellg();
				char* initelem = new char[end_pos - start_pos];
				int index = 0;
				original_file.seekg(start_pos, std::ios::beg);
				while(original_file.get(ch)){
					if(ch == '>'){
						break;
					}else{
						initelem[index] = ch;
						index++;
					}
				};
				initelem[index] = '\0';
				elementXML* new_content = new elementXML[size+1];
				for (int i = 0; i < size; ++i)
				{
					new_content[i] = content[i];
				}
				delete[] content;
				size+=1;
				content = new_content;
				content[size-1].set_up(initelem);
				delete[] initelem;
				child_order.push_back(size-1);
			}
			else if(ch!=' ' && ch !='\n'){
				int critical_position = 0;
				text = text + ch;
				while(original_file.get(ch)){
					if(ch =='\n' || ch == '<'){
						if(ch == '<'){
							original_file.seekg(critical_position, std::ios::beg);
						}
						break;
					}
					critical_position = original_file.tellg();
					text = text + ch;
				}
				content[size-1].set_text(text);
				text.clear();
			}
		};
		filled = true;
		tree = new elementXML();
		*tree = content[0];
		delete[] content;
		setup_ids();
	}else{
		std::cout << "The file cannot be opened" << std::endl;
		original_file.close();
		filled = false;
	}
	original_file.close();
	return filled;
}

void FileXML::setup_ids(){
	std::vector<std::string> ids;
	int i = 0;
	rec_id_setup(ids,tree,0);
}

void FileXML::rec_id_setup(std::vector<std::string> &ids, elementXML* elem, int index){
	bool tmp = true;
	if(elem->has_key("id")){
		for (std::vector<std::string>::iterator it = ids.begin(); it!= ids.end(); ++it)
		{
			if(it->compare(elem->get_id()) == 0){
				tmp = false;
			}
		}
		if(tmp){
			ids.push_back(elem->get_id());
		}else{
			std::string id = elem->get_id();
			elem->delete_atr_by_key("id");
			elem->add_atr("id",id + "_" + std::to_string(index));
			index++;
		}
	}else{
		std::string value = "_" + std::to_string(index);
		for (std::vector<std::string>::iterator it = ids.begin(); it!= ids.end(); ++it)
		{
			if(it->compare(value) == 0){
				while(it->compare(value) == 0){
					index++;
					value = "_" + std::to_string(index);
				}
			}
		}
		elem->add_atr("id",value);
		ids.push_back(value);
		index++;
	}
	if(elem->get_size_children()>0){
		elementXML* children = elem->get_children();
		for (int i = 0; i < elem->get_size_children(); ++i)
		{
			rec_id_setup(ids,&children[i],index);
		}
	}
}

void FileXML::empty_content(){
	filled = false;
	_clearContent();
}

elementXML* FileXML::find_elem_by_id(std::string id, elementXML* start){
	elementXML* result;
	if(start == nullptr){
		result = tree;
	}else{
		result = start;
	}
	
	if(id.compare(result->get_id()) == 0){
		return result;
	}

	elementXML* children = result->get_children();
	for (int i = 0; i < result->get_size_children(); ++i)
	{
		elementXML* result2 = find_elem_by_id(id,&children[i]);
		if(result2 != nullptr){
			return result2;
		}
	}
	return nullptr;
}

elementXML* FileXML::get_n_child(std::string id, int n){
	elementXML* elem = find_elem_by_id(id);
	if(elem == nullptr){
		std::cout << "No element with such id!" << std::endl;
		return elem;
	}
	for (int i = 0; i < n; ++i)
	{
		if(elem->get_size_children() > 0){
			elem = &(elem->get_children()[0]);
		}else{
			return nullptr;
		}
		
	}
	return elem;
}

void FileXML::print(){
	tree->printAll();
}

void FileXML::save_as(std::string name_){
	if(name.compare(name_) == 0){
		char name__[name.size()+1];
		name.copy(name__,name.size()+1);
		name__[name.size()+1] = '\0';
		remove(name__);
	}
	std::ofstream new_file(name_);
	write_in_file(new_file,tree,0);
	new_file.close();
}

void FileXML::write_in_file(std::ofstream& stream, elementXML* elem, int spaces){
	for (int i = 0; i < spaces; ++i){stream << "    ";}stream << "<" << elem->get_name();
	for (int i = 0; i < elem->get_size_attributes(); ++i){
		stream << ' ' << (elem->get_attributes()+ i)->get_key() << "=\"" << (elem->get_attributes()+ i)->get_value() << "\"";
	}
	stream << ">" << std::endl;
	if(elem->have_text()){
		for (int i = 0; i < spaces+1; ++i){stream << "    ";}stream << elem->get_text() << std::endl;
	}
	elementXML* children = elem->get_children();
	for (int i = 0; i < elem->get_size_children(); ++i){
		elementXML* child = &children[i];
		write_in_file(stream,child,spaces+1);
	}
	for (int i = 0; i < spaces; ++i){stream << "    ";}stream << "</" << elem->get_name() <<">" << std::endl;
}

FileXML::~FileXML(){
	_clearContent();
}

#endif