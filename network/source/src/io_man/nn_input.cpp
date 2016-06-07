#include "../../headers/io_man/nn_input.hpp"
using namespace std;
int NnInput::conversor(char* inter) {
	int inter_i = 0;
	for (int i = 0; i < 3; i++) {
		inter_i += (unsigned char)inter[i];
		inter_i *= 256;
	}
	inter_i += (int)inter[3];
	return inter_i;
}

void NnInput::fill() {
	std::ifstream file;
	file.open(this->filename, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		std::cout << filename << std::endl;
		throw invalid_argument("arquivo de entrada nao encontrado");
	}

	char inter[4];

	file.read((char*)&inter, 4);
	this->type = NnInput::conversor(inter);
	file.read((char*)&inter, 4);
	int num_images = NnInput::conversor(inter);
	this->size = num_images;
	this->input = (float**)calloc(num_images, sizeof(float*));

	file.read((char*)&inter, 4);
	uint x_size = NnInput::conversor(inter);

	file.read((char*)&inter, 4);
	uint y_size = NnInput::conversor(inter);

	int input_size = x_size * y_size;
	this->input_size = input_size;

	for (int i = 0; i < num_images; i++) {
		this->input[i] = (float*)calloc(input_size, sizeof(float));
		for (int j = 0; j < input_size; j++) {
			file.read((char*)&inter, 1);
			int value = (unsigned char)inter[0];
			this->input[i][j] = value;
		}
	}

	file.close();
}

void NnInput::clear() {
	for (int i = 0; i < this->size; i++) {
		free(this->input[i]);
	}
	free(this->input);
}

NnInput::NnInput(std::string filename) {
	this->filename = filename;
	this->input = NULL;
	this->size = 0;
	this->input_size = 0;
}

NnInput::NnInput() {
	this->filename = "";
	this->input = NULL;
	this->size = 0;
	this->input_size = 0;
}

void NnInput::processData() {
	float diff = 127;

	switch (this->type) {
		case 2051:
			for (int i = 0; i < this->size; i++) {
				for (int j = 0; j < this->input_size; j++) {
					this->input[i][j] = (this->input[i][j] - diff) / diff;
				}
			}
			break;
		default:
			for (int i = 0; i < this->size; i++) {
				for (int j = 0; j < this->input_size; j++) {
					this->input[i][j] = (this->input[i][j] - diff) / diff;
				}
			}
			break;
	}
}
