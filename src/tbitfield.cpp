// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
constexpr TELEM MaxElem = ~TELEM(0);

// Fake variables used as placeholders in tests
//static const int FAKE_INT = -1;
//static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) : BitLen(len), MemLen((len / BitsInElem) + (len % BitsInElem != 0))
{
    if (len < 0)
    {
        throw exception("Incorrent length value");
    }
    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
    : BitLen(bf.BitLen)
    , MemLen(bf.MemLen)
{
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / BitsInElem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % BitsInElem);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= BitLen || n < 0)
    {
        throw exception("Invalid n argument");
    }

    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen || n < 0)
    {
        throw exception("Invalid n argument");
    }

    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= BitLen || n < 0)
    {
        throw exception("Invalid n argument");
    }

    return (pMem[GetMemIndex(n)] >> (n % BitsInElem)) & 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen)
    {
        MemLen = bf.MemLen;
        TELEM* tmp = new TELEM[MemLen];
        delete[] pMem;
        pMem = tmp;
    }
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
    {
        return 0;
    }
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
        {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    const TBitField* less = MemLen > bf.MemLen ? &bf : this;
    TBitField res = MemLen > bf.MemLen ? TBitField(*this) : TBitField(bf);

    for (int i = 0; i < less->MemLen; i++)
    {
        res.pMem[i] |= less->pMem[i];
    }
       
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    
    TBitField res(max(BitLen, bf.BitLen));
    const int min_len = min(MemLen, bf.MemLen);

    for (int i = 0; i < min_len; i++)
    {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
        
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen - 1; i++)
    {
        res.pMem[i] = ~pMem[i];
    }
    res.pMem[MemLen - 1] = pMem[MemLen - 1] ^ (MaxElem >> (BitsInElem - (BitLen & (BitsInElem - 1))));
        
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char c;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> c;
        switch (c)
        {
        case '0':
            bf.ClrBit(i);
            break;
        case '1':
            bf.SetBit(i);
            break;
        default:
            continue;
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        ostr << (bf.GetBit(i) ? 1 : 0);
    }

    return ostr;
}
