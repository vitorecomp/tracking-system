class Variables {
  public:
	std::string training_image_file;
	std::string training_result_file;

	std::string test_image_file;
	std::string test_result_file;

	std::vector<std::vector<int>> net_sizes;
	std::string net_output;

	bool to_train;
	bool to_test;

	bool batch;
	bool adaptable;
	int num_epocs;

	double learnRate;
	double momen;
	int mul;
	int sum;
};
