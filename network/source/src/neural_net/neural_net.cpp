#include "../../headers/neural_net/neural_net.hpp"

// done
void NeuralNetwork::runValidation() {
	for (int i = 0; i < sizes.size(); i++) {
		this->buildNn(sizes[i]);
		this->validate();
	}
}

// done
void NeuralNetwork::runTraining() {
	json = this->readJson();
	// srand(time(NULL));
	// srand(time(0));
	for (int i = 0; i < sizes.size(); i++) {
		srand(1);
		this->buildNn(sizes[i]);
		this->train();
		this->saveNn(-1);
		this->destroyNet(sizes[i]);
	}
}

// done
NeuralNetwork::NeuralNetwork(nnsizes sizes, std::string filename) {
	this->sizes = sizes;
	this->filename = filename;
}

// done
void NeuralNetwork::buildNn(vector<int> sizes) {
	sizes.push_back(this->output.output_size);
	this->layers = (float ***)calloc(sizes.size(), sizeof(float **));
	this->layers_delta = (float ***)calloc(sizes.size(), sizeof(float **));

	this->outputs = (float **)calloc(sizes.size(), sizeof(float *));
	this->delta_error = (float **)calloc(sizes.size(), sizeof(float *));
	this->bias = (float **)calloc(sizes.size(), sizeof(float *));

	int input_size = this->input.input_size;

	this->layer_size = sizes;

	for (int i = 0; i < sizes.size(); i++) {
		int rows = sizes[i];
		int cols = input_size + 1;

		this->layers[i] = this->sin_matrix(rows, cols, true);
		this->layers_delta[i] = this->sin_matrix(rows, cols, false);

		this->outputs[i] = (float *)calloc(rows, sizeof(float));
		this->delta_error[i] = (float *)calloc(rows, sizeof(float));

		this->bias[i] = (float *)calloc(rows, sizeof(float));

		float sign = -1;
		for (int j = 0; j < rows; j++) {
			float random = (float(rand()) / float(RAND_MAX)) / 2.f;  // min 0.5
			random *= sign;
			sign *= -1;
			this->bias[i][j] = random;
		}

		input_size = sizes[i];
	}

	this->errors =
		(float *)calloc(layer_size[(layer_size.size() - 1)], sizeof(float));

	this->glerros = (float *)calloc(io::configs.num_epocs, sizeof(float));
	this->gnerros = (float *)calloc(io::configs.num_epocs, sizeof(float));

	if (this->exists()) {
		this->fill_layers();
		return;
	}
}

void NeuralNetwork::destroyNet(vector<int> sizes) {
	for (int i = 0; i < sizes.size(); i++) {
		int rows = sizes[i];

		for (int j = 0; j < rows; j++) {
			free(this->layers[i][j]);
			free(this->layers_delta[i][j]);
		}
		free(this->layers[i]);
		free(this->layers_delta[i]);
		free(this->outputs[i]);
		free(this->bias[i]);
	}

	free(this->errors);
	free(this->layers);
	free(this->layers_delta);

	free(this->outputs);
	free(this->bias);

	free(this->glerros);
	free(this->gnerros);
}

// done
void NeuralNetwork::train() {
	int epocs = io::configs.num_epocs;
	bool batch = io::configs.batch;

	for (int i = start_epoc; i < epocs; i++) {
		this->globalError = 0;
		this->num_errors = 0;

		for (int j = 0; j < this->input.size; j++) {
			cout << "Porcentage " << setw(8)
				 << ((float)j * 100) / (float)this->input.size << "\r";
			this->neural_run(j);

			if (batch) {
				this->sum_errors(j);
			} else {
				this->calibrate_weights(batch, j);
			}
		}

		cout << "                         ";
		cout << "Epoca :" << i + 1 << " ; ";
		cout << "Error : " << this->globalError / 2 << " ; ";
		cout << "Error N : " << this->num_errors << " \r ";

		this->glerros[i] = (this->globalError) / 2;
		this->gnerros[i] = this->num_errors;

		if (i % io::configs.mul == 0) {
			this->saveNn(i);
		}

		if (batch) {
			this->calibrate_weights(batch, 0);
		}
	}
	cout << endl;
}

// done
void NeuralNetwork::neural_run(int input_num) {
	// input layer
	for (int i = 0; i < this->layer_size[0]; i++) {
		this->outputs[0][i] = 0;
		for (int j = 0; j < this->input.input_size; j++) {
			this->outputs[0][i] +=
				this->input.input[input_num][j] * this->layers[0][i][j];
		}
		this->outputs[0][i] += this->bias[0][i];
		this->outputs[0][i] = this->function(0, this->outputs[0][i]);
	}

	// next layers
	for (int l = 1; l < this->layer_size.size(); l++) {
		for (int i = 0; i < this->layer_size[l]; i++) {
			this->outputs[l][i] = 0;
			for (int j = 0; j < this->layer_size[l - 1]; j++) {
				this->outputs[l][i] +=
					this->outputs[l - 1][j] * this->layers[l][i][j];
			}
			this->outputs[l][i] += this->bias[l][i];
			this->outputs[l][i] = this->function(0, this->outputs[l][i]);
		}
	}
}

float NeuralNetwork::function(int derivate, float x) {
	switch (derivate) {
		case 0:
			return tanh(x);

		// return (1.f / (1.f + exp(-x)));
		case 1:
			return 1 - pow(x, 2);
			// return exp(x) / pow((exp(x) + 1), 2);
	}

	return 0;
}

float NeuralNetwork::isIqual(int input_num, int i) {
	float des = -0.45, atin = -0.45;
	if (this->output.output_processed[input_num][i] > 0)
		des = 0.45;

	if (this->outputs[layer_size.size() - 1][i] > 0)
		atin = 0.45;

	return des - atin;
}

void NeuralNetwork::calcularStartError(int input_num) {
	bool right = true;
	int last = (layer_size.size() - 1);
	for (int i = 0; i < layer_size[(layer_size.size() - 1)]; i++) {
		float error_l = 0;
		float local = this->isIqual(input_num, i);
		right = right && (local == 0);

		error_l += (this->output.output_processed[input_num][i] -
					this->outputs[layer_size.size() - 1][i]);
		delta_error[last][i] = function(1, this->outputs[last][i]) * error_l;

		this->globalError += error_l * error_l;
		if (local != 0 && io::configs.sum == 1)
			error_l += local;
	}
	if (!right) {
		this->num_errors++;
	}
}

void NeuralNetwork::calibrate_last() {
	float delta_u = 0;

	int last = (layer_size.size() - 1);
	// error na ultima camada
	for (int i = 0; i < layer_size[last]; i++) {
		for (int j = 0; j < layer_size[last - 1]; j++) {
			delta_u = delta_error[last][i] * outputs[last - 1][j];

			layers[last][i][j] +=
				learnRate * delta_u + momen * layers_delta[last][i][j];
			layers_delta[last][i][j] = delta_u;
		}
	}
}

void NeuralNetwork::calibrate_input(int input_num) {
	float delta_u = 0;
	for (int i = 0; i < layer_size[0]; i++) {
		for (int j = 0; j < this->input.input_size; j++) {
			delta_u = delta_error[0][i] * input.input[input_num][j];

			layers[0][i][j] +=
				learnRate * delta_u + momen * layers_delta[0][i][j];
			layers_delta[0][i][j] = delta_u;
		}
	}
}

void NeuralNetwork::calibrate_hidden() {
	// calculo de delta interno
	int last = (layer_size.size() - 1);
	float delta_u = 0;
	for (int l = last - 1; l > 0; l--) {
		for (int i = 0; i < layer_size[l]; i++) {
			for (int j = 0; j < layer_size[l - 1]; j++) {
				delta_u = delta_error[l][i] * outputs[l - 1][j];
				layers[l][i][j] +=
					learnRate * delta_u + momen * layers_delta[l][i][j];
				layers_delta[l][i][j] = delta_u;
			}
		}
	}
}

// done
void NeuralNetwork::calibrate_weights(bool batch, int input_num) {
	learnRate = io::configs.learnRate;
	momen = io::configs.momen;

	// calculo de error
	int last = (layer_size.size() - 1);

	if (!batch) {
		this->calcularStartError(input_num);
	}

	this->calculateDeltas();
	this->calibrate_last();
	this->calibrate_hidden();
	this->calibrate_input(input_num);
}

void NeuralNetwork::calculateDeltas() {
	int last = (layer_size.size() - 1);
	// calculo de delta interno
	for (int l = last - 1; l >= 0; l--) {
		for (int i = 0; i < layer_size[l]; i++) {
			float error = 0;
			for (int j = 0; j < layer_size[l + 1]; j++) {
				error += delta_error[l + 1][j] * layers[l + 1][j][i];
			}
			delta_error[l][i] = function(1, outputs[l][i]) * error;
		}
	}
}

// done
float **NeuralNetwork::sin_matrix(int rows, int cols, bool random_num) {
	float sign = -1;
	float random = 0;
	float **matrix = (float **)calloc(rows, sizeof(float *));
	for (int i = 0; i < rows; i++) {
		matrix[i] = (float *)calloc(cols, sizeof(float));
		if (random_num)
			for (int j = 0; j < cols; j++) {
				// get a random number between -0.5 and 0.5
				random = (float(rand()) / float(RAND_MAX)) / 2.f;  // min 0.5
				random *= sign;
				sign *= -1;

				matrix[i][j] = random;
			}
	}
	return matrix;
}

// todo
void NeuralNetwork::validate() {
	this->globalError = 0;
	this->num_errors = 0;
	cout << "validando o processo:" << endl;
	for (int j = 0; j < this->input.size; j++) {
		cout << "Porcentage " << setw(8)
			 << ((float)j * 100) / (float)this->input.size << "\r";
		this->neural_run(j);
		this->calcularStartError(j);
	}
	cout << endl;
	cout << "Para a rede " << makeName() << endl;
	cout << "Com " << io::configs.num_epocs << " epocas" << endl;
	cout << "Erro quadatico medio = " << this->globalError / 2 << endl;
	cout << "Numuro classificado errados " << this->num_errors << endl;

	saveOut();
	saveFile(-1);
}

// todo
bool NeuralNetwork::exists() {
	string name = this->makeName();
	// open the text
	ifstream myfile;
	myfile.open(name, std::ifstream::in);
	// get the string
	if (myfile.is_open()) {
		myfile.close();
		return true;
	} else {
		return false;
	}
	return false;
}

// todo
void NeuralNetwork::fill_layers() {
	string name = this->makeName();
	// open the text
	ifstream myfile;
	myfile.open(name, std::ifstream::in);
	// get the string
	if (myfile.is_open()) {
		for (int i = 0; i < layer_size.size(); i++) {
			for (int j = 0; j < layer_size[i]; j++) {
				if (i == 0) {
					for (int k = 0; k < input.input_size; k++) {
						myfile >> layers[i][j][k];
					}
				} else {
					for (int k = 0; k < layer_size[i - 1]; k++) {
						myfile >> layers[i][j][k];
					}
				}
			}
		}

		for (int i = 0; i < layer_size.size(); i++) {
			for (int j = 0; j < layer_size[i]; j++) {
				myfile >> bias[i][j];
			}
		}

		myfile >> this->start_epoc;

		for (int i = 0; i < this->start_epoc; i++) {
			myfile >> glerros[i];
		}

		for (int i = 0; i < this->start_epoc; i++) {
			myfile >> gnerros[i];
		}

		myfile.close();
	} else {
		cout << name << endl;
		throw std::invalid_argument("Not open to save");
	}
}

// todo
void NeuralNetwork::sum_errors(int time) {
}
