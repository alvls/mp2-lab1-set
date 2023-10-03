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
        throw "invalid length";
    BitLen = len;
    MemLen = (BitLen - 1) / (sizeof(TELEM) * 8) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
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
    if (n >= BitLen || n < 0)
        throw "invalid index";
    return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM mask;
    mask = 1 << (n % (sizeof(TELEM) * 8));
    return (mask);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return (BitLen);
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= BitLen || n < 0)
        throw "invalid index";
    int ind = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[ind] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen || n < 0)
        throw "invalid index";
    int ind = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[ind] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int i = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return pMem[i] & mask;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf)
    {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    else
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 1;
    else
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return 1;
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int b_len, m_len;
    if (this == &bf)
    {
        TBitField res(*this);
        return res;
    }
    if (BitLen > bf.BitLen)
    {
        b_len = BitLen;
        m_len = bf.MemLen;
    }
    else
    {
        b_len = bf.BitLen;
        m_len = MemLen;
    }
    TBitField res(b_len);
    for (int i = 0;i < m_len;i++)
        res.pMem[i] = pMem[i] | bf.pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int b_len, m_len;
    if (this == &bf)
    {
        TBitField res(*this);
        return res;
    }
    if (BitLen > bf.BitLen)
    {
        b_len = BitLen;
        m_len = bf.MemLen;
    }
    else
    {
        b_len = bf.BitLen;
        m_len = MemLen;
    }
    TBitField res(b_len);
    for (int i = 0;i < m_len;i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i) == 0)
            res.SetBit(i);
        else
            res.ClrBit(i);
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char s;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> s;
        if (s == '1')
            bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int tmp;
    for (int i = 0; i < bf.BitLen; i++)
    {
        tmp = bf.GetBit(i);
        ostr << tmp;
    }
    return ostr;
}
