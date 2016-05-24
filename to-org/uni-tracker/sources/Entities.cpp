#include "../headers/Entities.hpp"

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

ullint Timer::getTotal(){
	return total;
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



Text::Text(string result, string input){
	this->result = result;
	this->input = input;

	TiXmlDocument doc(input.c_str());
	bool loadOkay = doc.LoadFile();
	if (loadOkay){
		dump_to_stdout(&doc); // defined later in the tutorial
	}else{
		std::cout << "Erro ao abrir xml " << input << std::endl;
	}
}

void Text::dump_attribs_to_stdout(TiXmlElement* pElement)
{
	if ( !pElement ) return;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int ival;

	bool objeto = false;
	Point position, size;
	while (pAttrib){
		string name = pAttrib->Name();
		string value = pAttrib->Value();

		//varifica se é o primeiro frame
		if(!this->element.compare("frame"))
			if(!name.compare("number"))
				if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)
					this->nFrame = ival;

		//no primeiro frame armazena as informações do objetos
		if(nFrame == 0)
			if(!this->element.compare("box")){
				objeto = true;
				if(!name.compare("xc"))
					if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)
						position.x = ival;
				if(!name.compare("yc"))
					if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)
						position.y = ival;
				if(!name.compare("w"))
					if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)
						size.x = ival;
				if(!name.compare("h"))
					if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)
						size.y = ival;

			}
		pAttrib=pAttrib->Next();
	}
	if(objeto){
		points.push_back(position);
		mask.push_back(size);
	}
}

void Text::dump_to_stdout( TiXmlNode* pParent){
	if ( !pParent ) return;

	TiXmlNode* pChild;
	int t = pParent->Type();
	int num;

	if( t == TiXmlNode::TINYXML_ELEMENT){
		this->element = pParent->Value();
		dump_attribs_to_stdout(pParent->ToElement());
	}

	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
		dump_to_stdout(pChild);
}

void Text::save(Point, int){}
