#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len <= 0)
        throw "Bit field length must be positive";

    BitLen = len;

    int bitsPerElem = sizeof(TELEM) * 8;
    MemLen = (BitLen + bitsPerElem - 1) / bitsPerElem;

    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}


TBitField::TBitField(const TBitField& bf)
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


int TBitField::GetMemIndex(const int n) const
{
    return n / (sizeof(TELEM) * 8);
}


TELEM TBitField::GetMemMask(const int n) const
{
    return (TELEM)1 << (n % (sizeof(TELEM) * 8));
}


int TBitField::GetLength(void) const
{
    return BitLen;
}


void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw "Bit index is out of range";

    pMem[GetMemIndex(n)] |= GetMemMask(n);
}


void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw "Bit index is out of range";

    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}


int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw "Bit index is out of range";

    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}


TBitField& TBitField::operator=(const TBitField& bf)
{
    if (this == &bf)
        return *this;

    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        pMem = new TELEM[bf.MemLen];
    }

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}


int TBitField::operator==(const TBitField& bf) const
{
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }

    return 1;
}


int TBitField::operator!=(const TBitField& bf) const
{
    return !(*this == bf);
}


TBitField TBitField::operator|(const TBitField& bf)
{
    int resultLength;

    if (BitLen > bf.BitLen)
        resultLength = BitLen;
    else
        resultLength = bf.BitLen;

    TBitField result(resultLength);

    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i))
            result.SetBit(i);
    }

    for (int i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i))
            result.SetBit(i);
    }

    return result;
}


TBitField TBitField::operator&(const TBitField& bf)
{
    int resultLength;

    if (BitLen > bf.BitLen)
        resultLength = BitLen;
    else
        resultLength = bf.BitLen;

    TBitField result(resultLength);

    int minLength;

    if (BitLen < bf.BitLen)
        minLength = BitLen;
    else
        minLength = bf.BitLen;

    for (int i = 0; i < minLength; i++)
    {
        if (GetBit(i) && bf.GetBit(i))
            result.SetBit(i);
    }

    return result;
}


TBitField TBitField::operator~(void)
{
    TBitField result(BitLen);

    for (int i = 0; i < BitLen; i++)
    {
        if (!GetBit(i))
            result.SetBit(i);
    }

    return result;
}


istream& operator>>(istream& istr, TBitField& bf)
{
    char bit;

    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> bit;

        if (bit == '1')
            bf.SetBit(i);
        else if (bit == '0')
            bf.ClrBit(i);
    }

    return istr;
}


ostream& operator<<(ostream& ostr, const TBitField& bf)
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);

    return ostr;
}