/*! \file
\author Florian Schmidt
\date 23.08.2010
*/

#ifndef IMAGE_INFO_H
#define IMAGE_INFO_H

// OpenCV
#include <opencv.hpp>

// C/C++ Standard Library
#include <string>
#include <vector>
using namespace std;

// Boost
#include <boost\filesystem.hpp>
namespace bfs = boost::filesystem;
#include <boost\date_time.hpp>
namespace bpt = boost::posix_time;
#include <boost\smart_ptr.hpp>

// TinyXML
#include "tinyxml.h"

// Eigene Klassen
#include "Object.h"

namespace fs 
{
	//! Klasse speichert alle wichtigen Metadaten zu einem Bild
	class ImageInfo 
	{
	public:
		ImageInfo();
		ImageInfo(TiXmlElement* xml);
		ImageInfo(const ImageInfo& info);
		~ImageInfo();

		bool is_valid() const { return (file_name && !file_name->empty() && bfs::exists(*file_name)); } //!< Überprüft, ob Bilddatei existiert
		cv::Mat load_image() const { return (is_valid()) ? cv::imread(file_name->string(),-1) : cv::Mat(); }	/*!< \return Bild oder cv::Mat() */
		TiXmlElement* xml(const ObjectCollection& oc = ObjectCollection()) const;	//!< Exportiert als CVML-Frame-XML-Element. Optional können im Bild vorhandene Objekte hinzugefügt werden.

		void set_file_name(const bfs::path& file) { if (file_name) *file_name = file; else file_name = new bfs::path(file); }
		void set_time(const bpt::ptime& time) { if (this->time) *this->time=time; else this->time  = new bpt::ptime(time); }
		void set_color_space(const string& color) { if (color_space) *color_space=color; else color_space = new string(color); }
		void set_depth(const string& depth) { if (this->depth) *this->depth=depth; else this->depth = new string(depth); }
		void set_pixel_size(double gsd) { if(pixel_size) *pixel_size=gsd; else pixel_size = new double(gsd); }
		void set_x_origin(double x) { if(x_origin) *x_origin=x; else x_origin = new double(x); }
		void set_y_origin(double y) { if(y_origin) *y_origin=y; else y_origin = new double(y); }
		void set_lat(double lat) { if(latitude) *latitude=lat; else latitude = new double(lat); }
		void set_lon(double lon) { if(longitude) *longitude=lon; else longitude = new double(lon); }
		void set_sunny(bool sun) { if(sunny) *sunny=sun; else sunny = new bool(sun); }
		
		int fidx;				//!< Index des Bildes in der Sequenz

		bfs::path* file_name;	//!< Vollständiger Dateiname des Bildes
		bpt::ptime* time;		//!< Aufnahmezeitpunkt als boost::posix_time::ptime-Instanz
		string* color_space;	//!< Farbraum in dem das Bild vorliegt, z.B.: rgb, ...
		string* depth;			//!< Halcon-Farbtiefe, z.B.: byte, int8, real, ...
		double* pixel_size;		//!< Bodenauflösung in [m]
		double* x_origin;		//!< X-Weltkoordinate der linken, oberen Bildecke
		double* y_origin;		//!< Y-Weltkoordinate der linken, oberen Bildecke
		double* latitude;		//!< Geographische Breite der Kamera zum Aufnahmezeitpunkt
		double* longitude;		//!< Geographische Länge der Kamera zum Aufnahmezeitpunkt
		bool* sunny;			//!< Gibt an ob die Sonne scheint und mit Objektschatten zu rechnen ist				
	};

	typedef boost::shared_ptr<ImageInfo> ImageInfo_p; //!< Sicherer Zeiger auf ImageInfo
	typedef std::map<unsigned int, cv::Mat> Sequence;	//!< Praktische Struktur, um über Index auf Bilder der Sequenz zuzugreifen

	//! Klasse speichert alle Metaddaten zu einer Bildsequenz 
	class SequenceInfo
	{
	public:
		SequenceInfo(): name(""), description("") {}		
		SequenceInfo(const bfs::path& info_xml, bool keep_objects=true) { init(info_xml, keep_objects); }	

		//! Initialisierung mittel XML-Datei
		/*! \param info_xml Vollständiger Dateiname. \param keep_objects Gibt an, ob eventuell vorhandene Objekte ebenfalls geladen und (zwischen) gespeichert werden sollen. Wird die Sequenz nicht verändert, ist dies nicht notwendig und das Laden der XML kann beschleunigt werden. */
		bool init(const bfs::path& info_xml , bool keep_objects=true);

		//! Speichert SequenceInfo in einer XML-Datei optional zusammen mit neuen Objekten
		bool save(const bfs::path& file, const ObjectCollection* oc=NULL) const;

		//! Wandelt Sequenz in XML-Element um. Optional können Objektpositionen mit integriert werden.
		TiXmlElement* xml(const ObjectCollection& oc=ObjectCollection()) const;

		bool has_img_info(int fidx) const { return (info.count(fidx)==1); }
		bool is_valid_fidx(int fidx) const { return has_img_info(fidx); }
		ImageInfo_p get_img_info(int fidx) { return (has_img_info(fidx)) ? info.find(fidx)->second : ImageInfo_p(); }
		void add_img_info(ImageInfo_p img_info) { info[img_info->fidx] = img_info; }
		void erase_img_info(int fidx) { info.erase(fidx); }

		bool load_images(Sequence* seq) const;	
		int get_start_fidx() const { return (info.empty()) ? 0 : info.begin()->second->fidx; } 
		int get_end_fidx() const { return (info.empty()) ? 0 : info.rbegin()->second->fidx; } 
		double get_dt(int fidx1, int fidx2) const;	/*<! \brief Ermittelt Zeitunterschied [s] zwischen beliebigen Zeitpunkten/Bildern. \return Zeitunterschied [s] oder 0. */
		double get_pixel_size() const { if (info.empty() || !info.begin()->second->pixel_size) return 0; else return *info.begin()->second->pixel_size;} /*!< \return Pixelgröße in [m] oder 0, falls nicht vorhanden */
		
		unsigned int count_images() const {return info.size();}

		string name;			//!< Name der Sequenz
		string description;		//!< Beschreibung der Sequenz

	private:

		typedef std::map<unsigned int, ImageInfo_p> ImageInfoMap;

		ImageInfoMap info;			//!< Map mit Metadaten zu allen Bildern der Sequenz
		ObjectCollection obj_list;	//!< Speichert eventuell enthaltene Objekte zwischen damit sie beim Abspeichern nicht verloren gehen.		
	};

} // namespace fs

#endif