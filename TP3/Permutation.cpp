#include "Permutation.hpp"

#include <algorithm>
#include <set>

static std::int64_t Pcd(std::int64_t a, std::int64_t b);
static std::int64_t Lcm(std::int64_t a, std::int64_t b);

Permutation::Permutation(std::size_t size)
    : m_Size(size), m_Images(size)
{
    std::iota(m_Images.begin(), m_Images.end(), 0);
}

Permutation::Permutation(const std::vector<std::size_t>& images)
    : m_Size(images.size()), m_Images(images)
{
}

Permutation Permutation::Inverse() const
{
    std::vector<std::size_t> inverseImages(m_Size);
    for (std::size_t i = 0; i < m_Size; ++i)
    {
        inverseImages[m_Images[i]] = i;
    }
    return Permutation(inverseImages);
}

std::size_t Permutation::Size() const
{
    return m_Size;
}

bool Permutation::IsDerangement() const
{
    return FixedPoints().size() == 0;
}

std::size_t Permutation::Order() const
{
    std::list<Cycle> cycles = Cycles();
    return std::accumulate(cycles.begin(), cycles.end(), 1L, [](std::size_t p, const Cycle& c) { return Lcm(p, c.Order()); });
}

std::list<std::size_t> Permutation::FixedPoints() const
{
    std::list<std::size_t> fixedPoints;
    std::size_t i = 0;
    std::copy_if(m_Images.begin(), m_Images.end(), std::back_inserter(fixedPoints), [&i](std::size_t x) {
        return x == i++;
    });
    return fixedPoints;
}

std::list<Cycle> Permutation::Cycles() const
{
    std::list<Cycle> cycles;
    std::set<std::size_t> s;
    for (std::size_t i = 0; i < m_Size; ++i)
    {
        s.emplace_hint(s.end(), i);
    }

    while (!s.empty())
    {
        std::size_t min = *s.begin();
        if (min != m_Images[min])
        {
            Cycle cycle;
            cycle.AddLastPoint(min);

            std::size_t current = m_Images[min];
            while (current != min)
            {
                s.erase(current);
                cycle.AddLastPoint(current);
                current = m_Images[current];
            }
            cycles.push_back(cycle);
        }
        s.erase(s.begin());
    }

    return cycles;
}


Permutation operator*(const Permutation& a, const Permutation& b)
{
    auto minmax = std::minmax(a, b, [](const Permutation& a, const Permutation& b) {
        return a.Size() < b.Size();
    });
    std::vector<std::size_t> images(minmax.second.m_Size);

    for (std::size_t i = 0; i < minmax.first.m_Size; ++i)
    {
        images[i] = b[a[i]];
    }
    for (std::size_t i = minmax.first.m_Size; i < minmax.second.m_Size; ++i)
    {
        images[i] = i;
    }

    return Permutation(images);
}

std::size_t Permutation::operator[](std::size_t i) const
{
    return m_Images[i];
}

std::ostream& operator<<(std::ostream& out, const Permutation& p)
{
    out << p.m_Size << " : ";
    std::copy(p.m_Images.begin(), p.m_Images.end(), std::ostream_iterator<std::size_t>(out, " "));
    return out;
}

std::istream& operator>>(std::istream& in, Permutation& p)
{
    in >> p.m_Size;
    p.m_Images.resize(p.m_Size);

    in.ignore(3); // Ignoring the space and the colon
    std::copy_n(std::istream_iterator<std::size_t>(in), p.m_Size, p.m_Images.begin());
    return in;
}

size_t Cycle::Size() const
{
    return *std::max_element(m_Elements.begin(), m_Elements.end());
}

std::size_t Cycle::Order() const
{
    return m_Elements.size();
}

Cycle Cycle::Inverse() const
{
    Cycle inverse;
    inverse.AddLastPoint(m_Elements.front());
    std::copy(m_Elements.rbegin(), m_Elements.rend() - 1, std::back_inserter(inverse.m_Elements));
    return inverse;
}

std::list<Cycle> Cycle::Cycles() const
{
    return {*this};
}

size_t Cycle::operator[](size_t i) const
{
    auto it = Find(i);
    if (it == m_Elements.end())
    {
        return i;
    }
    else if (it == m_Elements.end() - 1)
    {
        return m_Elements.front();
    }
    else
    {
        return *(it + 1);
    }
}

void Cycle::AddLastPoint(std::size_t element)
{
    m_Elements.push_back(element);
}

std::vector<size_t>::const_iterator Cycle::Find(size_t i) const
{
    return std::find(m_Elements.begin(), m_Elements.end(), i);
}

bool Cycle::operator<(const Cycle& rhs) const
{
    return m_Elements.size() != rhs.m_Elements.size() ?
           m_Elements.size() < rhs.m_Elements.size() :
           std::lexicographical_compare(m_Elements.begin(), m_Elements.end(), rhs.m_Elements.begin(), rhs.m_Elements.end());
}

std::ostream& operator<<(std::ostream& out, const Cycle& c)
{
    out << "[ ";
    std::copy(c.m_Elements.begin(), c.m_Elements.end(), std::ostream_iterator<std::size_t>(out, " "));
    out << "]";
    return out;
}

size_t SparsePermutation::Size() const
{
    return m_Size;
}

size_t SparsePermutation::operator[](size_t i) const
{
    auto it = m_NonTrivialImages.find(i);
    return it != m_NonTrivialImages.end() ? it->second : i;
}

size_t SparsePermutation::Order() const
{
    std::list<Cycle> cycles = Cycles();
    return std::accumulate(cycles.begin(), cycles.end(), 1L, [](std::size_t p, const Cycle& c) { return Lcm(p, c.Order()); });
}

std::list<Cycle> SparsePermutation::Cycles() const
{
    std::list<Cycle> cycles;
    std::set<std::size_t> s;
    for (std::size_t i = 0; i < m_Size; ++i)
    {
        s.emplace_hint(s.end(), i);
    }

    while (!s.empty())
    {
        std::size_t min = *s.begin();
        if (m_NonTrivialImages.find(min) == m_NonTrivialImages.end())
        {
            continue;
        }
        size_t minImage = m_NonTrivialImages.at(min);
        if (min != minImage)
        {
            Cycle cycle;
            cycle.AddLastPoint(min);

            std::size_t current = minImage;
            while (current != min)
            {
                s.erase(current);
                cycle.AddLastPoint(current);
                current = m_NonTrivialImages.at(current);
            }
            cycles.push_back(cycle);
        }
        s.erase(s.begin());
    }

    return cycles;
}

SparsePermutation operator*(const SparsePermutation& a, const SparsePermutation& b)
{
    auto minmax = std::minmax(a, b, [](const SparsePermutation& a, const SparsePermutation& b) {
        return a.Size() < b.Size();
    });

    const auto& minImages = minmax.first.m_NonTrivialImages;
    const auto& maxImages = minmax.second.m_NonTrivialImages;
    SparsePermutation c = minmax.second;
    for (auto kv : minImages)
    {
        bool containKey = maxImages.find(kv.second) == maxImages.end();
        if (containKey || maxImages.at(kv.second) != kv.first)
        {
            size_t t = maxImages.at(kv.second);
            c.m_NonTrivialImages[kv.first] = containKey ? kv.second : t;
        }
    }

    return c;
}

std::ostream& operator<<(std::ostream& out, const SparsePermutation& p)
{
    out << p.m_Size << " : Sparse " << p.m_NonTrivialImages.size() << "\n";
    for (const auto kv : p.m_NonTrivialImages)
    {
        out << kv.first << " " << kv.second << "\n";
    }
    return out;
}
    
std::istream& operator>>(std::istream& in, SparsePermutation& p)
{
    std::string sparse;
    size_t rows;
    in >> p.m_Size;
    in >> sparse;
    in >> rows;
    for (size_t i = 0; i < rows; ++i)
    {
        std::pair<size_t, size_t> keyValue;
        in >> keyValue.first >> keyValue.second;
        p.m_NonTrivialImages.insert(keyValue);
    }
    return in;
}
    

std::int64_t Pcd(std::int64_t a, std::int64_t b)
{
    std::pair<std::int64_t, std::int64_t> u(a, b);
    while (u.second != 0)
    {
        u = {u.second, u.first % u.second};
    }
    return u.first;
}

std::int64_t Lcm(std::int64_t a, std::int64_t b)
{
    return a * b / Pcd(a, b);
}