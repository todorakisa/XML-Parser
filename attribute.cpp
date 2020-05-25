#ifndef ATTRIBUTE_CPP_
#define ATTRIBUTE_CPP_

#include <string>
#include <iostream>
#include "attribute.h"

attribute& attribute::operator=(attribute& other){
	if(this!=&other){
		key = other.key;
		value = other.value;
	}
	return *this;
}

void attribute::set_key(std::string key_){
	key = key_;
}

void attribute::set_value(std::string value_){
	value = value_;
}

std::string attribute::get_key(){
	return key;
}

std::string attribute::get_value(){
	return value;
}

#endif