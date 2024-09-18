#pragma once

#include <cstddef>
#include <iostream>
#include <cstring>

// q9
namespace pr
{

    int compare(const char *a, const char *b)
    {
        while (*a && (*a == *b))
        {
            ++a;
            ++b;
        }
        return static_cast<int>(static_cast<unsigned char>(*a)) - static_cast<int>(static_cast<unsigned char>(*b));
    }

    class String
    {
    private:
        char *data;

    public:
        // q12
        String(const char *str)
        {
            size_t len = 0;
            while (str[len])
                ++len;

            data = new char[len + 1];
            for (size_t i = 0; i < len; ++i)
                data[i] = str[i];
        }
        // q12
        ~String()
        {
            delete[] data;
        }

        size_t length() const
        {
            size_t len = 0;
            while (data[len])
                ++len;

            return len;
        }

        String &operator=(const String &other)
        {
            if (this != &other)
            {
                delete[] data;

                size_t len = other.length();
                data = new char[len + 1];
                for (size_t i = 0; i <= len; ++i)
                    data[i] = other.data[i];
            }
            return *this;
        }

        bool operator<(const String &b) const
        {
            return pr::compare(this->data, b.data) < 0;
        }

        friend std::ostream &operator<<(std::ostream &os, const String &str);
        friend bool operator==(const String &a, const String &b);
    };
    // q10
    inline std::ostream &operator<<(std::ostream &os, const String &str)
    {
        os << str.data;
        return os;
    }

    inline bool operator==(const String &a, const String &b)
    {
        return compare(a.data, b.data) == 0;
    }
}
