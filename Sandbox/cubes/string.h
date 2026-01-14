#pragma once

#include <compare>
#include <cstddef>

namespace sb
{
class string final
{
public:
    string() = default;
    explicit string(const char* data);

    string(const string& other);
    string& operator=(const string& other);

    string(string&& other) noexcept;
    string& operator=(string&& other) noexcept;

    ~string();

    friend void swap(string& left, string& right) noexcept;

    [[nodiscard]] char& operator[](const size_t index);
    [[nodiscard]] const char& operator[](const size_t index) const;
    [[nodiscard]] std::strong_ordering operator<=>(const string& other) const noexcept;
    [[nodiscard]] bool operator==(const string& other) const noexcept;

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] const char* data() const noexcept;

    void print() const noexcept;

private:
    char* _data = nullptr;
    size_t _size = 0;
};
} // namespace sb
