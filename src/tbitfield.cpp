// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

bool isCorrectBitIndex(const int n, int BitLen)
{
    if (n < 0 || n > BitLen - 1) return false;
    return true;
}

TBitField::TBitField(int len)
{
    if (len < 1) throw std::length_error("Invalid bit field length");
    BitLen = len;
    MemLen = (int)(ceil( (float)len / (sizeof(TELEM) * 8) ));
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (!isCorrectBitIndex(n, BitLen)) throw std::invalid_argument("Incorrect n value");
    return (int)(n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (!isCorrectBitIndex(n, BitLen)) throw std::invalid_argument("Incorrect n value");
    return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (!isCorrectBitIndex(n, BitLen)) throw std::invalid_argument("Incorrect n value");
    pMem[GetMemIndex(n)] |= GetMemMask(n);

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (!isCorrectBitIndex(n, BitLen)) throw std::invalid_argument("Incorrect n value");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (!isCorrectBitIndex(n, BitLen)) throw std::invalid_argument("Incorrect n value");
    return (int)(( pMem[GetMemIndex(n)] & GetMemMask(n)) >> (n % (sizeof(TELEM) * 8)) );
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 1;
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i]) return 1;
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (BitLen > bf.BitLen)
    {
        TBitField t(*this);
        for (int i = 0; i < MemLen; i++) t.pMem[i] |= bf.pMem[i];
        return t;
    }
    else
    {
        TBitField t(bf);
        for (int i = 0; i < MemLen; i++) t.pMem[i] |= pMem[i];
        return t;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (BitLen > bf.BitLen)
    {
        TBitField t(*this);
        for (int i = 0; i < MemLen; i++) t.pMem[i] &= bf.pMem[i];
        return t;
    }
    else
    {
        TBitField t(bf);
        for (int i = 0; i < MemLen; i++) t.pMem[i] &= pMem[i];
        return t;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField t(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i) != 0) t.ClrBit(i);
        else t.SetBit(i);
    }
    return t;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    std::string tstr; istr >> tstr;
    bf = TBitField((int)tstr.size());
    for (int i = 0; i < tstr.size(); i++)
    {
        if (tstr[i] == '1') bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) ostr << bf.GetBit(i);
    return ostr;
}
