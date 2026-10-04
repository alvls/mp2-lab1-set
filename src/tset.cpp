#include "tset.h"


TSet::TSet(int mp)
    : MaxPower(mp), BitField(mp)
{
    if (mp <= 0)
        throw "Set size must be positive";
}


TSet::TSet(const TSet& s)
    : MaxPower(s.MaxPower), BitField(s.BitField)
{
}


TSet::TSet(const TBitField& bf)
    : MaxPower(bf.GetLength()), BitField(bf)
{
}


TSet::operator TBitField()
{
    return BitField;
}


int TSet::GetMaxPower(void) const
{
    return MaxPower;
}


int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element is out of range";

    return BitField.GetBit(Elem);
}


void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element is out of range";

    BitField.SetBit(Elem);
}


void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element is out of range";

    BitField.ClrBit(Elem);
}


TSet& TSet::operator=(const TSet& s)
{
    if (this != &s)
    {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }

    return *this;
}


int TSet::operator==(const TSet& s) const
{
    if (MaxPower != s.MaxPower)
        return 0;

    return BitField == s.BitField;
}


int TSet::operator!=(const TSet& s) const
{
    return !(*this == s);
}


TSet TSet::operator+(const TSet& s)
{
    TBitField result = BitField | s.BitField;

    return TSet(result);
}


TSet TSet::operator+(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element is out of range";

    TSet result(*this);
    result.InsElem(Elem);

    return result;
}


TSet TSet::operator-(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw "Element is out of range";

    TSet result(*this);
    result.DelElem(Elem);

    return result;
}


TSet TSet::operator*(const TSet& s)
{
    TBitField result = BitField & s.BitField;

    return TSet(result);
}


TSet TSet::operator~(void)
{
    TBitField result = ~BitField;

    return TSet(result);
}


istream& operator>>(istream& istr, TSet& s)
{
    int count;
    int element;

    istr >> count;

    for (int i = 0; i < count; i++)
    {
        istr >> element;
        s.InsElem(element);
    }

    return istr;
}


ostream& operator<<(ostream& ostr, const TSet& s)
{
    ostr << "{";

    bool first = true;

    for (int i = 0; i < s.MaxPower; i++)
    {
        if (s.BitField.GetBit(i))
        {
            if (!first)
                ostr << ", ";

            ostr << i;
            first = false;
        }
    }

    ostr << "}";

    return ostr;
}