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

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
  TBitField result(this->BitField);
  return result;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
  return MaxPower;
}

bool TSet::IsMember(const int Elem) const // элемент множества?
{
  return BitField.GetBit(Elem);
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
  if(this == &s)
    return *this;
  MaxPower = s.MaxPower;
  BitField = s.BitField;
  return *this;
}

bool TSet::operator==(const TSet &s) const // сравнение
{
  return BitField == s.BitField;
}

bool TSet::operator!=(const TSet &s) const // сравнение
{
  return BitField != s.BitField;
}

TSet TSet::operator+(const TSet &s) // объединение
{
  TSet result(BitField | s.BitField);
  return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
  TSet result(BitField);
  result.InsElem(Elem);
  return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
  TSet result(BitField);
  result.DelElem(Elem);
  return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
  TSet result(BitField & s.BitField);
  return result;
}

TSet TSet::operator~(void) // дополнение
{
  TSet result(~BitField);
  return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
  //{ a, b, ..., n }
  char temp;
  do
  {
    istr >> temp;
  } while(temp != '{'); // a b
  istr >> temp;
  do
  {
    istr >> temp;
    s.InsElem(temp);
    istr >> temp >> temp;
  } while(temp != '}');
  return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
  char symbol = ' ';
  ostr << '{';
  for(int i = 0; i < s.MaxPower; i++)
  {
    if(s.IsMember(i))
    {
      ostr << symbol << i;
      symbol = ',';
    }
  }
  ostr << '}';
  return ostr;
}
