#include "Entities.hpp"

Timer::Timer(){
	gettimeofday(&start_v, NULL);
	gettimeofday(&end_v, NULL);
	total = 0;
}

void Timer::start(){
	gettimeofday(&start_v, NULL);
}

ullint Timer::end(){
	gettimeofday(&end_v, NULL);
	total += time_to_int(start_v, end_v);
	return total;
}

void Timer::reset(){
	total = 0;
}

void Arguments::decode(int nargs, char **args){
	string key, value;
	for(int i = 1	; i < nargs; i++){
		if(args[i][0] == '-'){
			key = args[i];
			key = key.substr(1);
			value = "true";
		}else{
			key = args[i];
			if(i == nargs - 1){
				key = "No value for Argument" + key;
				throw invalid_argument(key);
			}
			i++;
			value = args[i];
		}
		map_args.insert(pair<string, string>(key, value));
	}
}

map<string, string> Arguments::getMap(){
	return map_args;
}


Arguments io::uni_args;
