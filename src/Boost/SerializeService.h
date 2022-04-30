//==================== template ��LNK2019 ���~ ============================================
//lnk 2019�����D
//�ҪO���b�sĶ�H�e�ݭn���@�������ѼƻP�즳�N�X����X�L�{�A�b�W�Ҥ��n��int���ѼƩM�ҪO�����w�q�ͦ��@�����int�����w�q�A�M��sĶ�C
//�b�챵���ɭ��챵���b.h��󤤧䤣��TPL�������int�������w�q�A�ҥH����LNK2019�C
//
//��ظѨM��k�G
//1) �b�n�Q�եΪ���Ʃw�q�W�e�[����rexport�C��p�G
//export typ GetSecondMember() const;
//
//�i��������r���Q�j�h��CPP�sĶ������]�]�AVS.net7.1�^
//
//forget it!
//
//2�^��typ.cpp�����N�X��btyp.h���A�R��typ.cpp���C�M��sĶ�Y�i�C�o�ˤl�A�sĶ���i�H�����int�������w�q�A�q�L�I:-)
//
// reference�G http://blog.donews.com/forient/category/5824.aspx


//==================== valarray ���~ C2334 ���~ ============================================
//�u�n�b#include WinDef.h���e�A#define NOMINMAX�άO�b�M�׳]�w��Preprocessor�̿�JNOMINMAX�N�i�H����max, min macro�F
//����r:boost/serialization/vector.hpp c2334 NOMINMAX

#pragma once
#include <boost/serialization/vector.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <fstream>
#include <iostream>
#pragma comment(lib, "libboost_serialization-vc90-mt-1_58.lib")

#include "../Utility/EricException/MyException.h"
#include "../Utility/DefineFiles/DefineFile.h"



using namespace std;

class SerializeService
{
public:
	SerializeService(void){};
	~SerializeService(void){};

	template<class T>
	void save(tstring filePath,const vector<T>& s)
	{
		std::ofstream ofs(filePath.c_str());
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(s);
		ofs.close();	
	}

	template<class T>
	void load(tstring filePath, vector<T>& collnew)
	{
		ifstream ifs(filePath.c_str());
		if(!ifs){
			throw MyException(0, "can`t find the file By Path in loadBeans function");
		}

		try{
			boost::archive::xml_iarchive ia(ifs);
			ia >> BOOST_SERIALIZATION_NVP(collnew); 
		}catch(exception e){
			tstring msg,err(e.what());
			msg += "Parse XML file of Serial Number error";
			msg += "\n\rPlease check the format of XML file or rebuild another one";
			msg += "\n\rOrigion error is :";
			msg += err;
			throw MyException(0, msg.c_str());
		}

	}

};
