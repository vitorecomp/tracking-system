#ifndef NN_INPUT
#define NN_INPUT

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

#include <bitset>	 // std::bitset
#include <iostream>   // std::cerr
#include <stdexcept>  // std::invalid_argument
#include <string>	 // std::string

class NnInput {
  public:
	int size;
	int input_size;
	int type;

	std::string filename;
	float** input;

	static int conversor(char* inter);

  public:
	NnInput();
	NnInput(std::string);
	void fill();
	void clear();
	void processData();
};

#endif
