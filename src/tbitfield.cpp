// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw FAKE_INT;
    BitLen = len;
    MemLen = (len + sizeof(TELEM)*8 - 1)/ (sizeof(TELEM)*8);
    pMem = new TELEM[MemLen];
    for(int i=0;i<MemLen;i++){
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new char[MemLen];
    for(int i=0;i<MemLen;i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen)
        throw FAKE_INT;
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > BitLen)
        throw FAKE_INT;
    return TELEM(1 << n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if(n < 0 || n > BitLen)
    throw FAKE_INT;
  pMem[n / (sizeof(TELEM) * 8)] = pMem[n / (sizeof(TELEM) * 8)] | (1 << n % (sizeof(TELEM) * 8));
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if(n < 0 || n > BitLen)
    throw FAKE_INT;
  pMem[n / (sizeof(TELEM) * 8)] = pMem[n/ (sizeof(TELEM) * 8)] & !(1 << n % (sizeof(TELEM) * 8));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if(n < 0 || n > BitLen)
    throw FAKE_INT;
  return int(pMem[n/ (sizeof(TELEM) * 8)] & (1 << n % (sizeof(TELEM) * 8)));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new char[MemLen];
  for(int i=0;i<MemLen;i++)
    pMem[i] = bf.pMem[i];
  return &this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  int i;
  if(BitLen != bf.BitLen)
    return 0;
  if(pMem == bf.pMem)
    return 1;
  for(i = 0;i<MemLen;i++)
    if(pMem[i] != bf.pMem[i])
      break;
  if(i != MemLen)
    return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  TBitField res;
  if(BitLen != bf.BitLen)
    throw FAKE_INT;
  if(pMem == bf.pMem)
    return *this;
  for(int i = 0;i<MemLen;i++)
    res.pMem[i] = pMem[i] | bf.pMem[i];
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  TBitField res;
  if(BitLen != bf.BitLen)
    throw FAKE_INT;
  if(pMem == bf.pMem)
    return *this;
  for(int i = 0;i<MemLen;i++)
    res.pMem[i] = pMem[i] & bf.pMem[i];
  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField res;
  for(int i = 0;i<MemLen;i++)
    res.pMem[i] = ~pMem[i]; 
  return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  string str;
  istr >> str;
  if (str.length() != bf.BitLen)
      throw FAKE_INT;
  for (int i = 0; i < bf.BitLen; i++)
      if (str[i] == '1')
          bf.SetBit(i);
      else if (str[i] == '0')
          bf.ClrBit(i);
      else
          throw FAKE_INT;
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    string str;
    for (int i = 0; i < bf.BitLen; i++)
        str += bf.GetBit(i).to_string();
    ostr << str;
    return ostr;
}
