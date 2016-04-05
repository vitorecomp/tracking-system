/*! \file
\author Florian Schmidt
\date 26.10.2010
*/

#ifndef OBJECT_H
#define OBJECT_H

// OpenCV
#include <opencv.hpp>

// Boost
#include <boost\foreach.hpp>
#include <boost\filesystem.hpp>
namespace bfs = boost::filesystem;
#include <boost\smart_ptr.hpp>
#include <boost\math\special_functions\round.hpp>
namespace bm = boost::math;

// STL
#include <vector>
#include <limits>
#include <numeric>
using namespace std;

// TinyXML
#include "tinyxml.h"

namespace fs
{

#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif

	///////////////////////////////////////////////////////////////////

	//! Abstrakte Basisklasse für Objektrepräsentationen im Bild.
	class Representation
	{
	public:
		Representation(double x=0, double y=0) : x(x), y(y) {}
		Representation(const cv::Point2d& pt) : x(pt.x), y(pt.y) {}
		virtual ~Representation() {}	
		Representation(const Representation& inst) { *this = inst; }
		
		//! Bildkoordinate des Bezugspunktes/Mittelpunktes/Schwerpunktes setzen
		void set_center(const cv::Point2d& pt) { x = pt.x; y = pt.y; }	
		void set_x(double val) { x = val; }
		void set_y(double val) { y = val; }

		cv::Point2d get_center() const { return cv::Point2d(x,y); }	
		double get_x() const { return x; }
		double get_y() const { return y; }
		virtual cv::Rect_<double> get_bbox() const = 0;

		double get_bbox_area() const { return get_bbox().area(); }
		double get_bbox_diag() const { cv::Rect_<double> rect = get_bbox(); return std::sqrt(rect.height*rect.height + rect.width*rect.width);}

		virtual TiXmlElement* xml() const { return new TiXmlElement("representation"); }
		TiXmlElement* get_bbox_xml() const;		//!< Erzeugt XML-Element aus BBOX der Objektrepräsentation
		
		virtual void draw(cv::Mat& img, const cv::Scalar& color=cv::Scalar(255,255,255), int thickness=1, int line_type=8) const = 0;
		void draw_bbox(cv::Mat& img, const cv::Scalar& color=cv::Scalar(255,255,255), int thickness=1, int line_type=8) const { cv::rectangle(img, get_bbox(), color, thickness, line_type); }		

		static boost::shared_ptr<Representation> make_rep(TiXmlElement* xml);	//!< XML-Beschreibung in Repräsentation umwandeln

	private:
		
		double x;	//!< X-Bildkoordinate des Bezugspunktes, Mittelpunktes oder Schwerpunktes.
		double y;	//!< Y-Bildkoordinate des Bezugspunktes, Mittelpunktes oder Schwerpunktes.
	};

	//! Sicherer Basisklassenzeiger auf Objektrepräsentation
	typedef boost::shared_ptr<Representation> Representation_p;	
		    
	//! Punkt (Bildkoordinate) als Objektrepräsentation
	class Point : public Representation
	{
	public:
		Point(double x=0, double y=0) : Representation(x,y) {}
		Point(const cv::Point2d pt) : Representation(pt) {}
		Point(TiXmlElement* xml);
		Point(const Point& inst) { *this = inst; }
		~Point() {}

		//! Erzeugt BBOX ohne Höhe und Breite
		cv::Rect_<double> get_bbox() const { return cv::Rect_<double>(get_x(), get_y(), 0, 0); }
		
		TiXmlElement* xml() const;

		//! Zeichnet Punkt als Kreuz ins Bild
		void draw(cv::Mat& img, const cv::Scalar& color=cv::Scalar(255,255,255), int thickness=1, int line_type=8) const;
	};

	//! Kreis als Objektrepräsenation
	class Circle : public Representation
	{
	public:
		Circle(double xc=0, double yc=0, double radius=0) : Representation(xc,yc), radius(radius) {}
		Circle(const cv::Point2d& pt, double radius) : Representation(pt), radius(radius) {}
		Circle(TiXmlElement* xml);
		Circle(const Circle& inst) { *this = inst; }
		~Circle() {}

		void set_radius(double val) { radius = val; }	//!< Setzt den Kreisradius. \param val Radius in Pixel.

		double get_radius() const { return radius; }
		cv::Rect_<double> get_bbox() const { return cv::Rect_<double>(get_x()-radius, get_y()-radius, 2*radius, 2*radius); }

		TiXmlElement* xml() const;
		void draw(cv::Mat& img, const cv::Scalar& color=cv::Scalar(255,255,255), int thickness=1, int line_type=8) const { cv::circle(img, get_center(), get_radius(), color, thickness, line_type); }		

	private:
		double radius;
	};

	//! Rechteck als Objektrepräsentation
	class Rectangle : public Representation
	{
	public:
		Rectangle(double xc=0, double yc=0, double width=0, double height=0) : Representation(xc,yc), width(width), height(height) {}
		Rectangle(const cv::Point2d& center, const cv::Size& size) : Representation(center), width(size.width), height(size.height) {}
		Rectangle(const cv::Rect_<double> rect) : Representation(rect.x+0.5*rect.width,rect.y+0.5*rect.height), width(rect.width), height(rect.height) {}
		Rectangle(TiXmlElement* xml);
		Rectangle(const Rectangle& inst) { *this = inst; }
		~Rectangle() {}
				
		void set_width(double val) { width = val; }
		void set_height(double val) { height = val; }
		void set_size(const cv::Size_<double>& size) { width = size.width; height = size.height; }	//!< \param size OpenCV-Size
		void set_rect(const cv::Rect_<double>& rect) { width = rect.width; height = rect.height; set_x(rect.x+width/2.); set_y(rect.y+height/2.); } //!< \param rect OpenCV-Rechteck

		double get_width() const { return width; }
		double get_height() const { return height; }
		cv::Size_<double> get_size() const { return cv::Size_<double>(width, height); }
		cv::Rect_<double> get_bbox() const { return cv::Rect_<double>(get_x()-width/2., get_y()-height/2., width, height); }	
		void get_contour(vector<cv::Point2d>& points) const;	//!< Gibt alle vier Eckpunkte in einem Vektor zurück. Reihenfolge: or, ur, ul, ol

		TiXmlElement* xml() const;
		void draw(cv::Mat& img, const cv::Scalar& color=cv::Scalar(255,255,255), int thickness=1, int line_type=8) const { cv::rectangle(img, get_bbox(), color, thickness, line_type); }		

	private:
		double width;
		double height;
	};

	//! Rotiertes Rechteck als Objektrepräsentation. 
	class RotatedRectangle : public Rectangle
	{
	public:
		RotatedRectangle(double xc=0, double yc=0, double width=0, double height=0, double orientation_rad=0) : Rectangle(xc,yc,width,height), orientation(orientation_rad) { normalize_orientation(); }
		RotatedRectangle(const cv::Point2d& center, const cv::Size& size, double orientation_rad) : Rectangle(center, size), orientation(orientation_rad) { normalize_orientation(); }
		RotatedRectangle(const cv::Rect_<double>& rect, double orientation_rad) : Rectangle(rect), orientation(orientation_rad) {}
		RotatedRectangle(TiXmlElement* xml);		
		RotatedRectangle(const RotatedRectangle& inst) { *this = inst; }
		~RotatedRectangle() {}

		//! Setzt Ausrichtung des Rechtecks. Drehwinkel bezüglich der X-Achse im linkshändischen Bildkoordinatensystem
		void set_orientation_rad(double rad_val) { orientation = rad_val; normalize_orientation(); }
		void set_orientation_deg(double deg_val) { orientation = deg_val/180*PI; normalize_orientation(); }		

		double get_orientation_rad() const { return orientation; }
		double get_orientation_deg() const { return orientation/PI*180; }
		cv::Rect_<double> get_bbox() const; 
		void get_contour(vector<cv::Point2d>& points) const;	//!< Gibt alle vier Eckpunkte in einem Vektor zurück. Reihenfolge: or, ur, ul, ol
				
		TiXmlElement* xml() const;
		void draw(cv::Mat& img, const cv::Scalar& color=cv::Scalar(255,255,255), int thickness=1, int line_type=8) const ;
		
	private:		
		void normalize_orientation() { orientation -= std::floor(orientation/(2*PI))*2*PI; } //!< Normalisiert den Winkel auf Wertebereich von [0; 2*PI)

		double orientation;	//!< Ausrichtung in [Radiant]. Positiver Drehsinn entsprechend Bildkoordinatensystem.
	};

	///////////////////////////////////////////////////////////////////

	//! Klasse beschreibt ein Objekt zu diskreten Zeitpunkten
	class Object
	{
	public:
		
		Object() { id = ++id_counter; }		
		Object(unsigned long* id, int* fidx=NULL, Representation_p representation=Representation_p(), double* detection_score=NULL);			
		Object(const Object& obj) { *this = obj; }
		~Object() {}
		
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		void set_id(unsigned long id) { this->id = id; }
		void set_rep(int fidx, Representation_p representation) { rep[fidx] = representation; }	//!< Setzt die Objektrepräsentation für einen bestimmten Zeitpunkt.
		void set_detection_score(int fidx, double score) { detection_score[fidx] = score; }		
		void set_track_score(int fidx, double score) { track_score[fidx] = score; }

		void erase_rep(int fidx) { rep.erase(fidx); }
		void erase_detection_score(int fidx) { detection_score.erase(fidx); }
		void erase_track_score(int fidx) { track_score.erase(fidx); }
		void erase(int fidx) { erase_rep(fidx); erase_detection_score(fidx); erase_track_score(fidx); }

		bool has_rep(int fidx) const { return (rep.count(fidx)!=0); }
		bool has_bbox(int fidx) const { return has_rep(fidx); }
		bool has_pos(int fidx) const { return has_rep(fidx); }
		bool has_pos(int fidx_start, int fidx_end) const;	//!< Wahr, falls Position für gesamte Zeitspanne vorhanden
		bool has_detection_score(int fidx) const { return (detection_score.count(fidx)!=0); }
		bool has_track_score(int fidx) const { return (track_score.count(fidx)!=0); }

		unsigned long get_id() const { return id; }		

		Representation_p get_rep(int fidx) const { return (has_rep(fidx)) ? rep.find(fidx)->second : Representation_p(); }	/*!< \return Objektrepräsenation für Zeitpunkt oder leerer Zeiger */
		
		double get_xc(int fidx) const { return (has_rep(fidx))? rep.find(fidx)->second->get_x() : 0; }	/*!< \return X-Koordinate des Objektzentrums oder den Wert 0 */
		double get_yc(int fidx) const { return (has_rep(fidx))? rep.find(fidx)->second->get_y() : 0; }	/*!< \return Gibt Y-Koordinate des Objektzentrums für Zeitpunkt 'fidx' zurück oder den Wert 0 */
		cv::Point2d get_pos(int fidx) const { return (has_rep(fidx)) ? rep.find(fidx)->second->get_center() : cv::Point2d(); } /*!< \return Punkt-Koordinate des Objektzentrums oder die Koordinate (0,0) */
		
		double get_width() const { return (rep.empty()) ? 0 : rep.begin()->second->get_bbox().width; } /*!< \return BBOX-Breite der ersten gespeicherten Objektrepräsentation oder 0 */
		double get_width(int fidx) const { return (has_rep(fidx)) ? rep.find(fidx)->second->get_bbox().width : 0; }	
		 
		double get_height() const { return (rep.empty()) ? 0 : rep.begin()->second->get_bbox().height; }	/*!< \return BBOX-Höhe der ersten gespeicherten Objektrepräsentation oder 0 */
		double get_height(int fidx) const { return (has_rep(fidx)) ? rep.find(fidx)->second->get_bbox().height : 0; }		

		double get_detection_score(int fidx) const { return (has_detection_score(fidx)) ? detection_score.find(fidx)->second : 0; }
		double get_track_score(int fidx) const { return (track_score.count(fidx)) ? track_score.find(fidx)->second : 0; }
		
		unsigned int count_positions() const { return this->rep.size(); }	/*!< \return Anzahl der Zeitpunkte mit Objektrepräsentation */

		cv::Point2d get_move(int fidx) const; /*!< \return Bewegung zum Zeitpunkt fidx = Pos(fidx) - Pos(fidx-1) sonst 0 */
		double get_velo(int fidx) const { return cv::norm(this->get_move(fidx)); }; /*!< \return Bewegungsstärke zum Zeitpunkt fidx */
		double get_dir(int fidx) const { return std::atan2(get_move(fidx).y, get_move(fidx).x); } /*!< \return Bewegungsrichtung zum Zeitpunkt fidx in Radiant [-pi,+pi] */

		//! Liefert Startwert für Iteration durch Objekteigenschaften
		int get_first_fidx() const;	/*!< \return Index des frühesten Zeitpunktes mit beliebiger Objekteigenschaft oder 0 */

		//! Liefert Endwert für Iteration durch Objekteigenschaften.
		int get_last_fidx() const;		/*!< \return Index des spätesten Zeitpunktes mit beliebiger Objekteigenschaft oder 0 */

		TiXmlElement* xml(int fidx) const;	/*!< \return XML-Element mit allen Objekteigenschaften für den angegebenen Zeitpunkt. */

		double get_distance (const boost::shared_ptr<Object> obj2, int fidx1, int fidx2) const;		/*!< \return Mittlerer Abstand der Objektzentren über alle Zeitpunkte an denen beide Objekte vorhanden sind oder 0 */
		double get_distance (const boost::shared_ptr<Object> obj2, int fidx) const { return get_distance(obj2, fidx, fidx); }	/*!< \return Abstand zweier Objektzentren für bestimmten Zeitpunkt oder 0 */

		double get_bbox_overlap(const boost::shared_ptr<Object> obj2, int fidx1, int fidx2) const;	/*!< \return Mittlere BBOX-Überlappung [0;1] über gesamten Zeittraum, egal ob eines der beiden Objekte (kurzzeitig) fehlt */
		double get_bbox_overlap(const boost::shared_ptr<Object> obj2, int fidx) const { return get_bbox_overlap(obj2, fidx, fidx); }	/*! \return BBOX-Überlappung [0;1] */

		cv::Rect_<double> get_bbox(int fidx) const { return (has_rep(fidx)) ? rep.find(fidx)->second->get_bbox() : cv::Rect_<double>(); }
		double get_bbox_diag(int fidx) const {return (has_rep(fidx)) ? rep.find(fidx)->second->get_bbox_diag() : 0; }
		double get_bbox_area(int fidx) const {return (has_rep(fidx)) ? rep.find(fidx)->second->get_bbox_area() : 0; }

		void draw(cv::Mat& img, int fidx, const cv::Scalar& color, int thickness=1, bool show_id = false, int line_type=8) const; 
		void draw_bbox(cv::Mat& img, int fidx, const cv::Scalar& color, int thickness=1, bool show_id = false, int line_type=8) const; 
		void draw_track(cv::Mat& img, int fidx1, int fidx2, const cv::Scalar& color, int thickness=1, int line_type=8) const;
		void draw_track(cv::Mat& img, const cv::Scalar& color, int thickness=1, int line_type=8) const {draw_track(img, get_first_fidx(), get_last_fidx(), color, thickness, line_type);}

	private:

		void draw_id(cv::Mat& img, int fidx, const cv::Scalar& color, int thickness=1, int line_type=8) const;

		unsigned long id;

		std::map<int, Representation_p> rep;		//!< Objektrepräsentation für bestimmten Zeitpunkt
		std::map<int, double> detection_score;		//!< Detektionswert für bestimmten Zeitpunkt
		std::map<int, double> track_score;			//!< Track Score Inkrement			
		
	public:

		static unsigned long id_counter;					//!< Merkt sich die bereits vergebenen Object-IDs
	};

	typedef boost::shared_ptr<Object> Object_p;				//!< Sicher Zeiger auf Objekt

	//! Sammlung von Objekten
	class ObjectCollection
	{
	public:		

		ObjectCollection() {}

		ObjectCollection(const bfs::path& info_xml) {init(info_xml);}	//!< Lässt sich mit SequenceInfo-XML-Datei initialisieren. \see fs::SequenceInfo()
		bool init(const bfs::path& info_xml);							//!< Lässt sich mit SequenceInfo-XML-Datei initialisieren. \see fs::SequenceInfo()

		ObjectCollection(const ObjectCollection& oc) { BOOST_FOREACH(Object_p obj, oc.objects) this->objects.push_back(Object_p(new Object(*obj))); }

		//! Speichert Objekte in einer SequenceInfo-XML-Datei ab. Vorhandene Objekte werden gelöscht.
		/*! \param info_xml Vollständiger Dateiname. \param cleanup_id Sorgt dafür, dass die Objekt-IDs mit 0 beginnend neu vergeben werden. */
		bool save(const bfs::path& info_xml, bool cleanup_id = false);

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		void insert(Object_p obj) { objects.push_back(obj); }
		void insert(ObjectCollection& oc) { objects.insert(objects.end(), oc.objects.begin(), oc.objects.end()); }

		Object_p get_object(unsigned long id) const;
				
		bool delete_obj(unsigned long id);

		void get_objects(int fidx, ObjectCollection* oc) const { this->get_objects(fidx, fidx, oc); }	//!< Auswahl aller Objekte zu einem bestimmten Zeitpunkt	
		void get_objects(int fidx1, int fidx2, ObjectCollection* oc) const ;	//!< Auswahl aller Objekte in einem bestimmten Zeitraum. Jedes Objekt muss zu jedem Zeitpunkt bekannt sein.

		unsigned int count_objects() const { return objects.size(); }	
		unsigned int count_objects(int fidx) const;			

		bool empty() const { return objects.empty(); }
		void clear() { objects.clear(); }

		void draw(cv::Mat& img, int fidx, const cv::Scalar& color, int thickness=1, bool show_id = false, int line_type=8) const { BOOST_FOREACH(Object_p obj, objects) obj->draw(img, fidx, color, thickness, show_id, line_type); } 
		void draw_bbox(cv::Mat& img, int fidx, const cv::Scalar& color, int thickness=1, bool show_id = false, int line_type=8) const { BOOST_FOREACH(Object_p obj, objects) obj->draw_bbox(img, fidx, color, thickness, show_id, line_type); } //! Zeichnet Objektposition (BBOX) in Bild ein 
		void draw_track(cv::Mat& img, int fidx1, int fidx2, const cv::Scalar& color, int thickness=1, int line_type=8) const { BOOST_FOREACH(Object_p obj, objects) obj->draw_track(img, fidx1, fidx2, color, thickness, line_type); } //! Zeichnet Objektpfad als Linie in Bild ein
		void draw_track(cv::Mat& img, const cv::Scalar& color, int thickness=1, int line_type=8) const { BOOST_FOREACH(Object_p obj, objects) obj->draw_track(img, color, thickness, line_type); } //! Zeichnet Objektpfad als Linie in Bild ein
					
		TiXmlElement* xml(int fidx) const;

		vector<Object_p> objects;	//!< Vector mit Objekten. Leichter zu handhaben als 'map'. Der Zugriff über die Objekt-ID ist zwar langsamer, er wird aber nicht so häufig benötigt.
	};
		
} // namespace fs
#endif 