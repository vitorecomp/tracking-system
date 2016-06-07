// includes

#include <fstream>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

// local libs
#include "./libs/jsonParser/json.h"
#include "./libs/output/IOMananger.hpp"

// files

#include "./headers/io_man/nn_input.hpp"
#include "./headers/io_man/nn_output.hpp"

#include "./headers/neural_net/neural_net.hpp"

// namespace
using namespace std;

// main
int main(int argc, char *argv[]) {
	// verify input
	if (argc < 2) {
		io::configs.runFile("{}");
	} else
		io::configs.openFile(argv[1]);
	io::configs.setConfigs();

	// thread io_th(run_io);
	NnInput train_input(io::configs.training_image_file);
	NnInput test_input(io::configs.test_image_file);

	NnOutput train_output(io::configs.training_result_file);
	NnOutput test_output(io::configs.test_result_file);

	std::vector<std::vector<int>> net_sizes = io::configs.net_sizes;
	std::string net_output = io::configs.net_output;
	NeuralNetwork nn(net_sizes, net_output);

	if (io::configs.to_train) {
		train_input.fill();
		train_output.fill();

		train_input.processData();
		train_output.processData();

		nn.setInput(train_input);
		nn.setOutput(train_output);

		nn.saveWekaFormat();
		nn.savePythonFormat();

		nn.runTraining();

		train_input.clear();
		train_output.clear();
	}

	if (io::configs.to_test) {
		test_input.fill();
		test_output.fill();

		test_input.processData();
		test_output.processData();

		nn.setInput(test_input);
		nn.setOutput(test_output);

		nn.runValidation();

		test_input.clear();
		test_output.clear();
	}
}
