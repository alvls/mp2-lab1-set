// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cassert>


// Fake variables used as placeholders in tests

static constexpr int bitsInElem = numeric_limits<TELEM>::digits;

TBitField::TBitField(int len): MemLen((len / bitsInElem) + ((len % bitsInElem) != 0))
{
	if (len < 0)
		throw exception("incorrect length");
	BitLen = len;
	pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField &bf): BitLen(bf.BitLen), MemLen(bf.MemLen)// конструктор копирования
{
	pMem = new TELEM[MemLen];
	copy(bf.pMem, bf.pMem + bf.MemLen, pMem);
}

inline void TBitField::IsValidIndex(const int index) const {
	if (index >= BitLen || index < 0) 
		throw exception("incorrect index");
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const noexcept // индекс в pМем для бита n 
{       
	 return n / bitsInElem;
	 //return n >> bitsInElem;
}

TELEM TBitField::GetMemMask(const int n) const noexcept // битовая маска для бита n
{       
	return 1 << (n & (bitsInElem - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	IsValidIndex(n);
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	IsValidIndex(n);
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	IsValidIndex(n);
	return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание 
{
	if(this != &bf) {
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;

		delete[] pMem;
		pMem = new TELEM[MemLen];

		copy(bf.pMem, bf.pMem + bf.MemLen, pMem);
	}

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) 
		return false; 
	
	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i])
			return false;
	}

	return true;
}

int TBitField::operator!=(const TBitField &bf) const// сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	const bool more = BitLen > bf.BitLen;
	const TBitField* rhs = more ? &bf : this;
	TBitField res = more ? TBitField(*this) : TBitField(bf);
	/*const TBitField* rhs;
	TBitField res(1);
	if (BitLen > bf.BitLen) {
		 rhs = &bf;
		 res = *this;
	}
	else {
		res = bf;
		rhs = this;
	}*/
	for (int i = 0; i < res.MemLen; i++) {
		res.pMem[i] |= rhs->pMem[i]; 
	}

	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	/*assert(BitLen == bf.BitLen);

	TBitField result(BitLen);

	for (int i = 0; i < MemLen; i++)
		result.pMem[i] = pMem[i] & bf.pMem[i];

	return result;*/
	TBitField res(max(BitLen, bf.BitLen));
	const int less = min(MemLen, bf.MemLen);

	for (int i = 0; i < less; i++) {
		res.pMem[i] = pMem[i] & bf.pMem[i];
	}

	return res;
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

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string str;
	istr >> str;

	assert(str.length() == bf.BitLen);

	for (int i = 0; i < bf.BitLen; i++) {
		bf.ClrBit(i);
		if (str[i] == '1')
			bf.SetBit(i);
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		//ostr << bf.GetBit(i);
		if (bf.GetBit(i) != 0) ostr << 1;
		else ostr << bf.GetBit(i);
	return ostr;
}
