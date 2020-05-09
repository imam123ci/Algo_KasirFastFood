#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream> 

//User standar csv input output structure
//Comma separated file
//4 column as Nova logic

//Function to readFile
//Return 2d string vector
auto bacaFile(std::string namafile, char delimeter) {
	//file connection
	std::fstream fl;
	fl.open(namafile, std::fstream::in);
	std::vector<std::vector<std::string>> data;
	if (!fl.is_open()) {
		std::cout << "file not opened";
		return data;
	}
	// variable
	std::string word, line;
	int i = 0;

	//looping row
	while (std::getline(fl, line, '\n')) {
		// used for breaking words 
		std::stringstream ss(line);
		//looping column
		data.push_back(std::vector<std::string>());
		// read every column data of a row and 
		// store it in a string variable, 'word'
		while (std::getline(ss, word, delimeter)) {
			data[i].push_back(word);
		}
		i++;
	}
	fl.close();

	return data;
}

auto tulisFile(std::string namafile, char delimeter, std::vector<std::vector<std::string>> data,
	std::string premessage = NULL) {
	// file pointer 
	std::fstream fl;
	fl.open(namafile, std::fstream::out | std::fstream::app);
	if (!fl.is_open()) {
		std::cout << "file not opened";
		return false;
	}
	// opens an existing csv file or creates a new file. 

	//write input
	//add premessage
	if (!premessage.empty())
		fl << premessage << std::endl;
	//add data
	for (std::vector<std::string> a : data) {
		for (std::string b : a) {
			fl << b << delimeter;
		}
		fl << std::endl;
	}

	fl.close();
	return true;
}

auto hapusFile(std::string namafile) {
	return (!remove(namafile.c_str()));
}
