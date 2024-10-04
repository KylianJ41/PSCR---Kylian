#pragma once

#include <cstddef>
#include <stdexcept>

namespace pr
{

    template <typename T>
    class Vector
    {
    private:
        T *data;
        size_t cur_size;
        size_t capacity;

        void resize(size_t new_capacity)
        {
            T *new_data = new T[new_capacity];

            for (size_t i = 0; i < cur_size; ++i)
            {
                new_data[i] = data[i];
            }

            delete[] data;

            data = new_data;
            capacity = new_capacity;
        }

    public:
        // type alias
        using iterator = T *;
        using const_iterator = const T *;

        Vector(size_t initial_capacity = 10)
            : data(new T[initial_capacity]), cur_size(0), capacity(initial_capacity) {}

        ~Vector()
        {
            delete[] data;
        }

        // Ce constructeur s'appelle de cette facon : Vector<int> v2 = v1;
        Vector(const Vector &other)
            : data(new T[other.capacity]), cur_size(other.cur_size), capacity(other.capacity)
        {
            for (size_t i = 0; i < cur_size; ++i)
            {
                data[i] = other.data[i];
            }
        }

        Vector &operator=(const Vector &other)
        {
            if (this != &other)
            {
                delete[] data;

                data = new T[other.capacity];
                cur_size = other.cur_size;
                capacity = other.capacity;
                for (size_t i = 0; i < cur_size; ++i)
                {
                    data[i] = other.data[i];
                }
            }
            // retourne l'objet lui-même plutot que la reference de l'objet
            return *this;
        }

        // le premier const signifie que la méthode ne modifie pas l'objet
        const T &operator[](size_t index) const
        {
            if (index >= cur_size)
            {
                throw std::out_of_range("Index hors limites");
            }
            return data[index];
        }

        T &operator[](size_t index)
        {
            if (index >= cur_size)
            {
                throw std::out_of_range("Index hors limites");
            }
            return data[index];
        }

        void push_back(const T &val)
        {
            if (cur_size >= capacity)
                resize(capacity * 2);

            data[cur_size++] = val;
        }

        size_t size() const
        {
            return cur_size;
        }

        bool empty() const
        {
            return cur_size == 0;
        }

        iterator begin() { return data; }
        iterator end() { return data + cur_size; }

        const_iterator begin() const { return data; }
        const_iterator end() const { return data + cur_size; }
    };

    template <typename Iterator, typename T>
    Iterator find(Iterator begin, Iterator end, const T &target)
    {
        for (Iterator it = begin; it != end; ++it)
        {
            if (*it == target)
                return it;
        }
        return end;
    }

}