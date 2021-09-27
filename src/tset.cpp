// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.GetMaxPower();
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if ((Elem < MaxPower) & (Elem >= 0))
    {
        if (BitField.GetBit(Elem) == 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        throw "Bad value of the variable";
    }
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem < MaxPower) & (Elem >= 0))
    {
        BitField.SetBit(Elem);
    }
    else
    {
        throw "Bad value of the variable";
    }
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((Elem < MaxPower) & (Elem >= 0))
    {
        BitField.ClrBit(Elem);
    }
    else
    {
        throw "Negative n";
    }
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (BitField == s.BitField) 
        return 1; 
    else 
        return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (MaxPower != s.MaxPower)
    {
        return 1;
    }
    if (BitField != s.BitField)
        return 1;
    else
        return 0;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet a(BitField | s.BitField);
    return a;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet a(*this);
    if ((Elem < MaxPower) & (Elem >= 0)) {
        a.InsElem(Elem);
    }
    else
    {
        throw "Bad value of the variable";
    }
    return a;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet a(*this);
    if ((Elem < MaxPower) & (Elem >= 0)) {
        a.DelElem(Elem);
    }
    else
    {
        throw "Bad value of the variable";
    }
    return a;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet a(BitField & s.BitField);
    return a;
}

TSet TSet::operator~(void) // дополнение
{
    TSet a(~BitField);
    return a;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    cin >> s.BitField;
    s.MaxPower = s.BitField.GetLength();
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    cout << "BitField of set: " << s.BitField;
    return ostr;
}
