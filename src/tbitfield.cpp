#include "tbitfield.h"

TBitField::TBitField(int len) 
{
    if (len <= 0) {
        throw invalid_argument("Length must be positive");
    }

    BitLen = len;
    MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];
    memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("Bit index out of range");
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("Bit index out of range");
    return 1 << (n & (sizeof(TELEM) * 8 - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("Bit index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("Bit index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("Bit index out of range");
    return ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
        memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField res(len);

    int minMemLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > bf.MemLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            res.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = minMemLen; i < bf.MemLen; i++) {
            res.pMem[i] = bf.pMem[i];
        }
    }

    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField res(len);

    int minMemLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i)) res.ClrBit(i);
        else res.SetBit(i);
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char bit;
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> bit;
        if (bit == '1') bf.SetBit(i);
        else if (bit == '0') bf.ClrBit(i);
        else
            throw invalid_argument("Invalid character in bitfield input");
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
