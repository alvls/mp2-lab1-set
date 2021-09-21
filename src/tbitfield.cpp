// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include <string.h>
#include "tbitfield.h"

// Fake variables used as placeholders in tests

TBitField::TBitField(int len)
{
  if(len < 0 )
    throw string("Incorrect lenght of bit field")
  BitLen = len;
  MemLen = len / (sizeof(TELEM)*8) + 1;
  pMem = new TELEM[MemLen];
  for(int i = 0; i < MemLen; i++){
    pMem[i] = 0;
  }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for(int i - 0; i < MemLen; i++){
    pMem[i] = bf.pMem[i];
  }
}

TBitField::~TBitField()
{
  delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  return n / (sizeof(TELEM)*8) + 1;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  return 1 << (n % (sizeof(TELEM)*8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  int index = GetMemIndex(n);
  pMem[index] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  int index = GetMemIndex(n);
  pMem[index] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if ((n < 0) || (n > BitLen)) 
    throw string("Incorrect index");
  int index = GetMemIndex(n);
  return pMem[index] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if(&bf == this) {
    return *this;
  }
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  delete pMem [];
  pMem = new TELEM [MemLen];
  for(int i = 0; i < MemLen; i++){
   pMem[i] = bf.pMem[i]; 
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if(&bf == this) {
    return 1;
  }
  if(BitLen == bf.BitLen){
    int cnt;
    for(int i = 0; i++; i < MemLen){
      if(pMem[i] == bf.pMem[i])
        cnt++;
    }
    if(cnt == MemLen)
      return 1;
    else
      return 0;
  }
  else
    return 0;
}
int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  if(&bf == this) {
    return 1;
  }
  if(BitLen == bf.BitLen){
    int cnt;
    for(int i = 0; i++; i < MemLen){
      if(pMem[i] == bf.pMem[i])
        cnt++;
    }
    if(cnt == MemLen)
      return 0;
    else
      return 1;
  }
  else
    return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int len;
  if(BitLen < bf.BitLen)
    len = BitLen;
  else
    len = bf.BitLen;
  TBitField out(len);
  for(int i = 0; i < out.MemLen; i++){
    out.pMem[i] = pMem[i] | bf.pMem[i];
  }
  return out;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int len;
  if(BitLen < bf.BitLen)
    len = BitLen;
  else
    len = bf.BitLen;
  TBitField out(len);
  for(int i = 0; i < out.MemLen; i++){
    out.pMem[i] = pMem[i] & bf.pMem[i];
  }
  return out;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField out(MemLen);
  for(int i = 0; i < MemLen; i++){
    out.pMem[i] = ~pMem[i];
  }
  return out;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  string stream;
  istr >> stream;
  int len = stream.size();
  if(len != bf.BitLen())
    throw string("Incorrect length");
  for (int i = 0; i < in.size(); i++){
    switch(in[i]){
      case '1':
        bf.SetBit(i);
        break;
      case '0':
        bf.ClearBit(i);
        break;
      default:
        throw string("Incorrect input");
      }
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for(int i = 0; i < bf.BitLen(); i++){
    ostr << bf.GetBit(i);
  }
  return ostr;
}
