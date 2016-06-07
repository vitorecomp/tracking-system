#include "../../headers/neural_net/neural_net.hpp"

void NeuralNetwork::saveWekaFormat() {
	string name = "./results/weka_format.csv";
	ifstream test;
	test.open(name, std::ifstream::in);
	if (test.is_open()) {
		test.close();
		return;
	}
	ofstream myfile;
	myfile.open(name, std::ofstream::out);

	// get the string
	if (myfile.is_open()) {
		for (int j = 0; j < this->input.input_size; j++) {
			stringstream ss;
			ss << "input" << j;
			myfile << ss.str() << " , ";
		}

		for (int j = 0; j < this->output.output_size; j++) {
			stringstream ss;
			ss << "output" << j;
			myfile << ss.str() << " , ";
		}
		myfile << endl;

		for (int i = 0; i < this->input.size; i++) {
			for (int j = 0; j < this->input.input_size; j++) {
				myfile << this->input.input[i][j] << " , ";
			}

			for (int j = 0; j < this->output.output_size; j++) {
				myfile << this->output.output_processed[i][j] << ",";
			}
			myfile << endl;
		}

		myfile.close();
	} else {
		cout << name << endl;
		throw std::invalid_argument("Not open to save");
	}
}

void NeuralNetwork::savePythonFormat() {
	string name = "./results/python_format.csv";
	ifstream test;
	test.open(name, std::ifstream::in);
	if (test.is_open()) {
		test.close();
		return;
	}
	ofstream myfile;
	myfile.open(name, std::ofstream::out);

	// get the string
	if (myfile.is_open()) {
		myfile << this->input.size << endl;
		myfile << this->input.input_size << endl;
		myfile << this->output.output_size << endl;

		for (int i = 0; i < this->input.size; i++) {
			for (int j = 0; j < this->input.input_size; j++) {
				myfile << this->input.input[i][j] << ";";
			}

			for (int j = 0; j < this->output.output_size; j++) {
				myfile << this->output.output_processed[i][j] << ";";
			}
			myfile << endl;
		}

		myfile.close();
	} else {
		cout << name << endl;
		throw std::invalid_argument("Not open to save");
	}
}
