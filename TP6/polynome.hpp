#pragma once

#include <algorithm>
#include <complex>
#include <numeric>
#include <iterator>
#include <ostream>
#include <type_traits>
#include <vector>


template <typename T>
bool is_zero(T x)
{
    return x == 0;
}

template <typename T>
bool is_one(T x)
{
    return x == 1;
}

template <>
bool is_zero<double>(double x)
{
    return std::abs(x) <= std::numeric_limits<double>::epsilon();
}

template <>
bool is_one<double>(double x)
{
    return std::abs(x - 1.0) <= std::numeric_limits<double>::epsilon();
}

template <typename T>
bool is_zero(std::complex<T> x)
{
    return is_zero(x.real()) && is_zero(x.imag());
}

template <typename T>
bool is_one(std::complex<T> x)
{
    return is_one(x.real()) && is_zero(x.imag());
}

template <typename T>
class polynome;

template <typename T>
std::ostream& operator<<(std::ostream& out, const polynome<T>& p);

template<typename T>
struct is_complex : public std::false_type {};

template<typename T>
struct is_complex<std::complex<T>> : public std::true_type {};

template <typename T>
polynome<T> operator+(const polynome<T>& p1, const polynome<T>& p2);

template <typename T>
polynome<T> operator-(const polynome<T>& p1, const polynome<T>& p2);

template <typename T>
class polynome
{
public:
    polynome(std::size_t deg = 0)
         : m_deg(deg), m_coeffs(deg + 1, 0)
    {}

    polynome(const T& a, std::size_t n)
         : m_deg(n), m_coeffs(n + 1, 0)
    {
        m_coeffs[n] = a;
    }

    polynome(const std::vector<T>& coeffs)
         : m_deg(coeffs.size() - 1), m_coeffs(coeffs)
    {}
public:
    std::size_t degree() const { return m_deg; }
    const T& operator[](std::size_t i) const
    {
        if (i <= m_deg) {
            return m_coeffs[i];
        }
        return 0;
    }

    friend std::ostream& operator<< <>(std::ostream& out, const polynome& p);
    friend polynome operator+ <>(const polynome& p1, const polynome& p2);
    friend polynome operator- <>(const polynome& p1, const polynome& p2);
private:
    void print_monome(std::ostream& out, const T& a, std::size_t n) const;
    polynome extend(std::size_t m) const;
    void adjust();
private:
    std::size_t m_deg;
    std::vector<T> m_coeffs;
};


template <typename T>
std::ostream& operator<<(std::ostream& out, const polynome<T>& p)
{
    for (std::size_t i = 0; i <= p.m_deg; ++i) {
        if (i != 0 && !is_zero(p.m_coeffs[i])) {
            out << " + ";
        }
        p.print_monome(out, p.m_coeffs[i], i);
    }
    return out;
}

template <typename T>
void polynome<T>::print_monome(std::ostream& out, const T& a, std::size_t n) const
{
    char x = is_complex<T>::value ? 'z' : 'x';
    if (is_zero(a)) {
        return;
    }
    if (n == 0) {
        out << a;
    } else if (n == 1) {
        out << a << '*' << x;
    } else {
        out << a << '*' << x << '^' << n;
    }
}

template <typename T>
polynome<T> polynome<T>::extend(std::size_t m) const
{
    if (m_deg < m) {
        return *this;
    }
    polynome<T> r(m_coeffs);
    r.m_coeffs.resize(m + 1);
    return r;
}

template <typename T>
void polynome<T>::adjust()
{
    auto it = std::find_if(m_coeffs.rbegin(), m_coeffs.rend(), [](const T& a) {
        return !is_zero(a);
    });
    auto d = std::distance(it, m_coeffs.rend());
    m_coeffs.resize(d);
    m_deg = d - 1;
}

template <typename T>
polynome<T> operator+(const polynome<T>& p1, const polynome<T>& p2)
{
    const polynome<T>* min = &p1;
    const polynome<T>* max = &p2;
    if (p1.m_deg > p2.m_deg) {
        min = &p2;
        max = &p1;
    }
    auto p3 = min->extend(max->m_deg);
    min = &p3;

    polynome<T> r(p1.m_coeffs);
    std::transform(r.m_coeffs.begin(), r.m_coeffs.end(), p2.m_coeffs.begin(),
                   r.m_coeffs.begin(), [](const T& a, const T& b) {
        return a + b;
    });
    r.adjust();
    return r;
}

template <typename T>
polynome<T> operator-(const polynome<T>& p1, const polynome<T>& p2)
{
    const polynome<T>* min = &p1;
    const polynome<T>* max = &p2;
    if (p1.m_deg > p2.m_deg) {
        min = &p2;
        max = &p1;
    }
    auto p3 = min->extend(max->m_deg);
    min = &p3;

    polynome<T> r(p1.m_coeffs);
    std::transform(r.m_coeffs.begin(), r.m_coeffs.end(), p2.m_coeffs.begin(),
                   r.m_coeffs.begin(), [](const T& a, const T& b) {
        return a - b;
    });
    r.adjust();
    return r;
}