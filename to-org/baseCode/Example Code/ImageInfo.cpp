/*! \file
\author Florian Schmidt
\date 23.08.2010
*/

#include "ImageInfo.h"

// Boost
#include <boost\foreach.hpp>
#include <boost\lexical_cast.hpp>

namespace fs 
{
	/////////////////////////////////////////////////////////

	ImageInfo::ImageInfo(): 
		fidx(-1), file_name(NULL), time(NULL), color_space(NULL), depth(NULL),
		pixel_size(NULL), x_origin(NULL), y_origin(NULL),
		latitude(NULL),	longitude(NULL), sunny(NULL)			
	{}

	ImageInfo::ImageInfo(TiXmlElement* xml):
		fidx(-1), file_name(NULL), time(NULL), color_space(NULL), depth(NULL),
		pixel_size(NULL), x_origin(NULL), y_origin(NULL),
		latitude(NULL),	longitude(NULL), sunny(NULL)			
	{
		if (string(xml->Value()) == "frame")
		{			
			xml->QueryIntAttribute("number", &fidx);			

			string string_attribute;
			
			if (xml->QueryStringAttribute("file", &string_attribute) == TIXML_SUCCESS)
				set_file_name(string_attribute);
			if (xml->QueryStringAttribute("color", &string_attribute) == TIXML_SUCCESS)
				set_color_space(string_attribute);
			if (xml->QueryStringAttribute("depth", &string_attribute) == TIXML_SUCCESS)
				set_depth(string_attribute);
			if (xml->QueryStringAttribute("utc", &string_attribute) == TIXML_SUCCESS)
				set_time(bpt::time_from_string(string_attribute));

			double double_attribute;

			if (xml->QueryDoubleAttribute("gsd", &double_attribute) == TIXML_SUCCESS ||
				xml->QueryDoubleAttribute("pixel_size", &double_attribute) == TIXML_SUCCESS)
				set_pixel_size(double_attribute);
			if (xml->QueryDoubleAttribute("x", &double_attribute) == TIXML_SUCCESS)
				set_x_origin(double_attribute);
			if (xml->QueryDoubleAttribute("y", &double_attribute) == TIXML_SUCCESS)
				set_y_origin(double_attribute);
			if (xml->QueryDoubleAttribute("latitude", &double_attribute) == TIXML_SUCCESS ||
				xml->QueryDoubleAttribute("lat", &double_attribute) == TIXML_SUCCESS)
				set_lat(double_attribute);
			if (xml->QueryDoubleAttribute("longitude", &double_attribute) == TIXML_SUCCESS ||
				xml->QueryDoubleAttribute("lon", &double_attribute) == TIXML_SUCCESS)
				set_lon(double_attribute);			
			
			bool bool_attribute;

			if (xml->QueryBoolAttribute("sunny", &bool_attribute) == TIXML_SUCCESS)
				set_sunny(bool_attribute);
		}
	}

	ImageInfo::ImageInfo(const ImageInfo& info):
		fidx(-1), file_name(NULL), time(NULL), color_space(NULL), depth(NULL),
		pixel_size(NULL), x_origin(NULL), y_origin(NULL),
		latitude(NULL),	longitude(NULL), sunny(NULL)			 
	{
		fidx = info.fidx;

		if(info.file_name) file_name = new bfs::path(*info.file_name);
		if(info.time) time = new bpt::ptime(*info.time);	
		if(info.color_space) color_space = new string(*info.color_space);
		if(info.depth) depth = new string(*info.depth);		
		if(info.pixel_size) pixel_size = new double(*info.pixel_size);	
		if(info.x_origin) x_origin = new double(*info.x_origin);	
		if(info.y_origin) y_origin = new double(*info.y_origin);	
		if(info.latitude) latitude = new double(*info.latitude);	
		if(info.longitude) longitude = new double(*info.longitude);	
		if(info.sunny) sunny = new bool(*info.sunny);				
	}

	ImageInfo::~ImageInfo()
	{
		delete file_name;
		delete time;	
		delete color_space;
		delete depth;		
		delete pixel_size;	
		delete x_origin;	
		delete y_origin;	
		delete latitude;	
		delete longitude;	
		delete sunny;		
	}

	TiXmlElement* ImageInfo::xml(const ObjectCollection& oc) const
	{
		TiXmlElement* frame = new TiXmlElement("frame");
		
		frame->SetAttribute("number", fidx);		

		if (file_name) frame->SetAttribute("file", file_name->string().c_str());
		if (time) frame->SetAttribute("utc", bpt::to_simple_string(*time).c_str());
		if (color_space) frame->SetAttribute("color", color_space->c_str());
		if (depth) frame->SetAttribute("depth", depth->c_str());
		if (pixel_size) frame->SetDoubleAttribute("pixel_size", *pixel_size);
		if (x_origin) frame->SetDoubleAttribute("x", *x_origin);
		if (y_origin) frame->SetDoubleAttribute("y", *y_origin);
		if (latitude) frame->SetDoubleAttribute("lat", *latitude);
		if (longitude) frame->SetDoubleAttribute("lon", *longitude);
		if (sunny) frame->SetAttribute("sunny", *sunny);
		
		// Objekte als xml serialisieren
		TiXmlElement* obj = oc.xml(fidx);
		if (obj) frame->LinkEndChild(obj);		
		
		return frame;
	}

	///////////////////////////////////////////////////////
	
	double SequenceInfo::get_dt(int fidx1, int fidx2) const
	{
		if (!has_img_info(fidx1) ||
			!info.find(fidx1)->second->time ||
			!has_img_info(fidx2) ||
			!info.find(fidx2)->second->time)
		{
			return 0;
		}
		else
		{
			return (*info.find(fidx2)->second->time - *info.find(fidx1)->second->time).total_milliseconds() / 1000.0;			
		}
	}

	bool SequenceInfo::init(const bfs::path& info_xml, bool keep_objects)
	{
		if (!bfs::exists(info_xml)) return false;

		TiXmlDocument xml;
		if (!xml.LoadFile(info_xml.string().c_str())) return false;
		
		TiXmlElement* dataset = xml.RootElement();
		if (dataset == NULL || dataset->ValueStr() != "dataset") return false;		
		
		dataset->QueryStringAttribute("name", &name);
		dataset->QueryStringAttribute("description", &description);
		
		// Externe CVML-Sequenzdateien enthalten oft nicht alle Infos in den einzelnen Frames.
		// Die Infos sollten daher im dataset-Tag als Attribut ergänzt werden und werden dann automatisch den frames hinzugefügt.
		
		double milliseconds_per_frame(0);
		{
			double frame_rate(0);
			dataset->QueryDoubleAttribute("frame_rate", &frame_rate);
			if (frame_rate>0) milliseconds_per_frame = 1000.0 / frame_rate;
		}
		
		vector<bfs::path> img_path;
		{
			string img_dir;
			dataset->QueryStringAttribute("img_dir", &img_dir);
			if (!img_dir.empty())
			{
				bfs::path dir(img_dir);
				if (bfs::exists(dir) && bfs::is_directory(dir))
				{
					for (bfs::directory_iterator dir_itr(dir), end_itr; dir_itr != end_itr; dir_itr++) 
						img_path.push_back(dir_itr->path());					
				}
			}
		}

		this->info.clear();			

		int idx(0);
		for (TiXmlElement* frame = dataset->FirstChildElement(); frame; frame = frame->NextSiblingElement())
		{
			ImageInfo_p img_info(new ImageInfo(frame));
			
			// Bei Bedarf werden fehlende Infos ergänzt

			if (img_info->fidx == -1)
				img_info->fidx = idx;
			
			if (!img_info->time && milliseconds_per_frame>0)
				img_info->set_time(bpt::ptime(bpt::min_date_time) + bpt::milliseconds(milliseconds_per_frame*idx));

			if (!img_info->file_name && int(img_path.size())>idx)
				img_info->set_file_name(img_path[idx]);

			add_img_info(img_info);
			idx++;				
		}
		
		// Eventuell enthaltene Objekte zwischenspeichern
		if (keep_objects)
			this->obj_list.init(info_xml);

		return true;
	}

	bool SequenceInfo::save(const bfs::path& file, const ObjectCollection* oc) const
	{
		if (this->count_images() == 0) return false;

		TiXmlDocument xml;
		xml.LinkEndChild(new TiXmlDeclaration("1.0", "utf-8", ""));
		
		// Mit eigenen oder neu zugewiesenen Objekten abspeichern
		if (oc == NULL)
			xml.LinkEndChild(this->xml(this->obj_list));
		else
			xml.LinkEndChild(this->xml(*oc));

		if (xml.SaveFile(file.string().c_str())) 
			return true;
		else 
			return false;
	}
	
	TiXmlElement* SequenceInfo::xml(const ObjectCollection& obj) const
	{
		TiXmlElement* dataset = new TiXmlElement("dataset");

		if (!this->name.empty()) dataset->SetAttribute("name", this->name);
		if (!this->description.empty()) dataset->SetAttribute("description", this->description);

		// Jede Bildinfo einzelnd serialisieren
		BOOST_FOREACH(const ImageInfoMap::value_type img_info, info) 
		{
			dataset->LinkEndChild( img_info.second->xml(obj) );
		}
		
		return dataset;
	}	

	bool SequenceInfo::load_images(Sequence* seq) const
	{
		if (seq == NULL || info.empty()) return false;

		BOOST_FOREACH(const ImageInfoMap::value_type& pair, info) 
		{
			cv::Mat frame = pair.second->load_image();
			if (frame.empty()) return false;	// Abbruch, falls Bilder nicht geladen werden können
			
			(*seq)[pair.second->fidx] = frame;	
		}	

		return true;
	}

}	// namespace fs