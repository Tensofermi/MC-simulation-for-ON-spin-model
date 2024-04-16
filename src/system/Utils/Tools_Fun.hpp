#pragma once

#include <bits/stdc++.h>
#include "Union_Find.hpp"

// Transform T to string type
template <class T>
std::string toStr(const T& t)
{
    std::string str;
    std::stringstream stream;
    stream << t;
    stream >> str;
    return str;
}

// Transform string to T type
template <class T>
T fromStr(const std::string& str)
{
    T t;
    std::stringstream stream;
    stream << str;
    stream >> t;
    return t;
}

// Transform double to string, and the precision we can set
std::string dou2str(double number, int precision) 
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << number;
    return oss.str();
}

template <class T>
std::string r_jf(T _var, int _width) 
{
    std::ostringstream oss;
    oss << std::right << std::setw(_width) << _var;
    return oss.str();
}

template <class T>
std::string l_jf(T _var, int _width) 
{
    std::ostringstream oss;
    oss << std::left << std::setw(_width) << _var;
    return oss.str();
}

int pbc_mod(int _a, int _L)
{
    if (_a >= 0)
    {
        return _a % _L;
    }
    else if (_a >= -_L)
    {
        _a += _L;
        return _a;
    }
    else
    {
        _a += _L;
        return pbc_mod(_a, _L);
    }
}

//--- Vector Operation
double dot(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    double result = 0.0;
    for (size_t i = 0; i < vector1.size(); i++)
        result += vector1[i] * vector2[i];

    return result;
}

double operator*(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    double result = 0.0;
    for (size_t i = 0; i < vector1.size(); i++)
        result += vector1[i] * vector2[i];

    return result;
}

std::vector<double> add(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    std::vector<double> result(vector1.size());

    for (size_t i = 0; i < vector1.size(); i++)
        result[i] = vector1[i] + vector2[i];

    return result;
}

std::vector<double> operator+(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    std::vector<double> result(vector1.size());

    for (size_t i = 0; i < vector1.size(); i++)
        result[i] = vector1[i] + vector2[i];

    return result;
}

std::vector<double> subtra(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    std::vector<double> result(vector1.size());

    for (size_t i = 0; i < vector1.size(); i++)
        result[i] = vector1[i] - vector2[i];
        
    return result;
}

std::vector<double> operator-(const std::vector<double>& vector1, const std::vector<double>& vector2) 
{
    std::vector<double> result(vector1.size());

    for (size_t i = 0; i < vector1.size(); i++)
        result[i] = vector1[i] - vector2[i];
        
    return result;
}

double abs(const std::vector<double>& vector) 
{
    double sumOfSquares = 0.0;
    for (size_t i = 0; i < vector.size(); i++)
        sumOfSquares += vector[i] * vector[i];

    return std::sqrt(sumOfSquares);
}

std::vector<double> scalarMulti(double scalar, const std::vector<double>& vector) 
{
    std::vector<double> result(vector.size());
    for (size_t i = 0; i < vector.size(); i++)
        result[i] = vector[i] * scalar;

    return result;
}

std::vector<double> operator*(double scalar, const std::vector<double>& vector) 
{
    std::vector<double> result(vector.size());
    for (size_t i = 0; i < vector.size(); i++)
        result[i] = vector[i] * scalar;

    return result;
}