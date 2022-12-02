#pragma once

#include "DefineFiles/DefineFile.h"
#include <vector>

using namespace std;

//Note: you can used msg.c_str for estring mode
#define SEND_MSG(fmt, ...)                               EricCore::Observer::sendMsg(0, true, false, fmt, __VA_ARGS__)

//to avoid "incorrent format specifier, 0" issue
#define SEND_MSG_ESTR(estr)                              EricCore::Observer::sendMsg(0, estr, false, true)
#define SEND_MSG_ESTR_CTRL(estr, isCrlf, isClear)        EricCore::Observer::sendMsg(0, estr, isCrlf, isClear)

#define SEND_MSG_CTRL(isCrlf, isClear, fmt, ...)         EricCore::Observer::sendMsg(0, isCrlf, isClear, fmt, __VA_ARGS__)
#define SEND_MSG_CLEAR()                                 EricCore::Observer::sendMsg(0, false, true, _ET(""))

#define SEND_MSG_AREA_1(isCrlf, isClear, fmt, ...)       EricCore::Observer::sendMsg(1, isCrlf, isClear, fmt, __VA_ARGS__)

//to avoid "incorrent format specifier, 0" issue
#define SEND_MSG_ESTR_AREA_1(estr)                       EricCore::Observer::sendMsg(1, estr, false, true)
#define SEND_MSG_ESTR_CTRL_AREA_1(estr, isCrlf, isClear) EricCore::Observer::sendMsg(1, estr, isCrlf, isClear)
#define SEND_CLEAR_AREA_1()                              EricCore::Observer::sendMsg(1, false, true, _ET(""))

namespace EricCore {
    class Observer
    {
        typedef void(*MsgFunPtr)(estring&, bool);
        typedef pair<int, MsgFunPtr> ObserObj;
        typedef vector<ObserObj> ObserObjColls;

    public:
        Observer(void);
        ~Observer(void);
        
        ERIC_STATIC void observerRegister(int id, MsgFunPtr pF);
        ERIC_STATIC void sendMsg(int id, estring& msg, bool isCrLf=true, bool isClean=false);
        ERIC_STATIC void sendMsg(int id, bool isCrLf, bool isClean, ELPCTSTR fmt, ...);
        ERIC_STATIC ObserObjColls m_obsColls;
    };
}