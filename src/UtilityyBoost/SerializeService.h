#pragma once
#include <boost/serialization/vector.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

//#include <boost/archive/xml_wiarchive.hpp>
//#include <boost/archive/xml_woarchive.hpp>

//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>

#include <iostream>
#pragma comment(lib, "libboost_serialization-vc143-mt-sgd-x32-1_79.lib")

#include "DefineFiles/DefineFile.h"

using namespace std;

class SerializeService
{
public:
	SerializeService(void){};
	~SerializeService(void){};

	template<class T>
	void save(estring filename, const vector<T>& colls)
	{
		// make an archive
		ofstream ofs(filename.c_str());
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(colls);
	}

	template<class T>
	void load(estring filename, vector<T>& colls)
	{
		// open the archive
		ifstream ifs(filename.c_str());
		assert(ifs.good());
		boost::archive::xml_iarchive ia(ifs);

		// restore the schedule from the archive
		ia >> BOOST_SERIALIZATION_NVP(colls);
	}

};
