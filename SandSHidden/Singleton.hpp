#pragma once

#include "pch.hpp"

template <class T>
class Singleton {
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    Singleton(const Singleton&) = default;
    Singleton& operator=(const Singleton&) = default;

public:
    static inline T* getIns() {
        static T inst;
        return &inst;
    }
};

