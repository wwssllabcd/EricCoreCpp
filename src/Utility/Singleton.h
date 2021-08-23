#pragma once
#include <vector>

#include "DefineFiles\DefineFile.h"
#include "Utility\EricException.h"
#include <mutex>

using namespace std;

mutex g_mutex;

template <typename T>
class Singleton
{
public:
    ~Singleton() {};

    static Singleton<T>* get_instance() {
        lock_guard<mutex> lock(g_mutex);
        if (m_instance<T> == NULL) {
            m_instance<T> = new Singleton<T>();
        }
        return m_instance<T>;
    }

    void release() {
        return m_instance<T> = NULL;
    }

    void push_back(const T& item) {
        m_itemColls.push_back(item);
    }

    T get_item(eu32 idx) {
        if (idx >= m_itemColls.size() ) {
            THROW_MYEXCEPTION(0, _ET("Singleton::get_item OFB, idx=%X >= size=%X"), idx, m_itemColls.size());
        }
        return m_itemColls[idx];
    }

    eu32 size() {
        return m_itemColls.size();
    }
    
private:
    Singleton() {};
    vector<T> m_itemColls;
};

template <typename T>
static Singleton<T>* m_instance = NULL; // initialize pointer




