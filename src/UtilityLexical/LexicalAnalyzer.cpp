#include "stdafx.h"

#include "LexicalAnalyzer.h"

bool is_digit(echar c) {
	if ('0' <= c && c <= '9') {
		return true;
	}
	return false;
}

bool get_num(echar c) {
	if ('0' <= c && c <= '9') {
		return true;
	}
	return false;
}

void get_token(estring cmd) {

	if (is_digit(cmd[0])) {

	}
}
