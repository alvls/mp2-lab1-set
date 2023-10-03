// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
   if (len < 0)
      throw "Invalid length";
   BitLen = len;
   MemLen = BitLen / (sizeof(TELEM) * CHAR_BIT);
   if (BitLen % (sizeof(TELEM) * CHAR_BIT))
      MemLen++;
   pMem = new TELEM[MemLen];
   for (int i = 0; i < MemLen; i++)
      pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
   pMem = new TELEM[MemLen];
   for (int i = 0; i < MemLen; i++)
      pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
   delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
   if (n < 0 || n >= BitLen)
      throw "Invalid value of n";
   return n / (sizeof(TELEM) * CHAR_BIT);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
   if (n < 0 || n >= BitLen)
      throw "Invalid value of n";
   return 1 << (n % (sizeof(TELEM) * CHAR_BIT));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
   return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
   if (n >= BitLen || n < 0)
      throw "Invalid value of n";
   pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
   if (n >= BitLen || n < 0)
      throw "Invalid value of n";
   pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
   if (n >= BitLen || n < 0)
      throw "Invalid value of n";
   return (pMem[GetMemIndex(n)] >> (n % (sizeof(TELEM) * CHAR_BIT))) & 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
   if (this != &bf) {
      BitLen = bf.BitLen;
      MemLen = bf.MemLen;
      delete[]pMem;
      pMem = new TELEM[MemLen];
      for (int i = 0; i < MemLen; i++) {
         pMem[i] = bf.pMem[i];
      }
   }
   return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
   if (BitLen != bf.BitLen)
      return 0;
   for (int i = 0; i < MemLen; i++) {
      if (pMem[i] != bf.pMem[i])
         return 0;
   }
   return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
   if (BitLen != bf.BitLen)
      return 1;
   for (int i = 0; i < MemLen; i++) {
      if (pMem[i] != bf.pMem[i])
         return 1;
   }
   return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
   int resultLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen; 
   int resultMem = (BitLen > bf.BitLen) ? bf.MemLen : MemLen;
   TBitField temp(resultLen); 

   for (int i = 0; i < resultMem; i++) {
      temp.pMem[i] = pMem[i] | bf.pMem[i];
   }

   return temp; 
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
   int resultLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
   int resultMem = (BitLen > bf.BitLen) ? bf.MemLen : MemLen;
   TBitField temp(resultLen);

   for (int i = 0; i < resultMem; i++) {
      temp.pMem[i] = pMem[i] & bf.pMem[i];
   }

   return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
   TBitField result(BitLen);
   for (int i = 0; i < BitLen; i++) {
      if (GetBit(i) == 0)
         result.SetBit(i);
      else
         result.ClrBit(i);
   }
   return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
   string bitString;
   istr >> bitString;
   for (int i = 0; i < bitString.length(); i++) {
      if (bitString[i] == '1')
         bf.SetBit(i);
      else
         bf.ClrBit(i);
   }
   return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
   for (int i = 0; i < bf.BitLen; i++) {
      ostr << bf.GetBit(i);
   }
   return ostr;
}
