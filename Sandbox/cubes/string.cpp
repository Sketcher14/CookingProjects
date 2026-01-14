#include "cubes/string.h"

#include <cstring>
#include <print>
#include <utility>

namespace sb
{
string::string(const char* data)
{
    if (data == nullptr)
        return;

    const size_t data_size = strlen(data);
    _data = new char[data_size + 1];

    strcpy(_data, data);
    _size = data_size;
}

string::string(const string& other)
{
    if (other._data == nullptr)
        return;

    _data = new char[other._size + 1];
    strcpy(_data, other._data);
    _size = other._size;
}

string& string::operator=(const string& other)
{
    if (this != &other)
    {
        string temp(other);
        swap(*this, temp);
    }

    return *this;
}

string::string(string&& other) noexcept
{
    swap(*this, other);
}

string& string::operator=(string&& other) noexcept
{
    if (this != &other)
    {
        string temp(std::move(other));
        swap(*this, temp);
    }

    return *this;
}

string::~string()
{
    delete[] _data;
}

void swap(string& left, string& right) noexcept
{
    std::swap(left._size, right._size);
    std::swap(left._data, right._data);
}

char& string::operator[](const size_t index)
{
    return _data[index];
}

const char& string::operator[](const size_t index) const
{
    return _data[index];
}

std::strong_ordering string::operator<=>(const string& other) const noexcept
{
    if (_data == nullptr && other._data == nullptr)
        return std::strong_ordering::equal;
    if (_data == nullptr)
        return std::strong_ordering::less;
    if (other._data == nullptr)
        return std::strong_ordering::greater;

    const int result = strcmp(_data, other._data);
    if (result < 0)
        return std::strong_ordering::less;
    if (result > 0)
        return std::strong_ordering::greater;

    return std::strong_ordering::equal;
}

bool string::operator==(const string& other) const noexcept
{
    if (_data == nullptr && other._data == nullptr)
        return true;
    if (_data == nullptr || other._data == nullptr)
        return false;

    if (_size != other._size)
        return false;

    return strcmp(_data, other._data) == 0;
}

size_t string::size() const noexcept
{
    return _size;
}

bool string::empty() const noexcept
{
    return _size == 0;
}

const char* string::data() const noexcept
{
    return _data == nullptr ? "" : _data;
}

void string::print() const noexcept
{
    if (_data == nullptr)
        return;

    for (size_t i = 0; i < _size; ++i)
        std::print("{}", _data[i]);

    std::println();
}
} // namespace sb
