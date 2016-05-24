#include "../headers/Entities.hpp"

Argument io::uniArgs;

Argument::Argument(){
	startText = "../database/TrainingDataSet/MunichAutobahn01/MunichAutobahn01.xml";
	pointTextName = "../results/points.xml";
	videoName= "../database/TrainingDataSet/MunichAutobahn01/MunichAutobahn01.avi";

	nthreads = 4;
	saving  = true;
	debug = true;
	savingPoints = true;
	savingVideo = true;
	saveImage = true;
}

Argument::Argument(int, char**) : Argument(){
}

string Argument::getStartText(){
	return startText;
}

string Argument::getPointTextName(){
	return pointTextName;
}


string Argument::getVideoName(){
	return videoName;
}

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
