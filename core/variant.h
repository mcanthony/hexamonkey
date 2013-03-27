//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef VARIANT_H
#define VARIANT_H


#include <exception>
#include <iostream>

#include <string>
class ObjectType;

/*!
 * \brief The Variant class
 */
class Variant
{
public:
    enum Type{
        unknown = 0,
        integer = 1,
        unsignedInteger = 2,
        floating = 3,
        string = 4,
        objectType = 5
    };

    class InvalidTypeConversionException: public std::exception
    {
      virtual const char* what() const throw()
      {
        return "Variant: invalid type conversion";
      }
    };

    class InvalidOperationException: public std::exception
    {
      virtual const char* what() const throw()
      {
        return "Variant: invalid operation";
      }
    };

    Variant();

    Variant(const Variant& other);

    Variant(bool l);
    Variant(char l);
    Variant(int l);
    Variant(long l);
    Variant(long long l);

    Variant(unsigned char ul);
    Variant(unsigned int ul);
    Variant(unsigned long ul);
    Variant(unsigned long long ul);

    Variant(float f);
    Variant(double f);

    Variant(const std::string& s);
    Variant(const char* s);

    Variant(const ObjectType& t);

    ~Variant();

    void setValue(Variant other);

    void setValue(bool l);
    void setValue(char l);
    void setValue(int l);
    void setValue(long l);
    void setValue(long long l);

    void setValue(unsigned char ul);
    void setValue(unsigned int ul);
    void setValue(unsigned long ul);
    void setValue(unsigned long long ul);

    void setValue(float f);
    void setValue(double f);

    void setValue(const std::string& s);
    void setValue(const char* s);

    void setValue(const ObjectType& t);

    void clear();

    Variant& operator=(Variant other);
    friend void swap(Variant& a, Variant& b);

    bool canConvertTo(Type otherType) const;
    Variant &convertTo(Type newType);

    long long          toInteger()         const;
    unsigned long long toUnsignedInteger() const;
    double             toDouble()          const;
    const std::string& toString()          const;
    const ObjectType&  toObjectType()      const;
    ObjectType&        toObjectType()           ;
    bool               toBool()            const;

    Type type() const;
    bool hasNumericalType() const;

    friend bool operator==(const Variant& a, const Variant& b);
    friend bool operator< (const Variant& a, const Variant& b);

    Variant& operator+=(const Variant& other);
    Variant& operator-=(const Variant& other);
    Variant& operator*=(const Variant& other);
    Variant& operator/=(const Variant& other);
    Variant& operator%=(const Variant& other);

    Variant& operator++();
    Variant  operator++(int);
    Variant& operator--();
    Variant  operator--(int);


    Variant operator-() const;

    Variant& operator|=(const Variant& other);
    Variant& operator^=(const Variant& other);
    Variant& operator&=(const Variant& other);
    Variant& operator<<=(const Variant& other);
    Variant& operator>>=(const Variant& other);

    Variant operator~() const;

    bool operator!() const;

    std::ostream& display(std::ostream& out) const;


private:
    typedef union{
        long long l;
        unsigned long long ul;
        double f;
        std::string* s;
        ObjectType* t;
    } Data;

    Data _data;
    Type _type;

};

void swap(Variant& a, Variant& b);

Variant operator+ (Variant a, const Variant& b);
Variant operator- (Variant a, const Variant& b);
Variant operator* (Variant a, const Variant& b);
Variant operator/ (Variant a, const Variant& b);
Variant operator% (Variant a, const Variant& b);

Variant operator| (Variant a, const Variant& b);
Variant operator^ (Variant a, const Variant& b);
Variant operator& (Variant a, const Variant& b);
Variant operator<<(Variant a, const Variant& b);
Variant operator>>(Variant a, const Variant& b);

bool operator==(const Variant& a, const Variant& b);
bool operator!=(const Variant& a, const Variant& b);
bool operator< (const Variant& a, const Variant& b);
bool operator<=(const Variant& a, const Variant& b);
bool operator> (const Variant& a, const Variant& b);
bool operator>=(const Variant& a, const Variant& b);

bool operator&&(const Variant& a, const Variant& b);
bool operator||(const Variant& a, const Variant& b);

std::ostream& operator<<(std::ostream& out, const Variant& variant);


#endif // VARIANT_H