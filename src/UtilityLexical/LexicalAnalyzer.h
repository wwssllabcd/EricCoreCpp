#pragma once
#include "DefineFiles/Definefile.h"
#include <vector>

using namespace std;

typedef struct {
	estring funName;
	vector<estring> params;
}Function;

typedef struct {
	eu8 type;
	eu32 num;
	estring str;
}Identify;
