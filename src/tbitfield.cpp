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
        throw "ERROR: incorrect length";
    BitLen = len;
    MemLen = (BitLen - 1) / (sizeof(TELEM) * CHAR_BIT) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n +
{
    if (n > BitLen - 1 || n < 0)
        throw "ERROR: incorrect index";
    return ((n - 1) / (sizeof(TELEM) * CHAR_BIT));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n +
{
    const int num = GetMemIndex(n);
    TELEM mask;
    const int offset = n % (sizeof(TELEM) * CHAR_BIT);//смещение
    mask = 1 << offset;
    mask &= pMem[n / (sizeof(TELEM) * CHAR_BIT)];
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n>BitLen - 1)
        throw "ERROR: incorrect index";
    TELEM tmp;
    tmp = (1 << n);
    pMem[n / (sizeof(TELEM) * CHAR_BIT)] |= tmp;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n>BitLen - 1)
        throw "ERROR: incorrect index";
    TELEM tmp = (1 << n);
    tmp = ~tmp;
    pMem[n / (sizeof(TELEM) * CHAR_BIT)] &= tmp;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n>BitLen - 1)
        throw "ERROR: incorrect index";
    TELEM tmp = GetMemMask(n);
    if (tmp > 0)
        return 1;
    else
        return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание +
{
    if (&bf != this)
    {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение +
{
    if (bf.BitLen != BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение +
{
    if (bf.BitLen != BitLen)
        return 1;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 1;
    return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int max_len;
    int mem_len;
    if (BitLen > bf.BitLen)
    {
        max_len = BitLen;
        mem_len = bf.MemLen;
    }
    else
    {
        max_len = bf.BitLen;
        mem_len = MemLen;
    }
    TBitField out(max_len);
    int i;
    for (i = 0; i < mem_len; i++)
    {
        out.pMem[i] = bf.pMem[i] | pMem[i];
    }
    if (BitLen > bf.BitLen)
    {
        for (; i < MemLen; i++)
        {
            out.pMem[i] = pMem[i];
        }
    }
    else
    {
        for (; i < bf.MemLen; i++)
        {
            out.pMem[i] = bf.pMem[i];
        }
    }
    return out;
}


TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int max_len;
    int mem_len;
    if (BitLen > bf.BitLen)
    {
        max_len = BitLen;
        mem_len = bf.MemLen;
    }
    else
    {
        max_len = bf.BitLen;
        mem_len = MemLen;
    }
    TBitField out(max_len);
    for (int i = 0; i < mem_len; i++)
    {
        out.pMem[i] = bf.pMem[i] & pMem[i];
    }
    return out;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField out(*this);
    for (int i = 0; i < BitLen; i++)
        if (out.GetBit(i) > 0)
            out.ClrBit(i);
        else
            out.SetBit(i);
    return out;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод 
{
    string b_string;
    istr >> b_string;
    TBitField bf1(b_string.length());
    for (int i = 0; i < b_string.length(); i++)
    {
        if (b_string[i] == '1')
            bf1.SetBit(i);
    }
    bf = bf1;
    return istr;
}


ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    int out;
    for (int i = 0; i < bf.BitLen; i++)
    {
        out = bf.GetBit(i);
        ostr << out;
    }
    ostr << '\n';
    return ostr;
}
