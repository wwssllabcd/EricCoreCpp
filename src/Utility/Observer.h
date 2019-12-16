#pragma once

#include "DefineFiles/DefineFile.h"
#include <vector>

using namespace std;


#define SEND_MSG(fmt, ...)                 Observer::sendMsg(0, true, false, fmt, __VA_ARGS__)
#define SEND_MSG_NOCRLF(fmt, ...)          Observer::sendMsg(0, false, false, fmt, __VA_ARGS__)
#define SEND_MSG_STR(estr)                 Observer::sendMsg(0, estr, true, false)
#define SEND_MSG_CLEAR()                   Observer::sendMsg(0, false, true, _ET(""))

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
        ERIC_STATIC void sendMsg(int id, estring& msg, bool isCrLf, bool isClean);
        ERIC_STATIC void sendMsg(int id, bool isCrLf, bool isClean, ELPCTSTR fmt, ...);
        ERIC_STATIC ObserObjColls m_obsColls;
    };
}