#include "../../headers/io_man/nn_output.hpp"
using namespace std;

int NnOutput::conversor(char* inter) {
	int inter_i = 0;
	for (int i = 0; i < 3; i++) {
		inter_i += (unsigned char)inter[i];
		inter_i *= 256;
	}
	inter_i += (int)inter[3];
	return inter_i;
}

void NnOutput::fill() {
	std::ifstream file;

	file.open(this->filename, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		std::cout << filename << std::endl;
		throw invalid_argument("arquivo de entrada nao encontrado");
	}

	char inter[4];

	file.read((char*)&inter, 4);
	this->type = NnOutput::conversor(inter);
	file.read((char*)&inter, 4);
	int num_images = NnOutput::conversor(inter);
	this->size = num_images;
	this->output = (float*)calloc(num_images, sizeof(float));

	for (int i = 0; i < num_images; i++) {
		file.read((char*)&inter, 1);
		int value = (unsigned char)inter[0];
		this->output[i] = value;
	}

	file.close();
}

void NnOutput::clear() {
	free(this->output);

	if (output_processed != NULL) {
		for (int i = 0; i < this->size; i++)
			free(this->output_processed[i]);
		free(this->output_processed);
	}
}

NnOutput::NnOutput(std::string filename) {
	NnOutput();
	this->filename = filename;
}

NnOutput::NnOutput() {
	this->filename = "";
	this->output_processed = NULL;
	this->output = NULL;
	this->output_size = 10;
	this->size = 0;
	this->type = 0;
}

void NnOutput::processData() {
	this->output_size = 10;
	this->output_processed = (float**)calloc(this->size, sizeof(float*));
	for (int i = 0; i < this->size; i++) {
		float* uni_matrix = (float*)calloc(output_size, sizeof(float));
		for (int j = 0; j < output_size; j++)
			uni_matrix[j] = -0.9;
		int index = this->output[i];
		uni_matrix[index] = 0.9;
		this->output_processed[i] = uni_matrix;
	}
}
