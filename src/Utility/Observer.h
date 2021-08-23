#pragma once

#include "DefineFiles/DefineFile.h"
#include <vector>

using namespace std;

//Note: you can used msg.c_str for estring mode
#define SEND_MSG(fmt, ...)                               EricCore::Observer::sendMsg(0, true, false, fmt, __VA_ARGS__)
#define SEND_MSG_STR(estr)                               SEND_MSG(estr.c_str())

#define SEND_MSG_CTRL(isCrlf, isClear, fmt, ...)         EricCore::Observer::sendMsg(0, isCrlf, isClear, fmt, __VA_ARGS__)
#define SEND_MSG_CLEAR()                                 EricCore::Observer::sendMsg(0, false, true, _ET(""))


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