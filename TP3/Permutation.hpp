#pragma once

#include <iostream>
#include <list>
#include <map>
#include <random>
#include <vector>

class Cycle;

class ImmutablePermutation
{
public:
    virtual ~ImmutablePermutation() = default;
public:
    virtual size_t Size() const = 0;
    virtual size_t operator[](size_t i) const = 0;
    virtual size_t Order() const = 0;
    virtual std::list<Cycle> Cycles() const = 0;
};

class Permutation : public ImmutablePermutation
{
public:
    template <typename RNG>
    Permutation(size_t size, RNG& g);
    Permutation(size_t size = 1);
    Permutation(const std::vector<size_t>& images);

    size_t Size() const override;
    size_t operator[](size_t i) const override;
    size_t Order() const override;
    std::list<Cycle> Cycles() const override;

    Permutation Inverse() const;
    bool IsDerangement() const;
    std::list<size_t> FixedPoints() const;
public:
    friend Permutation operator*(const Permutation& a, const Permutation& b);
    friend std::ostream& operator<<(std::ostream& out, const Permutation& p);
    friend std::istream& operator>>(std::istream& in, Permutation& p);
private:
    size_t m_Size;
    std::vector<size_t> m_Images;
};

template <typename RNG>
Permutation::Permutation(std::size_t size, RNG& g)
    : Permutation(size)
{
    for (size_t i = 0; i < size - 1; ++i)
    {
        std::uniform_int_distribution<size_t> u(i, size - 1);
        std::swap(m_Images[i], m_Images[u(g)]);
    }
}

class SparsePermutation : public ImmutablePermutation
{
public:
    size_t Size() const override;
    size_t operator[](size_t i) const override;
    size_t Order() const override;
    std::list<Cycle> Cycles() const override;
public:
    friend SparsePermutation operator*(const SparsePermutation& a, const SparsePermutation& b);
    friend std::ostream& operator<<(std::ostream& out, const SparsePermutation& p);
    friend std::istream& operator>>(std::istream& in, SparsePermutation& p);
private:
    size_t m_Size;
    std::map<size_t, size_t> m_NonTrivialImages;
};

class Cycle : public ImmutablePermutation
{
public:
    size_t Size() const override;
    size_t operator[](size_t i) const override;
    size_t Order() const override;
    std::list<Cycle> Cycles() const override;

    Cycle Inverse() const;
    bool operator<(const Cycle& rhs) const;
public:
    friend Permutation;
    friend SparsePermutation;
    friend std::ostream& operator<<(std::ostream& out, const Cycle& c);
private:
    void AddLastPoint(size_t element);
    std::vector<size_t>::const_iterator Find(size_t i) const;
private:
    std::vector<size_t> m_Elements;
};
