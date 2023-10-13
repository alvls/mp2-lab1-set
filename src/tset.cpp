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

TSet::TSet(int mp) : MaxPower(mp),BitField(mp)
{
   
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField),MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf),MaxPower(bf.GetLength())
{
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
    if (BitField.GetBit(Elem) == 1)
        return 1;
    return 0;
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
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if ((MaxPower != s.MaxPower) || (BitField!=s.BitField))
        return 0;
    return 1;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    TSet tmp(*this);
    if (tmp == s)
        return 0;
    return 1;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet ans(max(MaxPower, s.MaxPower));
    TSet tmp(*this);
    for (int i = 0; i < ans.MaxPower; i++) {
        if ((i <= tmp.MaxPower) && (i <= s.MaxPower)&&(tmp.IsMember(i) || s.IsMember(i)))
            ans.InsElem(i);
        if (tmp.MaxPower > s.MaxPower) {
            if ((i > s.MaxPower) && (tmp.IsMember(i)))
                ans.InsElem(i);
        }
        else {
            if ((i > tmp.MaxPower) && (s.IsMember(i)))
                ans.InsElem(i);
        }
    }
    return ans;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem > MaxPower)
        throw "Elem out of range";
    TSet tmp(*this);
    if (tmp.IsMember(Elem))
        return tmp;
    tmp.InsElem(Elem);
    return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmp(*this);
    if (tmp.IsMember(Elem)) {
        tmp.DelElem(Elem);
        return tmp;
    }
    return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet ans(max(MaxPower, s.MaxPower));
    TSet tmp(*this);
    for (int i = 0; i < ans.MaxPower; i++) {
        if (i <= min(MaxPower, s.MaxPower)) {
            if (tmp.IsMember(i) && s.IsMember(i))
                ans.InsElem(i);
        }      
    }
    return ans;
}

TSet TSet::operator~(void) // дополнение
{
    TSet tmp(*this);
    for (int i = 0; i < MaxPower; i++) {
        if (tmp.IsMember(i))
            tmp.DelElem(i);
        else
            tmp.InsElem(i);
    }
    return tmp;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int m,n,elem;
    cout << "Enter MaxPower\n";
    cin >> m;
    TSet tmp(m);
    cout << "Enter number of elements\n";
    cin >> n;
    for (int i = 0; i < n; i++) {
        cout << "Enter Element \n";
        cin >> elem;
        tmp.InsElem(elem);
    }

    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{

    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i))
            ostr << i;
    }
    return ostr;
}
