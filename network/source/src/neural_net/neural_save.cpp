#include "../../headers/neural_net/neural_net.hpp"

// done
void NeuralNetwork::setInput(NnInput &input) {
	this->input = input;
}

// done
void NeuralNetwork::setOutput(NnOutput &output) {
	this->output = output;
}

void NeuralNetwork::saveFile(int i) {
	// open the text
	ofstream myfile;
	myfile.open(filename, std::ifstream::out);
	Json::StyledWriter writer;
	string json_s = writer.write(json);
	// get the string
	if (myfile.is_open()) {
		myfile << json_s.c_str();
		myfile.close();
	} else {
		throw std::invalid_argument("Json File invalid");
	}
}

// done
void NeuralNetwork::saveNn(int max = -1) {
	if (max == -1)
		max = io::configs.num_epocs - 1;

	string name = this->makeName();
	// open the text
	ofstream myfile;
	myfile.open(name, std::ofstream::out);
	// get the string
	if (myfile.is_open()) {
		for (int i = 0; i < layer_size.size(); i++) {
			for (int j = 0; j < layer_size[i]; j++) {
				if (i == 0) {
					for (int k = 0; k < input.input_size; k++) {
						myfile << layers[i][j][k] << " ";
					}
					myfile << endl;
				} else {
					for (int k = 0; k < layer_size[i - 1]; k++) {
						myfile << layers[i][j][k] << " ";
					}
					myfile << endl;
				}
			}
			myfile << endl;
			myfile << endl;
		}

		myfile << endl;
		myfile << endl;

		for (int i = 0; i < layer_size.size(); i++) {
			for (int j = 0; j < layer_size[i]; j++) {
				myfile << bias[i][j] << " ";
			}
			myfile << endl;
		}
		myfile << endl;
		myfile << endl;
		myfile << endl;

		myfile << max + 1;
		myfile << endl;
		for (int i = 0; i <= max; i++) {
			myfile << glerros[i] << " ";
		}
		myfile << endl;
		for (int i = 0; i <= max; i++) {
			myfile << gnerros[i] << " ";
		}
		myfile << endl;

		myfile.close();
	} else {
		cout << name << endl;
		throw std::invalid_argument("Not open to save");
	}

	jsonFormat(name, max);
	saveFile(max);
}

void NeuralNetwork::jsonFormat(string name, int posic) {
	json[name]["rate"] = io::configs.learnRate;
	json[name]["momen"] = io::configs.momen;
	json[name]["file"] = io::configs.training_image_file;
	json[name]["error"] = glerros[posic];
	json[name]["error_n"] = gnerros[posic];
}

void NeuralNetwork::saveOut() {
	string name = makeName();
	json[name]["file"] = io::configs.test_image_file;
	json[name]["final_error"] = this->globalError / 2;
	json[name]["final_error_n"] = this->num_errors;
}

string NeuralNetwork::makeName() {
	stringstream ss;
	ss << "./results/" << layer_size.size();
	for (int i = 0; i < layer_size.size(); i++) {
		ss << "_" << layer_size[i];
	}
	ss << "matrix.txt";
	return ss.str();
}

Json::Value NeuralNetwork::readJson() {
	string line;
	stringstream ss;
	// open the text
	ifstream myfile;
	myfile.open(filename, std::ifstream::in);

	// get the string
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			ss << line << '\n';
		}

		myfile.close();
	} else {
		ss << "{}";
	}

	// validate the input
	string json = ss.str().c_str();
	// parse the string
	Json::Value value;
	Json::Reader reader;

	bool parsed = reader.parse(json, value);
	if (!parsed)
		throw std::invalid_argument("Json File invalid");

	// configure from json
	return value;
}
