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
    if (len < 1)
        throw outOfRange;
    BitLen = len;
    MemLen = ceil(static_cast<float>(len) / (sizeof(TELEM) * bitsPerByte));
    pMem = new TELEM[MemLen];
    for (size_t i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }

}

TBitField::TBitField(const TBitField &bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
    pMem = new TELEM[BitLen];
    for (size_t i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n > BitLen || n < 0)
        throw outOfRange;
    return n / (sizeof(TELEM) * bitsPerByte);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if(n < 0)
        throw outOfRange;
    return 1 << (n % (sizeof(TELEM) * bitsPerByte));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if(n >= BitLen || n < 0)
        throw outOfRange;
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen || n < 0)
        throw outOfRange;
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= BitLen || n < 0)
        throw outOfRange;
    return (pMem[GetMemIndex(n)] >> (n % (sizeof(TELEM) * bitsPerByte))) & 1;
        
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf)
    {
        delete[]pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[bf.MemLen];
        for (size_t i = 0; i < MemLen; i++)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (size_t i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (bf.BitLen > BitLen)
    {
        TBitField temp(bf);
        for (size_t i = 0; i < MemLen; i++)
        {
            temp.pMem[i] |= this->pMem[i];
        }
        return temp;
    }
    else
    {
        TBitField temp(*this);
        for (size_t i = 0; i < bf.MemLen; i++)
        {
            temp.pMem[i] |= bf.pMem[i];
        }
        return temp;
    } 
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
   
    if (bf.BitLen > BitLen)
    {
        TBitField temp(bf);
        for (size_t i = 0; i < MemLen; i++)
        {
            temp.pMem[i] &= this->pMem[i];
        }
        return temp;
    }
    else
    {
        TBitField temp(*this);
        for (size_t i = 0; i < bf.MemLen; i++)
        {
            temp.pMem[i] &= bf.pMem[i];
        }
        return temp;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(*this);
    for (size_t i = 0; i < temp.BitLen; i++)
    {
        if (GetBit(i) == 0)
            temp.SetBit(i);
        else
            temp.ClrBit(i);
    }
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string strBites;
    istr >> strBites;
    for (size_t i = 0; i < strBites.length(); i++)
    {
        if (strBites[i] == '1')
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (size_t i = bf.BitLen-1; i > 0; i--)
    {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
