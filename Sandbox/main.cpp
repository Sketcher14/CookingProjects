#include <fstream>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <print>
#include <random>
#include <thread>
#include <unordered_set>

// example when shared_ptrs leads to a memory leak
struct Y;

struct XX
{
    std::shared_ptr<Y> p; // should be weak_ptr to avoid the memory leak
    ~XX()
    {
        std::cout << "~X()\n";
    }
};

struct Y
{
    std::shared_ptr<XX> p;

    ~Y()
    {
        std::cout << "~Y()\n";
    }
};

void oops()
{
    auto x = std::make_shared<XX>();
    auto y = std::make_shared<Y>();
    std::cout << x.use_count() << ' ' << y.use_count() << std::endl;
    x->p = y;
    y->p = x;
    std::cout << x.use_count() << ' ' << y.use_count() << std::endl;
}

std::mutex mtx1;
std::mutex mtx2;

// deadlock example
void foo1()
{
    std::cout << "foo1 Trying to lock mutex1" << std::endl;
    std::lock_guard lock1(mtx1);
    std::cout << "foo1 Locked mutex1" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "foo1 Trying to lock mutex2" << std::endl;
    std::lock_guard lock2(mtx2);
    std::cout << "foo1 Locked mutex2" << std::endl;
}

void foo2()
{
    std::cout << "foo2 Trying to lock mutex2" << std::endl;
    std::lock_guard lock2(mtx2);
    std::cout << "foo2 Locked mutex2" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "foo2 Trying to lock mutex1" << std::endl;
    std::lock_guard lock1(mtx1);
    std::cout << "foo2 Locked mutex1" << std::endl;
}

/*
 * testing passing a custom object to functions
 */

// struct giga_struct
// {
//     test_object to_{};
//
//     giga_struct() = default;
//
//     explicit giga_struct(test_object&& to) :
//         to_(std::move(to))
//     {};
// };

// options:
// 1. if I need to move object to the class or struct (it will no longer be available
// at the call location), I will declare the r-value constructor
// because it does only one move constructor + blocks passing the object by value
// 2. if I need to copy object to the class or struct, I will declare the const l-value reference
// which does only one copy of the object

template<>
struct std::hash<std::pair<int, int>>
{
    size_t operator()(const std::pair<int, int>& pair) const noexcept
    {
        const std::hash<int> hasher;
        const size_t first = hasher(pair.first);
        const size_t second = hasher(pair.second);
        return first ^ (second << 1);
    }
};


/* include directives */
template<class U>
std::enable_if<(sizeof(U) > 4)>::type custom_enable()
{
    std::cout << "enabled" << std::endl;
}

template<class T>
concept HasFoo = requires(const T& cl) { cl.foo(); };


template<class T>
class Singleton
{
public:
    static T* get_instance()
    {
        if (_instance == nullptr)
            _instance = new T();

        return _instance;
    }

protected:
    Singleton() = default;

    Singleton(const T& other) = delete;

    Singleton& operator=(const T& other) = delete;

private:
    static T* _instance;
};

template<class T>
T* Singleton<T>::_instance = nullptr;

struct AAA : public Singleton<AAA>
{
    friend Singleton;

public:
    void print()
    {
        std::cout << "hello from AAA" << std::endl;
    }

private:
    AAA() = default;
};

struct BBB
{
    // uint64_t Value1 = 10;
    uint32_t Value2 = 20;
    // uint16_t Value3 = 30;
};

void blocking_destructor_example()
{
    std::cout << "Starting...\n";

    {
        auto fut = std::async(std::launch::async,
                              []
                              {
                                  std::this_thread::sleep_for(std::chrono::seconds(5));
                                  std::cout << "Task completed\n";
                                  return 42;
                              });

        std::cout << "About to destroy future...\n";
    } // ← БЛОКИРУЕТСЯ ЗДЕСЬ на 5 секунд!

    std::cout << "Future destroyed\n";
}

#include <cstdint>

class A
{
public:
    A()
    {
        std::cout << "A()" << std::endl;
    }

    A([[maybe_unused]] const A& other)
    {
        std::cout << "A copy constructor" << std::endl;
    }

    virtual ~A()
    {
        std::cout << "~A()" << std::endl;
    }
};

class B
{
public:
    B()
    {
        member = new A;
    }

    B(const B& other)
    {
        member = other.member == nullptr ? nullptr : new A(*other.member);
    }

    B& operator=(const B& other)
    {
        if (this != &other)
        {
            //A* new_member = other.member == nullptr ? nullptr : new A(*other.member);
            B temp(other);
            swap(*this, temp);
        }

        return *this;
    }

    B(B&& other) noexcept
    {
        member = other.member;
        other.member = nullptr;
    }

    B& operator=(B&& other) noexcept
    {
        if (this != &other)
        {
            B temp(std::move(other));
            swap(*this, temp);
            // delete member;
            // member = other.member;
            // other.member = nullptr;
        }

        return *this;
    }

    ~B()
    {
        std::cout << "delete " << member << std::endl;
        delete member;
    }

    friend void swap(B& lhs, B& rhs) noexcept
    {
        std::swap(lhs.member, rhs.member);
    }

    [[nodiscard]] A* get_member() const
    {
        return member;
    }

    void set_member(A* value)
    {
        delete member;
        member = value;
    }

private:
    A* member;
};

int main()
{
    int k = std::numeric_limits<int>::max();
    k += 10;
    std::cout << k << std::endl;
    return 0;
}

template<size_t size>
class Tribonacci
{
public:
    static constexpr long long value =
            Tribonacci<size - 3>::value + Tribonacci<size - 2>::value + Tribonacci<size - 1>::value;
};

template<>
class Tribonacci<0>
{
public:
    static constexpr long long value = 0;
};

template<>
class Tribonacci<1>
{
public:
    static constexpr long long value = 1;
};

template<>
class Tribonacci<2>
{
public:
    static constexpr long long value = 1;
};


template<size_t... Is>
constexpr auto calculate_tribonaccis(std::index_sequence<Is...>)
{
    return std::array<long long, sizeof...(Is)>{ Tribonacci<Is>::value... };
}

// constexpr std::array<long long, 38> tribonaccis = calculate_tribonaccis(std::make_index_sequence<38>{});


// int main()
// {
//     int n;
//     std::cin >> n;
//
//     const long long tribonacci = tribonaccis[static_cast<size_t>(n)];
//     std::cout << tribonacci;
// }

// int main()
// {
//     std::cout << boolalpha;
//     std::cout << is_void<void>::value << std::endl;
//     std::cout << is_void<int>::value << std::endl;
//     std::cout << is_pointer<XX*>::value << std::endl;
//     std::cout << is_pointer<XX>::value << std::endl;
//     std::cout << is_pointer<XX&>::value << std::endl;
//     std::cout << is_lvalue_reference_v<XX&> << std::endl;
//     std::cout << is_lvalue_reference_v<const XX&> << std::endl;
//     std::cout << is_lvalue_reference_v<XX> << std::endl;
//     std::cout << is_pointer<int*>::value << std::endl;
//     std::cout << is_pointer<int**>::value << std::endl;
//     std::cout << is_pointer<int[10]>::value << std::endl;
//     std::cout << is_pointer<nullptr_t>::value << std::endl;
// }


// //base function: recursive end
// void printf_vt(const char *s)
// {
//     while (*s) {
//         if (*s == '%' && *(++s) != '%')
//             throw std::runtime_error("invalid format string: missing arguments");
//         std::std::cout << *s++;
//     }
// }
//
// //recursive call
// template<typename T, typename... Rest>
// void printf_vt(const char *s, T value, Rest... rest)
// {
//     while (*s) {
//         if (*s == '%' && *(++s) != '%') {
//             std::std::cout << value;
//             printf_vt(s, rest...); // called even when *s is 0 but does nothing in that case
//             return;
//         }
//         std::std::cout << *s++;
//     }
// }
//
// int main() {
//     int x = 10;
//     float y = 3.6;
//     std::string s = std::string("Variadic templates");
//     const char* msg = "%s can accept %i parameters (or %s), x=%d, y=%f\n";
//     std::printf(msg, s.c_str(), 100, "more", x, y);   //replace 's' by 's.c_str()' to prevent the output bug
//
//     const char* msg2 = "% can accept % parameters (or %); x=%, y=%\n";
//     printf_vt(msg2, s, 100, "more", x, y);
//     return 0;
// }

// double my_min(double n)
// {
//     return n;
// }
//
// template<typename... Args>
// double my_min(double n, Args... args)
// {
//     return std::fmin(n, my_min(args...));
// }
//
// int main() {
//
//     double x1 = my_min(2);
//     double x2 = my_min(2, 3);
//     double x3 = my_min(2, 3, 4, 5, 4.7,5.6, 9.9, 0.1);
//     std::std::cout << "x1=" << x1 <<  ", x2=" << x2 <<  ", x3=" << x3 << std::std::endl;
//     return 0;
// }

// #define __PRETTY_FUNCTION____FUNCSIG__
//
// using namespace std;
// template<class T, template<class, class...> class X, class... Args>
// std::ostream& operator <<(std::ostream& os, const X<T, Args...>& objs) {
//     os << __PRETTY_FUNCTION__ << ":" << std::endl;
//     for (auto const& obj : objs)
//         os << obj << ' ';
//     return os;
// }

// future, async and promise
// int main()
// {
//     //auto spPromise = std::make_shared<promise<size_t> >();
//     //std::future<size_t> future = spPromise->get_future();
//     auto call = [/*spPromise*/](size_t value)
//     {
//         size_t i = 0;
//         while (i < value)
//         {
//             std::cout << i << ' ';
//             i++;
//         }
//         //spPromise->set_value(i);
//         std::cout << '\n';
//     };
//
//     auto start_time = std::chrono::system_clock::now();
//
//     std::future<void> future = std::async(std::launch::deferred, call, 1000);
//     std::cout << "created future with deferred async" << std::endl;
//
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//
//     future.get();
//
//     auto finish_time = std::chrono::system_clock::now();
//     std::chrono::duration<double> diff{finish_time - start_time};
//
//     std::cout << "get value in " << diff.count() << std::endl;
//
//     return 0;
// }

// int n;
// cin >> n;
//
// for (int i = 0; i < n; ++i)
// {
//     string idName, timestampName, remaining_line;
//     int id, timestamp;
//     vector<string> words;
//     cin >> idName >> id >> timestampName >> timestamp;
//
//     getline(cin, remaining_line);
//
//     istringstream iss(remaining_line);
//     string str;
//     while (iss >> str)
//         words.push_back(str);
//
//     std::cout << id << ' ' << timestamp << ' ';
//     for (const string& word : words)
//     {
//         std::cout << word << ' ';
//     }
//     std::cout << std::endl;
// }
