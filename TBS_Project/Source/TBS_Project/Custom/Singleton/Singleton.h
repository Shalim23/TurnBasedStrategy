#pragma once

template<typename ClassName>
class Singleton
{
public:
    static ClassName& GetInstance(){ static ClassName instance; return instance; }
    virtual void Init() = 0;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    Singleton(const Singleton& other) = delete;
    Singleton(const Singleton&& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
    Singleton& operator=(const Singleton&& other) = delete;
};
