#ifndef SRC_COMMON_VARIANT_H_
#define SRC_COMMON_VARIANT_H_

#include <iostream>
#include <type_traits>
#include <utility>
#include "except/exceptions.h"

template <typename... Ts>
class Variant
{
private:
    std::aligned_union_t<0, Ts...> data;
    std::size_t id;

    template <typename K, typename H, typename... Us>
    struct Type
    {
        static constexpr std::size_t id = Type<K, Us...>::id;
    };

    template <typename K, typename... Us>
    struct Type<K, K, Us...>
    {
        static constexpr std::size_t id = sizeof...(Us) + 1;
    };

    template <typename... Us>
    struct Helper
    {
        static void destroy(std::size_t, void*) {}
        static void copy(std::size_t, void*, void*) {}
    };

    template <typename H, typename... Us>
    struct Helper<H, Us...>
    {
        static void destroy(std::size_t id, void* data)
        {
            if (id == Type<H, H, Us...>::id)
                reinterpret_cast<H*>(data)->~H();
            else
                Helper<Us...>::destroy(id, data);
        }

        static void copy(std::size_t id, const void* data, void* newdata)
        {
            if (id == Type<H, H, Us...>::id)
                new (newdata) H(reinterpret_cast<H*>(data));
            else
                Helper<Us...>::copy(id, data, newdata);
        }

        static void move(std::size_t id, const void* data, void* newdata)
        {
            if (id == Type<H, H, Us...>::id)
                new (newdata) H(std::move(reinterpret_cast<H*>(data)));
            else
                Helper<Us...>::move(id, data, newdata);
        }
    };
    
public:
    Variant():
        id(-1)
    {}

    Variant(const Variant& other):
        id(other.id), data(other.data)
    {}

    Variant(const Variant&& other):
      id(std::move(other.id))
    {
        Helper<Ts...>::move(this->id, &other.data, &data);
    }

    ~Variant()
    {
        this->clear();
    }

    bool hasValue()
    {
        return this->id != -1;
    }

    void clear()
    {
        Helper<Ts...>::destroy(this->id, &this->data);
        this->id = -1;
    }

    template <typename T>
    bool is()
    {
        return Type<T, Ts...>::id == this->id;
    }

    template <typename T>
    T& get()
    {
        if (!this->is<T>())
            throw VariantException();

        return *reinterpret_cast<T*>(&this->data);
    }

    template <typename T, typename... Args>
    void set(Args&&... args)
    {
        this->clear();
        new (&this->data) T(std::forward<Args...>(args...));
        this->id = Type<T, Ts...>::id;
    }

    Variant& operator=(const Variant& other)
    {
        this->clear();
        Helper<Ts...>::copy(other.id, &other.data, &this->data);
        return *this;
    }

    Variant& operator=(const Variant&& other)
    {
        this->clear();
        this->id = std::move(other.id);
        Helper<Ts...>::move(other.id, &other.data, &data);
        return *this;
    }
};

#endif
