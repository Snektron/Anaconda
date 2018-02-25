#ifndef SRC_COMMON_VARIANT_H_
#define SRC_COMMON_VARIANT_H_

#include <iostream>
#include <type_traits>
#include <utility>
#include <ostream>
#include "common/format.h"
#include "except/exceptions.h"

template <typename... Ts>
class Variant
{
private:
    std::size_t id;
    std::aligned_union_t<0, Ts...> data;

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

        static void copy(std::size_t, const void*, void*) {}

        static void move(std::size_t, const void*, void*) {}

        static void print(std::size_t, const void*, std::ostream& os) {
            os << "None";
        }
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
                new (newdata) H(*reinterpret_cast<const H*>(data));
            else
                Helper<Us...>::copy(id, data, newdata);
        }

        static void move(std::size_t id, const void* data, void* newdata)
        {
            if (id == Type<H, H, Us...>::id)
                new (newdata) H(std::move(*reinterpret_cast<const H*>(data)));
            else
                Helper<Us...>::move(id, data, newdata);
        }

        static void print(std::size_t id, const void* data, std::ostream& os)
        {
            if (id == Type<H, H, Us...>::id)
                os << *reinterpret_cast<const H*>(data);
            else
                Helper<Us...>::print(id, data, os);
        }
    };
    
public:
    template <typename T, typename... Args>
    static Variant<Ts...> make_mv(Args&&... args)
    {
        Variant<Ts...> var;
        var.set<T>(std::forward<Args...>(args...));
        return var;
    }

    template <typename T, typename... Args>
    static Variant<Ts...> make(Args&... args)
    {
        Variant<Ts...> var;
        var.set<T>(args...);
        return var;
    }

    Variant():
        id(0)
    {}

    Variant(const Variant& other):
        id(other.id)
    {
        Helper<Ts...>::copy(this->id, &other.data, &data);
    }

    Variant(const Variant&& other):
      id(std::move(other.id))
    {
        Helper<Ts...>::move(this->id, &other.data, &data);
    }

    ~Variant()
    {
        this->clear();
    }

    bool hasValue() const
    {
        return this->id != 0;
    }

    void clear()
    {
        Helper<Ts...>::destroy(this->id, &this->data);
        this->id = 0;
    }

    template <typename T>
    bool is() const
    {
        return Type<T, Ts...>::id == this->id;
    }

    template <typename T>
    T& get()
    {
        if (!this->is<T>())
            throw VariantException(fmt::sprintf("Error Variant::get(): id = ", this->id, " get type = ", Type<T, Ts...>::id));

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
        this->id = other.id;
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

    void print(std::ostream& os) const {
        Helper<Ts...>::print(this->id, &this->data, os);
    }

    template <typename... Us>
    friend std::ostream& operator<<(std::ostream&, const Variant<Us...>&);
};

template <typename H, typename... Ts>
std::ostream& operator<<(std::ostream& os, const Variant<H, Ts...>& variant)
{
    variant.print(os);
    return os;
}

#endif