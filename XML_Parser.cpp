#include <iostream>
#include <string>

using namespace std;

#include "fileXML.h"
#include "xpath.h"

int main(){

	string command;
	bool active = true;
	FileXML file;

	while(active){

		cout << ">";
		getline(cin,command);

		if(command.compare(0,5,"open ") == 0){
			if(file.is_filled()){
				cout << "You must close the opened file first" << endl;
			}else{
				string file_name = command.substr(5);
				file.set_name(file_name);
				if(file.fill_content()){
					cout << "Successfully opened " << file.get_name() << endl;
				}
			}	
		}

		if(command.compare("close") == 0){
			if(file.is_filled()){
				file.empty_content();
				cout << "Successfully closed " << file.get_name() << endl;
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare(0,7,"saveas ") == 0){
			if(file.is_filled()){
				string file_name = command.substr(7);
				file.save_as(file_name);
				cout << "Successfully saved as " << file_name << endl;	
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare("save") == 0){
			if(file.is_filled()){
				file.save_as(file.get_name());
				cout << "Successfully saved " << file.get_name() << endl;
			}else{
				cout << "You must open a file first" << endl;
			}	
		}

		if(command.compare("help") == 0){
			cout << "-------------------------------------------------------" << endl;
			cout << "The following commands are supported:" << endl;
			cout << "-------------------------------------------------------" << endl;
			cout << "open <file>     opens <file>, if there is not such file it creates one!" << endl;
			cout << "close           closes currently opened file" << endl;
			cout << "save            saves the currently open file" << endl;
			cout << "saveas <file>   saves the currently open file in <file>" << endl;
			cout << "help            prints this information" << endl;
			cout << "exit            exits the program" << endl;
			cout << "-------------------------------------------------------" << endl;
			cout << "XML Functionalyties:" << endl;
			cout << "-------------------------------------------------------" << endl;
			cout << "print                    prints the opened file" << endl;
			cout << "select <id> <key>        prints the value of the attribute" << endl;
			cout << "set <id> <key> <value>   sets a value to a give attribute of element" << endl;
			cout << "children <id>            prints all childs of element" << endl;
			cout << "child <id> <n>           prints the n child of element" << endl;
			cout << "text <id>                prints the text of element" << endl;
			cout << "delete <id> <key>        deletes an attribute of element" << endl;
			cout << "newchild <id>            creates empty element with id as child of givens id element" << endl;
			cout << "xpath <id> <XPath>       executing simple XPath 2.0 requests" << endl;
			cout << "-------------------------------------------------------" << endl;
		}	

		if(command.compare("exit") == 0){
			file.empty_content();
			cout << "Exiting the program..." << endl;
			active = false;
		}

		if(command.compare("print") == 0){
			if(file.is_filled()){
				file.print();
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare(0,7,"select ") == 0){
			if(file.is_filled()){
				string part_of_command = command.substr(7);
				string id;
				string key;

				for (int i = 0; i < part_of_command.size(); ++i)
				{
					if(part_of_command[i] == ' '){
						id = part_of_command.substr(0,i);
						key = part_of_command.substr(i+1);
						break;
					}
				}

				elementXML* result_elem = file.find_elem_by_id(id);
				attribute* result_atr;
				if(result_elem!=nullptr)
				result_atr = result_elem->find_atr_by_key(key);
				if(result_elem == nullptr){
					cout << "No element with such id!" << endl;
				}else if(result_atr == nullptr){
					cout << "No attribute with such key!" << endl;
				}else{
					cout << "Value of the atribut: " << result_atr->get_value() << endl;
				}
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare(0,4,"set ") == 0){
			if(file.is_filled()){
				string part_of_command = command.substr(4);
				string id;
				string key;
				string value;

				for (int i = 0; i < part_of_command.size(); ++i)
				{
					if(part_of_command[i] == ' '){
						id = part_of_command.substr(0,i);
						part_of_command = part_of_command.substr(i);
						break;
					}
				}

				for (int i = 1; i < part_of_command.size(); ++i)
				{
					if(part_of_command[i] == ' '){
						key = part_of_command.substr(1,i-1);
						value = part_of_command.substr(i+1);
						break;
					}
				}

				elementXML* result_elem = file.find_elem_by_id(id);
				attribute* result_atr;
				if(result_elem!=nullptr)
				result_atr = result_elem->find_atr_by_key(key);
				if(result_elem == nullptr){
					cout << "No element with such id!" << endl;
				}else if(result_atr == nullptr){
					cout << "No attribute with such key!" << endl;
				}else{
					result_atr->set_value(value);
					cout << "Value seted!" << endl;
				}
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare(0,9,"children ") == 0){
			if(file.is_filled()){
				string id = command.substr(9);
				elementXML* result_elem = file.find_elem_by_id(id);
				if(result_elem == nullptr){
					cout << "No element with such id!" << endl;
				}else{
					elementXML* children = result_elem->get_children();
					if(children == nullptr){
						cout << "This element doesn't have children" << endl;
					}else{
						cout << "Childs:" << endl; 
						for (int i = 0; i < result_elem->get_size_children(); ++i)
						{
							cout <<"-"<< children[i].get_name() << endl;
						}
					}		
				}	
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare(0,5,"text ") == 0){
			if(file.is_filled()){
				string id = command.substr(5);
				elementXML* result_elem = file.find_elem_by_id(id);
				if(result_elem == nullptr){
					cout << "No element with such id!" << endl;
				}else{
					cout << "Text:" << result_elem->get_text() << endl;
				}
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare(0,6,"child ") == 0){
			if(file.is_filled()){
				string part_of_command = command.substr(6);
				string id;
				int n;

				for (int i = 0; i < part_of_command.size(); ++i)
				{
					if(part_of_command[i] == ' '){
						id = part_of_command.substr(0,i);
						n = stoi(part_of_command.substr(i));
						break;
					}
				}
				
				elementXML* elem = file.get_n_child(id,n);
				if(elem == nullptr){
					cout << "No such element" << endl;
				}else{
					cout << "N child of " << id << ": " << elem->get_name() << endl;
				}
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare(0,7,"delete ") == 0){
			if(file.is_filled()){
				string part_of_command = command.substr(7);
				string id;
				string key;

				for (int i = 0; i < part_of_command.size(); ++i)
				{
					if(part_of_command[i] == ' '){
						id = part_of_command.substr(0,i);
						key = part_of_command.substr(i+1);
						break;
					}
				}

				elementXML* elem = file.find_elem_by_id(id);
				if(elem == nullptr){
					cout << "No element with such id!" << endl;
				}else if(!elem->has_key(key)){
					cout << "No attribute with such key!" << endl;
				}else{
					elem->delete_atr_by_key(key);
					cout << "Successfully deleted!" << endl;
				}
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare(0,9,"newchild ") == 0){
			if(file.is_filled()){
				string id = command.substr(9);
				elementXML new_elem;
				elementXML* elem = file.find_elem_by_id(id);
				if(elem == nullptr){
					cout << "No element with such id!" << endl;
				}else{
					elem->add_child(new_elem);
					cout << "Successfully added new child!" << endl;
				}
				file.setup_ids();
			}else{
				cout << "You must open a file first" << endl;
			}
		}

		if(command.compare(0,6,"xpath ") == 0){
			if(file.is_filled()){
				string part_of_command = command.substr(6);
				string id;
				string xpath;

				for (int i = 0; i < part_of_command.size(); ++i)
				{
					if(part_of_command[i] == ' '){
						id = part_of_command.substr(0,i);
						xpath = part_of_command.substr(i+1);
						break;
					}
				}

				elementXML* elem = file.find_elem_by_id(id);
				if(elem == nullptr){
					cout << "No element with such id!" << endl;
				}else{
					string name;
					XPath path(elem);
					path.execute(xpath);
					path.print();
				}
				
			}else{
				cout << "You must open a file first" << endl;
			}
		}
	}
}