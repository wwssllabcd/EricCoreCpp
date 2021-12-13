#pragma once
#include <vector>

#include "DefineFiles\DefineFile.h"
#include "Utility\EricException.h"
#include <mutex>

using namespace std;

static ULONG m_instance = NULL; // initialize pointer


template <typename T>
class Singleton
{
public:
    ~Singleton() {};

    static Singleton<T>* get_instance() {
        static mutex g_mutex;//maybe not thread safe

        lock_guard<mutex> lock(g_mutex);
        if (m_instance == NULL) {
            m_instance = (ULONG)(new Singleton<T>());
        }
        return (Singleton<T>*)m_instance;
    }

    void release() {
        clear();
        if (m_instance != NULL) {
            //it need to free memory
        }
        m_instance = NULL;
    }

    void push_back(const T& item) {
        m_itemColls.push_back(item);
    }

    void clear() {
        m_itemColls.clear();
    }

    T get_item(eu32 idx) {
        if (idx >= m_itemColls.size() ) {
            THROW_MYEXCEPTION(0, _ET("Singleton::get_item OFB, idx=%X >= size=%X"), idx, m_itemColls.size());
        }
        return m_itemColls[idx];
    }

    eu32 size() {
        return (eu32)m_itemColls.size();
    }
    
private:
    Singleton() {};
    vector<T> m_itemColls;
    
};







