/*! /file
/brief Program illustrates the use of the classes for objects and image sequences.
/author Florian Schmidt
*/

// The following C++ librarys are necessary to run the code.

// OpenCV: http://opencv.willowgarage.com
#include <opencv.hpp>

// Boost: http://www.boost.org/ , http://www.boostpro.com/download/
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

// TinyXML 1: http://www.grinninglizard.com/tinyxml/index.html

// C/C++ Standard Library
using namespace std;

#include "Object.h"
#include "ImageInfo.h"

int main()
{	
	const bfs::path file("C:/Training/AA_Easy_01/AA_Easy_01.xml");

	fs::SequenceInfo seq_info;
	fs::ObjectCollection oc;

	if (!seq_info.init(file) || !oc.init(file))	return 0;

	cout << seq_info.name << ": " << seq_info.count_images() << " frames" << endl;
	
	const int start_fidx = seq_info.get_start_fidx();
	const int end_fidx = seq_info.get_end_fidx();
	cout << "Index of first frame: " << start_fidx << endl;
	cout << "Index of last frame: " << end_fidx << endl;
	cout << "Time between first and last frame: " << seq_info.get_dt(start_fidx, end_fidx) << "s" << endl;
	cout << "Number of oc: " << oc.count_objects() << endl;
		
	// Draw objects in image sequence

	fs::Sequence seq;
	if (!seq_info.load_images(&seq)) return 0;

	cv::namedWindow("Objects");
	for (int fidx=start_fidx; fidx<=end_fidx; fidx++)
	{
		oc.draw(seq[fidx], fidx, cv::Scalar(255,0,0), 1, true);
		oc.draw_track(seq[fidx], fidx-4, fidx, cv::Scalar(255,0,0));

		cv::imshow("Objects", seq[fidx]);
		cv::waitKey(1000);
	}	

	// Create new circle-like Object in first image

	fs::Object_p new_obj(new fs::Object());

	double x(30), y(30), r(4);
	fs::Representation_p rep(new fs::Circle(x, y, r));
	new_obj->set_rep(start_fidx, rep);

	new_obj->draw(seq[start_fidx], start_fidx, cv::Scalar(255,255,255), 1, true);
	cv::imshow("Objects", seq[start_fidx]);
	cv::waitKey();

	// Find nearest object

	double min_distance(DBL_MAX);
	fs::Object_p closest_obj;

	BOOST_FOREACH(fs::Object_p obj, oc.objects)
	{
		double distance = obj->get_distance(new_obj, start_fidx);
		if (distance < min_distance)
		{
			min_distance = distance;
			closest_obj = obj;
		}
	}

	closest_obj->draw(seq[start_fidx], start_fidx, cv::Scalar(255,255,255), 1, true);
	cv::line(seq[start_fidx], new_obj->get_pos(start_fidx), closest_obj->get_pos(start_fidx), cv::Scalar(255,255,255));
	cv::imshow("Objects", seq[start_fidx]);
	cv::waitKey();

	return 0;
}