/*! \file
\author Florian Schmidt
\date 26.10.2010
*/

// Boost
#include <boost\foreach.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\math\special_functions\round.hpp>
namespace bm = boost::math;

// C/C++ Standard Library
#include <string>
#include <cmath>
#include <limits>
#include <cassert>
using namespace std;

// Eigene Klassen
#include "Object.h"
#include "ImageInfo.h"

namespace fs
{
	TiXmlElement* Representation::get_bbox_xml() const
	{
		TiXmlElement* xml = new TiXmlElement("box");

		cv::Rect_<double> bbox = get_bbox();
		xml->SetDoubleAttribute("xc", bbox.x + bbox.width/2.);
		xml->SetDoubleAttribute("yc", bbox.y + bbox.height/2.);
		xml->SetDoubleAttribute("w", bbox.width);
		xml->SetDoubleAttribute("h", bbox.height);

		return xml;
	}

	Representation_p Representation::make_rep(TiXmlElement* xml)
	{
		Representation_p rep;
		
		string type;
		if (xml->ValueStr() != "representation" || xml->QueryStringAttribute("type", &type) != TIXML_SUCCESS) return rep;

		if (type == "Point")
			rep = Representation_p(new Point(xml));

		else if (type == "Circle")
			rep = Representation_p(new Circle(xml));

		else if (type == "Rectangle")
			rep = Representation_p(new Rectangle(xml));

		else if (type == "RotatedRectangle")
			rep = Representation_p(new RotatedRectangle(xml));

		return rep;
	}

	/////

	Point::Point(TiXmlElement* xml) : Representation(0,0)
	{
		double x,y;
		if (xml->QueryDoubleAttribute("xc", &x) == TIXML_SUCCESS) set_x(x);
		if (xml->QueryDoubleAttribute("yc", &y) == TIXML_SUCCESS) set_y(y);
	}

	TiXmlElement* Point::xml() const
	{
		TiXmlElement* xml = Representation::xml();

		xml->SetAttribute("type", "Point");
		xml->SetDoubleAttribute("xc", get_x());
		xml->SetDoubleAttribute("yc", get_y());
		
		return xml;
	}

	void Point::draw(cv::Mat& img, const cv::Scalar& color, int thickness, int line_type) const
	{
		const int offset=4;
		cv::line(img, get_center() + cv::Point2d(-offset,-offset), get_center() + cv::Point2d(+offset,+offset), color, thickness, line_type);
		cv::line(img, get_center() + cv::Point2d(+offset,-offset), get_center() + cv::Point2d(-offset,+offset), color, thickness, line_type);			
	}		

	/////

	Circle::Circle(TiXmlElement* xml) : Representation(0,0), radius(0)
	{
		double x, y, r;
		if (xml->QueryDoubleAttribute("xc", &x) == TIXML_SUCCESS) set_x(x);
		if (xml->QueryDoubleAttribute("yc", &y) == TIXML_SUCCESS) set_y(y);
		if (xml->QueryDoubleAttribute("r", &r) == TIXML_SUCCESS) set_radius(r);
	}

	TiXmlElement* Circle::xml() const
	{
		TiXmlElement* xml = Representation::xml();

		xml->SetAttribute("type", "Circle");
		xml->SetDoubleAttribute("xc", get_x());
		xml->SetDoubleAttribute("yc", get_y());
		xml->SetDoubleAttribute("r", get_radius());
		
		return xml;
	}

	/////

	Rectangle::Rectangle(TiXmlElement* xml) : Representation(0,0), width(0), height(0)
	{
		double x, y, w, h;
		if (xml->QueryDoubleAttribute("xc", &x) == TIXML_SUCCESS) set_x(x);
		if (xml->QueryDoubleAttribute("yc", &y) == TIXML_SUCCESS) set_y(y);
		if (xml->QueryDoubleAttribute("w", &w) == TIXML_SUCCESS) set_width(w);
		if (xml->QueryDoubleAttribute("h", &h) == TIXML_SUCCESS) set_height(h);
	}

	TiXmlElement* Rectangle::xml() const
	{
		TiXmlElement* xml = Representation::xml();

		xml->SetAttribute("type", "Rectangle");
		xml->SetDoubleAttribute("xc", get_x());
		xml->SetDoubleAttribute("yc", get_y());
		xml->SetDoubleAttribute("w", get_width());
		xml->SetDoubleAttribute("h", get_height());
		
		return xml;
	}

	void Rectangle::get_contour(vector<cv::Point2d>& points) const
	{
		cv::Point2d pt(0,0);

		// Oben Rechts
		pt.x = get_x() + 0.5*get_width();
		pt.y = get_y() - 0.5*get_height();
		points.push_back(pt);

		// Unten Rechts
		pt.x = get_x() + 0.5*get_width();
		pt.y = get_y() + 0.5*get_height();
		points.push_back(pt);
		
		// Unten Links
		pt.x = get_x() - 0.5*get_width();
		pt.y = get_y() + 0.5*get_height();
		points.push_back(pt);

		// Oben Links
		pt.x = get_x() - 0.5*get_width();
		pt.y = get_y() - 0.5*get_height();
		points.push_back(pt);
	}

	/////

	RotatedRectangle::RotatedRectangle(TiXmlElement* xml) : Rectangle(xml), orientation(0)
	{
		double o;
		if (xml->QueryDoubleAttribute("o", &o) == TIXML_SUCCESS) set_orientation_deg(o);	
		normalize_orientation();
	}

	TiXmlElement* RotatedRectangle::xml() const
	{
		TiXmlElement* xml = Rectangle::xml();

		xml->SetAttribute("type", "RotatedRectangle");
		xml->SetDoubleAttribute("o", get_orientation_deg());
		
		return xml;
	}

	void RotatedRectangle::get_contour(vector<cv::Point2d>& points) const
	{
		// Eckpunkte berechnen

		cv::Point2d pt(0,0);

		const double cos_2 = 0.5*std::cos(get_orientation_rad());
		const double sin_2 = 0.5*std::sin(get_orientation_rad());		

		// oben rechts
		pt.x = get_x() + cos_2*get_height() - sin_2*get_width();
		pt.y = get_y() + sin_2*get_height() + cos_2*get_width();
		points.push_back(pt);

		// unten rechts
		pt.x = get_x() - cos_2*get_height() - sin_2*get_width();
		pt.y = get_y() - sin_2*get_height() + cos_2*get_width();
		points.push_back(pt);

		// unten links
		pt.x = get_x() - cos_2*get_height() + sin_2*get_width();
		pt.y = get_y() - sin_2*get_height() - cos_2*get_width();
		points.push_back(pt);

		// oben links
		pt.x = get_x() + cos_2*get_height() + sin_2*get_width();
		pt.y = get_y() + sin_2*get_height() - cos_2*get_width();	
		points.push_back(pt);
	}

	cv::Rect_<double> RotatedRectangle::get_bbox() const 
	{
		vector<cv::Point2d> pts;
		get_contour(pts);

		double min_x(pts[0].x), min_y(pts[0].y), max_x(pts[0].x), max_y(pts[0].y);

		min_x = std::min(min_x, pts[1].x);
		min_y = std::min(min_y, pts[1].y);
		max_x = std::max(max_x, pts[1].x);
		max_y = std::max(max_y, pts[1].y);

		min_x = std::min(min_x, pts[2].x);
		min_y = std::min(min_y, pts[2].y);
		max_x = std::max(max_x, pts[2].x);
		max_y = std::max(max_y, pts[2].y);

		min_x = std::min(min_x, pts[3].x);
		min_y = std::min(min_y, pts[3].y);
		max_x = std::max(max_x, pts[3].x);
		max_y = std::max(max_y, pts[3].y);

		return cv::Rect_<double>(min_x, min_y, max_x-min_x, max_y-min_y);
	}

	void RotatedRectangle::draw(cv::Mat& img, const cv::Scalar& color, int thickness, int line_type) const 
	{
		vector<cv::Point2d> pts;
		get_contour(pts);

		cv::line(img, pts[0], pts[1], color, thickness, line_type);
		cv::line(img, pts[1], pts[2], color, thickness, line_type);
		cv::line(img, pts[2], pts[3], color, thickness, line_type);
		cv::line(img, pts[3], pts[0], color, thickness, line_type);

		// Spitze zeigt Orientierung an

		double x = 0.5*(pts[0].x+pts[3].x) + std::cos(orientation)*get_width()*0.5;
		double y = 0.5*(pts[0].y+pts[3].y) + std::sin(orientation)*get_width()*0.5;
		cv::Point2d tip(x, y);		

		cv::line(img, pts[0], tip, color, thickness, line_type);
		cv::line(img, tip, pts[3], color, thickness, line_type);
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	unsigned long Object::id_counter = 0;	

	int Object::get_first_fidx() const
	{ 
		int fidx = INT_MAX;
		
		if (!rep.empty()) fidx = std::min(fidx, rep.begin()->first);
		if (!detection_score.empty()) fidx = std::min(fidx, detection_score.begin()->first);
		if (!track_score.empty()) fidx = std::min(fidx, track_score.begin()->first);
		if (fidx == INT_MAX) fidx = 0;

		return fidx;
	}	

	int Object::get_last_fidx() const
	{
		int fidx = 0;
		
		if (!rep.empty()) fidx = std::max(fidx, rep.rbegin()->first);
		if (!detection_score.empty()) fidx = std::max(fidx, detection_score.rbegin()->first);
		if (!track_score.empty()) fidx = std::max(fidx, track_score.rbegin()->first);

		return fidx;
	}	
			
	Object::Object(unsigned long* id, int* fidx, Representation_p representation, double* score)
	{		
		if (id)
		{
			this->id = *id;	
			if (*id>id_counter) id_counter=*id;
		}
		else
			this->id = ++id_counter;

		if (fidx)
		{
			if (representation.get()) rep[*fidx] = representation;
			if (score) detection_score[*fidx] = *score;			
		}
	}	
	
	cv::Point2d Object::get_move(int fidx) const
	{
		// Bewegung kann nur ermittelt werden, wenn der aktuelle und der vorhergehende Zeitpunkt bekannt sind

		if (!has_pos(fidx) || !has_pos(fidx-1)) 
			return cv::Point(0,0);
		else
			return get_pos(fidx) - get_pos(fidx-1);
	}

	TiXmlElement* Object::xml(int fidx) const
	{
		TiXmlElement* obj = NULL; 

		obj = new TiXmlElement("object");
		obj->SetAttribute("id", get_id());

		if (has_rep(fidx))
		{
			obj->LinkEndChild(rep.find(fidx)->second->get_bbox_xml());
			obj->LinkEndChild(rep.find(fidx)->second->xml());
		}

		if (has_detection_score(fidx))
		{
			TiXmlElement* score = new TiXmlElement("detection_score");
			score->SetDoubleAttribute("value", get_detection_score(fidx));			
			obj->LinkEndChild(score);
		}

		if (has_track_score(fidx))
		{
			TiXmlElement* ts = new TiXmlElement("track_score");
			ts->SetDoubleAttribute("value", get_track_score(fidx));			
			obj->LinkEndChild(ts);
		}

		return obj;
	}

	void Object::draw_id(cv::Mat& img, int fidx, const cv::Scalar& color, int thickness, int line_type) const
	{
		if (this->has_bbox(fidx))
		{
			cv::Rect_<double> bbox = get_bbox(fidx);
			cv::putText(img, boost::lexical_cast<string>(this->id), cv::Point(bbox.br().x+1, bbox.tl().y-2), cv::FONT_HERSHEY_SIMPLEX, 0.25, color, thickness, line_type);
		}
	}

	void Object::draw(cv::Mat& img, int fidx, const cv::Scalar& color, int thickness, bool show_id, int line_type) const
	{
		if (has_rep(fidx))
		{
			rep.find(fidx)->second->draw(img, color, thickness, line_type);			
			if (show_id) draw_id(img, fidx, color, thickness);			
		};
	}

	void Object::draw_bbox(cv::Mat& img, int fidx, const cv::Scalar& color, int thickness, bool show_id, int line_type) const
	{
		if (has_rep(fidx))
		{
			rep.find(fidx)->second->draw_bbox(img, color, thickness, line_type);			
			if (show_id) draw_id(img, fidx, color, thickness);			
		};
	}

	void Object::draw_track(cv::Mat& img, int fidx1, int fidx2, const cv::Scalar& color, int thickness, int line_type) const
	{
		if (fidx1<=fidx2 && !img.empty())
		{
			for (;fidx1<=fidx2; fidx1++)
			{
				if (fidx1<fidx2)
				{
					// Verbindung zw. zwei Standpunkten einzeichnen
					if (has_pos(fidx1) && has_pos(fidx1+1))
						cv::line(img, get_pos(fidx1), get_pos(fidx1+1), color, thickness, line_type);

					// Falls nur eine Position bekannt ist, wird eine punktförmige "Linie" eingezeichnet
					else if (has_pos(fidx1) && !has_pos(fidx1+1))
						cv::line(img, get_pos(fidx1), get_pos(fidx1), color, thickness, line_type);

					else if (!has_pos(fidx1) && has_pos(fidx1+1))
						cv::line(img, get_pos(fidx1+1), get_pos(fidx1+1), color, thickness, line_type);
				}

				// Punkt-Linie zeichnen, wenn Zeitpunkte identisch
				else if (fidx1==fidx2 && has_pos(fidx1))
				{
					cv::line(img, get_pos(fidx1), get_pos(fidx1), color, thickness, line_type);
				}				
			}
		}
	}

	double Object::get_distance(const Object_p obj2, int fidx1, int fidx2) const
	{
		double dist(0);
		unsigned int dist_counter(0);

		for (int fidx=fidx1; fidx<=fidx2; fidx++)
		{
			if (this->has_pos(fidx) && obj2->has_pos(fidx))
			{
				dist_counter++;
				dist += cv::norm(this->get_pos(fidx) - obj2->get_pos(fidx));
			}
		}

		return (dist_counter==0) ? 0 : dist/dist_counter;
	}

	double Object::get_bbox_overlap(const Object_p obj2, int fidx1, int fidx2) const
	{
		double overlap = 0;
		if (fidx2<fidx1) std::swap(fidx1, fidx2);
		
		for (int fidx=fidx1; fidx<=fidx2; fidx++)
		{
			if (has_bbox(fidx) && obj2->has_bbox(fidx))
			{
				double intersection_area = (get_bbox(fidx) & obj2->get_bbox(fidx)).area();				
				if (intersection_area>0)
					overlap += intersection_area/(get_bbox_area(fidx) + obj2->get_bbox_area(fidx) - intersection_area);				
			}
		}
		
		return overlap/(fidx2-fidx1+1);
	}

	bool Object::has_pos(int fidx_start, int fidx_end) const
	{
		bool answer = false;

		for (int fidx=fidx_start; fidx<=fidx_end; fidx++)
		{
			if (has_pos(fidx)) 
				answer = true;
			else
			{
				answer = false;
				break;
			}
		}

		return answer;
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	bool ObjectCollection::init(const bfs::path& info)
	{
		if (!bfs::exists(info)) return false;
		
		TiXmlDocument xml;
		if (!xml.LoadFile(info.string().c_str())) return false;

		TiXmlElement* dataset = xml.RootElement();
		if (dataset==NULL || dataset->ValueStr() != "dataset") return false;

		objects.clear();
		std::map<unsigned long, Object_p> obj_list;	// Temporäre Hilfsliste

		for (TiXmlElement* frame = dataset->FirstChildElement(); frame; frame = frame->NextSiblingElement())
		{
			int fidx;
			frame->QueryIntAttribute("number", &fidx);
			 
			for (TiXmlElement* child = frame->FirstChildElement(); child; child = child->NextSiblingElement())
			{					
				if (child->ValueStr() == "objectlist")
				{				
					// Eigenschaften jedes Objektes auslesen

					for (TiXmlElement* obj = child->FirstChildElement(); obj; obj = obj->NextSiblingElement())
					{
						unsigned long id;						
						if (obj->QueryValueAttribute<unsigned long>("id", &id) == TIXML_NO_ATTRIBUTE) 
							id = ++fs::Object::id_counter;

						if (!obj_list.count(id))
							obj_list[id] = Object_p(new Object(&id));
						
						double score(DBL_MIN);	
						obj->QueryDoubleAttribute("confidence", &score);

						TiXmlElement *xml_bbox(NULL), *xml_rep(NULL);

						for (TiXmlElement *obj_att = obj->FirstChildElement(); obj_att; obj_att = obj_att->NextSiblingElement())
						{
							if (obj_att->ValueStr() == "box") 
								xml_bbox = obj_att;

							else if (obj_att->ValueStr() == "representation")
								xml_rep = obj_att;							

							else if (obj_att->ValueStr() == "track_score")
							{
								double ts(0);
								obj_att->QueryDoubleAttribute("value", &ts);
								obj_list[id]->set_track_score(fidx, ts);
							}

							// Zusätzliche Varianten, wie der "detection score" in der XML-Datei enthalten sein kann
							else if (obj_att->ValueStr() == "confidence")	
							{								
								obj_att->QueryDoubleAttribute("score", &score);						
							}	
							else if (obj_att->ValueStr() == "score")
							{
								if (obj_att->GetText())
									score = boost::lexical_cast<double>(obj_att->GetText());
								else
									obj_att->QueryDoubleAttribute("value", &score);
							}	
							else if (obj_att->ValueStr() == "detection_score")
							{
								if (obj_att->GetText())
									score = boost::lexical_cast<double>(obj_att->GetText());
								else
									obj_att->QueryDoubleAttribute("value", &score);
							}	
						}

						if (score!=DBL_MIN) 
							obj_list[id]->set_detection_score(fidx, score);			

						if (xml_rep != NULL)
						{
							Representation_p rep = Representation::make_rep(xml_rep);
							if (rep!=NULL) 
								obj_list[id]->set_rep(fidx, rep);
						}
						else if (xml_bbox)
						{
							// Alle Objekte, die keine "representation" enthalten, sondern nur eine "bbox", erhalten ein Rechteck als Objektrepräsentation
							Representation_p rect = Representation_p(new Rectangle(xml_bbox));
							obj_list[id]->set_rep(fidx, rect);
						}						
					}
				}	
			}
		}		

		for (std::map<unsigned long, Object_p>::iterator itr=obj_list.begin(); itr!=obj_list.end(); itr++)
			this->objects.push_back(itr->second);		

		return true;
	}

	bool ObjectCollection::save(const bfs::path& info_xml, bool cleanup_id)
	{
		// IDs neu vergeben
		if (cleanup_id)
		{
			for (unsigned int i=0; i<objects.size(); i++)	
				objects[i]->set_id(i+1);
		}

		SequenceInfo info;
		if (!info.init(info_xml, false)) return false;

		TiXmlDocument xml;
		xml.LinkEndChild(new TiXmlDeclaration("1.0", "utf-8", ""));
		xml.LinkEndChild(info.xml(*this));

		if (!xml.SaveFile(info_xml.string().c_str())) return false;

		return true;
	}

	void ObjectCollection::get_objects(int fidx1, int fidx2, ObjectCollection* oc) const
	{
		if (!oc) return;

		vector<Object_p> obj_list;

		BOOST_FOREACH(Object_p obj, objects) 
		{
			if (obj->has_pos(fidx1, fidx2)) 
				obj_list.push_back(obj);
		}			

		oc->objects = obj_list;
	}

	unsigned int ObjectCollection::count_objects(int fidx) const
	{
		ObjectCollection tmp;
		this->get_objects(fidx, &tmp);
		return tmp.count_objects();
	}
		
	TiXmlElement* ObjectCollection::xml(int fidx) const
	{
		ObjectCollection olist;
		this->get_objects(fidx, &olist);

		TiXmlElement* xml_obj_list = NULL;

		if (olist.count_objects())
		{
			xml_obj_list = new TiXmlElement("objectlist");
			
			BOOST_FOREACH(Object_p obj, objects)
			{
				if (obj->has_pos(fidx))
					xml_obj_list->LinkEndChild(obj->xml(fidx));
			}
		}

		return xml_obj_list;
	}

	Object_p ObjectCollection::get_object(unsigned long id) const
	{
		BOOST_FOREACH(Object_p obj, objects)
		{
			if (obj->get_id() == id) return obj;
		}

		return Object_p();
	}

	bool ObjectCollection::delete_obj(unsigned long id)
	{
		for (unsigned int i=0; i<objects.size(); i++)
		{
			if (objects[i]->get_id() == id)
			{
				objects.erase(objects.begin()+i);
				return true;
			}
		}

		return false;
	}

} // namespace fs