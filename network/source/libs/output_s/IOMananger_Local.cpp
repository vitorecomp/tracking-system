#include "../output/IOMananger.hpp"

// Start screen
void Output::makeMap() {
	/*
	Screen *main_messages = (Screen*) new MessageBox("main_messages", 0, 0,
	35, 80, io::configs.windows_lines);
	screen.insert(std::pair<string,Screen*>("main_messages",main_messages)
   );
	Screen *progress_bar = (Screen*) new ProgressBar(   "progress_bar",
														0, 80, 5, 60,
														ProgressBar::HORIZONTAL,
														0,
														100,
														io::configs.windows_lines);
	screen.insert(std::pair<string,Screen*>("progress_bar",progress_bar));
	*/
}

// make configs
void Configs::decodeMap() {
	if (json["training_image_file"] != Json::nullValue)
		this->training_image_file = json["training_image_file"].asString();
	else
		this->training_image_file = "./resource/train-images-idx3-ubyte";

	if (json["test_image_file"] != Json::nullValue)
		this->test_image_file = json["test_image_file"].asString();
	else
		this->test_image_file = "./resource/t10k-images-idx3-ubyte";

	if (json["training_result_file"] != Json::nullValue)
		this->training_result_file = json["training_result_file"].asString();
	else
		this->training_result_file = "./resource/train-labels-idx1-ubyte";

	if (json["test_result_file"] != Json::nullValue)
		this->test_result_file = json["test_result_file"].asString();
	else
		this->test_result_file = "./resource/t10k-labels-idx1-ubyte";

	if (json["to_train"] != Json::nullValue)
		this->to_train = json["to_train"].asBool();
	else
		this->to_train = true;

	if (json["to_test"] != Json::nullValue)
		this->to_test = json["to_test"].asBool();
	else
		this->to_test = true;

	if (json["batch"] != Json::nullValue)
		this->batch = json["batch"].asBool();
	else
		this->batch = false;

	if (json["adaptable"] != Json::nullValue)
		this->adaptable = json["adaptable"].asBool();
	else
		this->adaptable = false;

	if (json["num_epocs"] != Json::nullValue)
		this->num_epocs = json["num_epocs"].asInt();
	else
		this->num_epocs = 10;

	if (json["learnRate"] != Json::nullValue)
		this->learnRate = json["learnRate"].asDouble();
	else
		this->learnRate = 0.5;

	if (json["momen"] != Json::nullValue)
		this->momen = json["momen"].asDouble();
	else
		this->momen = 0.002;

	if (json["mul"] != Json::nullValue)
		this->mul = json["mul"].asInt();
	else
		this->mul = 3;

	if (json["sum"] != Json::nullValue)
		this->sum = json["sum"].asInt();
	else
		this->sum = 0;

	int neurons = 10;
	if (json["neurons"] != Json::nullValue)
		neurons = json["neurons"].asInt();
	else
		neurons = 10;

	if (json["nn_struct"] != Json::nullValue) {
		const Json::Value seeds = json["nn_struct"];
		for (int index = 0; index < seeds.size(); ++index) {
			std::vector<int> v;
			const Json::Value inter = seeds[index];
			for (int index2 = 0; index2 < inter.size(); index2++) {
				v.push_back(inter[index2].asInt());
			}
			net_sizes.push_back(v);
		}
	} else {
		std::vector<int> v;
		v.push_back(neurons);
		net_sizes.push_back(v);
	}

	if (json["net_output"] != Json::nullValue)
		this->net_output = json["net_output"].asString();
	else
		this->net_output = "./results/nets.json";
}

// Logger make Map
void Logger::makeMap() {
}
