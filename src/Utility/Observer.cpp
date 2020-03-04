//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"
#include "Observer.h"
#include "Utility.h"
#include "EricException.h"
#include <stdarg.h> // for va_start, va_end

using namespace EricCore;
using namespace std;

Observer::Observer(void) {
}

Observer::~Observer(void) {
}

Observer::ObserObjColls Observer::m_obsColls;
void Observer::observerRegister(int id, MsgFunPtr pF) {
    if (pF == 0) {
        THROW_MYEXCEPTION(UTI_OBS_REG_FAIL_NO, _ET("observerRegister error: function ptr is empty"));
    }
    for (ObserObjColls::size_type i = 0; i < Observer::m_obsColls.size(); i++) {
        MsgFunPtr testFP = Observer::m_obsColls[i].second;
        if (testFP == pF) {
            return;
        }
    }
    ObserObj o;
    o.first = id;
    o.second = pF;
    m_obsColls.push_back(o);
}

void Observer::sendMsg(int id, estring& msg, bool isCrLf, bool isClean) {
    ObserObjColls::size_type size = m_obsColls.size();
    if (size == 0) {
        THROW_MYEXCEPTION(UTI_SEND_MSG_FAIL, _ET("sendMsg error: no any send msg function"));
    }

    for (ObserObjColls::size_type i = 0; i < size; i++) {
        ObserObj o = m_obsColls[i];
        MsgFunPtr pF = m_obsColls[i].second;
        if (pF == 0) {
            THROW_MYEXCEPTION(UTI_SEND_MSG_FAIL, _ET("sendMsg error: no any send msg function"));
        }
        if (o.first == id) {
            if (isCrLf) {
                msg = Utility::crLf() + msg;
            }
            pF(msg, isClean);
        }
    }
}

void Observer::sendMsg(int id, bool isCrLf, bool isClean, ELPCTSTR fmt, ...) {
    estring	str;
    va_list	arg;
    va_start(arg, fmt);
    Utility su;
    str = su.strFormatValist(0, fmt, arg);
    va_end(arg);
    sendMsg(id, str, isCrLf, isClean);
}

