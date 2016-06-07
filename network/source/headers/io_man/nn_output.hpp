#ifndef NN_OUTPUT
#define NN_OUTPUT

#include <fstream>
#include <iostream>
#include <sstream>

#include <bitset>	 // std::bitset
#include <iostream>   // std::cerr
#include <stdexcept>  // std::invalid_argument
#include <string>	 // std::string

class NnOutput {
  public:
	int size;
	int output_size;

	int type;

	std::string filename;
	float* output;
	float** output_processed;
	static int conversor(char* inter);

  public:
	NnOutput();
	NnOutput(std::string);
	void fill();
	void clear();
	void processData();
};

#endif
