// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

///Tset(0)

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
    ;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
    ; ///test pls
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
    ;
}

TSet::operator TBitField()
{
    TBitField bf = BitField;
    return bf;
} ///??

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return !!(BitField.GetBit(Elem));
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (*this != s) {
        BitField = s.BitField;
        MaxPower = s.MaxPower;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return ((MaxPower == s.MaxPower) && (BitField == s.BitField)); 
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TBitField bf = (BitField | s.BitField);
    TSet tmp(bf);
    return tmp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet st(*this);
    st.InsElem(Elem);
    return st;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet st(*this);
    st.DelElem(Elem);
    return st;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TBitField bf = (BitField & s.BitField);
    TSet tmp(bf);
    return tmp;
}

TSet TSet::operator~(void) // дополнение
{
    TBitField bf = (~BitField);
    TSet st(bf);
    return st;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    return istr>>s.BitField;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    return ostr<< s.BitField;
}
