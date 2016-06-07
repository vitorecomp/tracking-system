#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

// local libs
#include "../../libs/jsonParser/json.h"
#include "../../libs/output/IOMananger.hpp"

// files

#include "../io_man/nn_input.hpp"
#include "../io_man/nn_output.hpp"

#define MUL 3

typedef std::vector<std::vector<int>> nnsizes;

class NeuralNetwork {
	NnInput input;
	NnOutput output;

	std::string filename;
	nnsizes sizes;

	float*** layers;
	float** bias;

	float*** layers_delta;
	float** delta_error;

	float** outputs;

	float* errors;
	float* glerros;
	float* gnerros;

	float learnRate;
	float momen;

	float globalError;
	int num_errors;

	int start_epoc;

	std::vector<int> layer_size;

	// methods build
	void buildNn(std::vector<int>);
	float** sin_matrix(int, int, bool);

	bool exists();
	void fill_layers();

	// execution
	void validate();
	void train();
	void neural_run(int);

	// error
	void sum_errors(int);
	void calibrate_weights(bool, int);
	void calculateDeltas();
	void calibrate_last();
	void calibrate_hidden();
	void calibrate_input(int);
	void calcularStartError(int);

	float calulateError(int, int);
	float isIqual(int, int);

	// saveNn
	Json::Value json;
	Json::Value readJson();
	void jsonFormat(string, int);
	string makeName();
	void saveFile(int);
	void saveNn(int);
	void saveOut();

	// free
	void destroyNet(std::vector<int>);

	// func
	float function(int, float);

  public:
	NeuralNetwork(nnsizes, std::string);

	void setInput(NnInput&);
	void setOutput(NnOutput&);

	void runTraining();
	void runValidation();

	void saveWekaFormat();
	void savePythonFormat();
};

#endif
