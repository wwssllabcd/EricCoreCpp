//==================== template 的LNK2019 錯誤 ============================================
//lnk 2019的問題
//模板類在編譯以前需要做一次類型參數與原有代碼的整合過程，在上例中要把int型參數和模板類的定義生成一份基於int的類定義，然後編譯。
//在鏈接的時候鏈接器在.h文件中找不到TPL函數關於int類型的定義，所以報錯LNK2019。
//
//兩種解決方法：
//1) 在要被調用的函數定義名前加關鍵字export。比如：
//export typ GetSecondMember() const;
//
//可惜該關鍵字不被大多數CPP編譯器支持（包括VS.net7.1）
//
//forget it!
//
//2）把typ.cpp里的代碼放在typ.h中，刪除typ.cpp文件。然後編譯即可。這樣子，編譯器可以找到基於int類型類定義，通過！:-)
//
// reference： http://blog.donews.com/forient/category/5824.aspx


//==================== valarray 錯誤 C2334 錯誤 ============================================
//只要在#include WinDef.h之前，#define NOMINMAX或是在專案設定的Preprocessor裡輸入NOMINMAX就可以關閉max, min macro了
//關鍵字:boost/serialization/vector.hpp c2334 NOMINMAX

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
