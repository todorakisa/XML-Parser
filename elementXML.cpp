#ifndef ELEMENTXML_CPP_
#define ELEMENTXML_CPP_

#include "elementXML.h"
#include <iostream>
#include <string>

void elementXML::copy(const elementXML& other){
	name = other.name;
	text = other.text;
	size_atr = other.size_atr;
	size_children = other.size_children;
	delete[] atr;
	delete[] children;
	if(other.atr!=nullptr){
		atr = new attribute[other.size_atr];
		for (int i = 0; i < other.size_atr; ++i)
		{
			atr[i] = other.atr[i];
		}
	}else{
		atr = nullptr;
	}
	if(other.children!=nullptr){
		children = new elementXML[other.size_children];
		for (int i = 0; i < other.size_children; ++i)
		{
			children[i] = other.children[i];
		}
	}else{
		children = nullptr;
	}
}

elementXML::elementXML():children(nullptr),atr(nullptr),size_atr(0),size_children(0){}

elementXML::elementXML(const elementXML& other):children(nullptr),atr(nullptr),size_atr(0),size_children(0){
	copy(other);
}

elementXML& elementXML::operator=(const elementXML& other){
	if(this == &other){
		return *this;
	}
	copy(other);
	return *this;
}

void elementXML::set_up(char* init){
	int index = 0;
	int tmp = 0;
	if(init[index]==' '){
		while(init[index]==' ' && init[index]!='\0'){
			index++;
		}
	}
	tmp = index;
	while(init[tmp]!=' ' && init[tmp]!='\0'){
		tmp++;
	}
	std::string name_;
	for (int i = index; i < tmp; ++i)
	{
		name_ = name_ + init[i];			
	}
	index = tmp;
	this->name = name_;
	while(init[index]==' ' && init[index]!='\0'){
		index++;
	}

	while(init[index]!='\0'){
		std::string key_;
		std::string value_;
		
		tmp = index;
		while(init[tmp]!='=' && init[tmp]!='\0' && init[tmp]!=' '){
			tmp++;
		}
		for (int i = index; i < tmp; ++i)
		{
			key_ = key_ + init[i];			
		}
		index = tmp;
		if(init[index] !='=' && init[index]!='\0'){

			while(init[index]==' ' && init[index]!='\0'){
				index++;
			}	
		}
		if(init[index] == '='){
			index++;
		}else{
			std::cout << "The file has wrong syntaxis" << std::endl;
			break;
		}
		if(init[index] == ' '){
			while(init[index]==' ' && init[index]!='\0'){
				index++;
			}	
		}

		if(init[index]!= '\"'){
			std::cout << "The file has wrong syntaxis" << std::endl;
		}else{
			index++;
		}
		tmp = index;
		while(init[tmp]!='\"' && init[tmp]!='\0'){
			tmp++;
		}
		if(init[tmp]!='\"'){
			std::cout << "The file has wrong syntaxis" << std::endl;
			break;
		}
		for (int i = index; i < tmp; ++i)
		{
			value_ = value_ + init[i];			
		}
		index = tmp+1;
		add_atr(key_,value_);
		while(init[index]==' ' && init[index]!='\0'){
			index++;
		}	
	}
}

void elementXML::add_child(const elementXML& other){
	elementXML* new_children = new elementXML[size_children+1];
	for (int i = 0; i < size_children; ++i)
	{
		new_children[i] = children[i];
	}
	new_children[size_children] = other;
	size_children++;
	delete[] children;	
	children = new_children;
};

attribute* elementXML::find_atr_by_key(std::string key){
	attribute* result = nullptr;
	for (int i = 0; i < size_atr; i++)
	{
		if(atr[i].get_key().compare(key) == 0){
			result = &atr[i];
		}
	}
	return result;
}

void elementXML::delete_atr_by_key(std::string key){
	attribute* new_atr = new attribute[size_atr-1];
	int index = 0; 
	for (int i = 0; i < size_atr; i++)
	{
		if(atr[i].get_key().compare(key) == 0){

		}else{
			new_atr[index] = atr[i];
			index++;
		}
	}
	size_atr--;
	delete[] atr;
	atr = new_atr;
}

bool elementXML::has_key(std::string key){
	for (int i = 0; i < size_atr; i++)
	{
		if(atr[i].get_key().compare(key) == 0){
			return true;
		}
	}
	return false;
}

void elementXML::change_atr_value(std::string key, std::string value){
	for (int i = 0; i < size_atr; i++)
	{
		if(atr[i].get_key().compare(key) == 0){
			atr[i].set_value(value);
		}
	}
}

std::string elementXML::get_id(){
	std::string value_;
	for (int i = 0; i < size_atr; i++)
	{
		if(atr[i].get_key().compare("id") == 0){
			value_ = atr[i].get_value();
		}
	}
	return value_;
}

void elementXML::set_text(std::string text_){
	text = text_;
}

std::string elementXML::get_text(){
	return text;
}

bool elementXML::have_text(){
	if(text.size()>0){
		return true;
	}else{
		return false;
	}
}

void elementXML::add_atr(std::string key,std::string value){
	attribute* new_atr = new attribute[size_atr+1];
	for (int i = 0; i < size_atr; ++i)
	{
		new_atr[i] = atr[i];
	}
	new_atr[size_atr].set_key(key);
	new_atr[size_atr].set_value(value);
	size_atr++;
	delete[] atr;
	atr = new_atr;
};

void elementXML::printAll(int spaces){
	for (int i = 0; i < spaces; ++i){std::cout << "    ";}std::cout << "<" << name;
	for (int i = 0; i < size_atr; ++i){
		std::cout << ' ' << atr[i].get_key() << "=\"" << atr[i].get_value() << "\"";
	}
	std::cout << ">" << std::endl;
	if(have_text()){
		for (int i = 0; i < spaces+1; ++i){std::cout << "    ";} std::cout << text << std::endl;
	}
	for (int i = 0; i < size_children; ++i){
		(children + i)->printAll(spaces+1);
	}
	for (int i = 0; i < spaces; ++i){std::cout << "    ";}std::cout << "</" << name <<">" << std::endl;
}

std::string elementXML::get_name(){
	return name;
}

attribute* elementXML::get_attributes(){
	return atr;
}

elementXML* elementXML::get_children(){
	return children;
}

int elementXML::get_size_attributes(){
	return size_atr;
}

int elementXML::get_size_children(){
	return size_children;
}

elementXML::~elementXML(){
	delete[] atr;
	delete[] children;
}

#endif