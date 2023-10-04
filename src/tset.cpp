// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(0)
{
    BitField = TBitField(mp);
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) :MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    TBitField result(BitField.GetLength());
    for (int i = 0; i < BitField.GetLength(); i++)
    {
        if (BitField.GetBit(i))
        {
            result.SetBit(i);
        }
    }
    return result;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return BitField.GetLength();
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem >= 0 && Elem < BitField.GetLength())
    {
        if (BitField.GetBit(Elem))
        {
            return 1;
        }
    }
    return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= BitField.GetLength())
    {
        throw out_of_range("Попытка вставки элемента за пределами допустимого диапазона");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem >= 0 && Elem < BitField.GetLength())
        BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s)
        BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return (BitField == s.BitField);
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return (BitField != s.BitField);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet result(BitField);
    if (Elem >= 0 && Elem < BitField.GetLength())
        result.BitField.ClrBit(Elem);
    return result;

}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TSet result(BitField & s.BitField);
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(BitField);
    for (int i = 0; i < BitField.GetLength(); i++)
    {
        if (BitField.GetBit(i))
        {
            result.BitField.ClrBit(i);  // Очищаем бит, если он был установлен
        }
        else
        {
            result.BitField.SetBit(i);  // Устанавливаем бит, если он был очищен
        }
    }
    return result;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
