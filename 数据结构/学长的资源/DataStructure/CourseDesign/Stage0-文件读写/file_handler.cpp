#include <bits/stdc++.h>
#include "file_handler.h"
using std::string;
using std::fstream;
using std::ios;
fstream file;
string path; 
FileHandler::FileHandler(){}
FileHandler::~FileHandler(){} 
int FileHandler::open(const std::string& file_path) {
	path=file_path;
	file.open(file_path, ios::out); 
	return 0;
}
int FileHandler::write(const std::string& data) {
	if(!file.is_open() )
	file.open(path,ios::app);
	file << data;
	file.close();
	return 0;
}
int FileHandler::read(std::string* data) {
	if(file.is_open())
	file.close();
	file.open(path,ios::in);
	string line;
	while (getline(file, line)) {
		*data += line;
	}
	file.close();
	return 0;
}
