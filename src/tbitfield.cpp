#include "tbitfield.h"

// Конструктор, инициализирующий битовое поле с заданной длиной.
TBitField::TBitField(int length) {
    if (length < 0) throw - 1; // Исключение для отрицательной длины
    BitLen = length; // Устанавливаем длину битового поля
    // Рассчитываем необходимое количество ячеек памяти для хранения битов
    MemLen = (length + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen](); // Инициализируем память под биты
}

// Конструктор копирования, создающий новую копию битового поля на основе другого.
TBitField::TBitField(const TBitField& otherField) {
    BitLen = otherField.BitLen; // Копируем длину
    MemLen = otherField.MemLen; // Копируем количество ячеек памяти
    pMem = new TELEM[MemLen]; // Выделяем память для копии
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = otherField.pMem[i]; // Копируем данные из другого поля
    }
}

// Деструктор, освобождающий выделенную память.
TBitField::~TBitField() {
    delete[] pMem; // Освобождаем память, выделенную под биты
}

// Получение индекса в памяти для заданного бита.
int TBitField::GetMemIndex(int bitPos) const {
    if (bitPos < 0 || bitPos >= BitLen) throw - 1; // Исключение для некорректного индекса
    return bitPos / (sizeof(TELEM) * 8); // Возвращаем индекс ячейки памяти
}

// Создание маски для определенного бита.
TELEM TBitField::GetMemMask(int bitPos) const {
    return 1 << (bitPos % (sizeof(TELEM) * 8)); // Возвращаем маску для бита
}

// Возвращает длину битового поля.
int TBitField::GetLength() const {
    return BitLen; // Возвращаем длину
}

// Устанавливает бит в заданной позиции.
void TBitField::SetBit(int bitPos) {
    if (bitPos < 0) throw - 1; // Исключение для некорректного индекса
    pMem[GetMemIndex(bitPos)] |= GetMemMask(bitPos); // Устанавливаем бит
}

// Сбрасывает бит в заданной позиции.
void TBitField::ClrBit(int bitPos) {
    if (bitPos < 0) throw - 1; // Исключение для некорректного индекса
    pMem[GetMemIndex(bitPos)] &= ~GetMemMask(bitPos); // Сбрасываем бит
}

// Получает значение бита в заданной позиции (1 или 0).
int TBitField::GetBit(int bitPos) const {
    if (bitPos < 0) throw - 1; // Исключение для некорректного индекса
    return (pMem[GetMemIndex(bitPos)] & GetMemMask(bitPos)) ? 1 : 0; // Возвращаем значение бита
}

// Перегрузка оператора присваивания.
TBitField& TBitField::operator=(const TBitField& otherField) {
    if (this != &otherField) { // Проверяем самоприсваивание
        delete[] pMem; // Освобождаем старую память
        BitLen = otherField.BitLen; // Копируем длину
        MemLen = otherField.MemLen; // Копируем количество ячеек памяти
        pMem = new TELEM[MemLen]; // Выделяем новую память
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = otherField.pMem[i]; // Копируем данные из другого поля
        }
    }
    return *this; // Возвращаем текущий объект
}

// Перегрузка оператора сравнения на равенство.
int TBitField::operator==(const TBitField& otherField) const {
    if (BitLen != otherField.BitLen) return false; // Проверяем длины битовых полей
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != otherField.pMem[i]) return false; // Проверяем значения битов
    }
    return true; // Все равны
}

// Перегрузка оператора сравнения на неравенство.
int TBitField::operator!=(const TBitField& otherField) const {
    return !(*this == otherField); // Проверяем на неравенство
}

// Перегрузка оператора побитового ИЛИ.
TBitField TBitField::operator|(const TBitField& otherField) {
    int maxBitLen = std::max(BitLen, otherField.BitLen); // Находим максимальную длину
    TBitField result(maxBitLen); // Создаем новый объект

    int minMemLen = std::min(MemLen, otherField.MemLen); // Находим минимальное количество ячеек памяти
    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] | otherField.pMem[i]; // Устанавливаем биты из обоих полей
    }

    // Если одна из битовых полей больше, добавляем остаток
    if (MemLen > minMemLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            result.pMem[i] = pMem[i]; // Копируем оставшиеся биты
        }
    }
    else {
        for (int i = minMemLen; i < otherField.MemLen; i++) {
            result.pMem[i] = otherField.pMem[i]; // Копируем оставшиеся биты
        }
    }

    return result; // Возвращаем результат
}

// Перегрузка оператора побитового И.
TBitField TBitField::operator&(const TBitField& otherField) {
    int maxBitLen = std::max(BitLen, otherField.BitLen); // Находим максимальную длину
    TBitField result(maxBitLen); // Создаем новый объект

    int minMemLen = std::min(MemLen, otherField.MemLen); // Находим минимальное количество ячеек памяти
    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] & otherField.pMem[i]; // Устанавливаем пересечения битов
    }

    return result; // Возвращаем результат
}

// Перегрузка оператора побитового НЕ.
TBitField TBitField::operator~() {
    TBitField result(BitLen); // Создаем новый объект

    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i]; // Инвертируем биты
    }

    // Обработка последнего элемента, чтобы оставить только полезные биты
    int lastBits = BitLen % (sizeof(TELEM) * 8); // Определяем количество значимых бит в последнем элементе
    if (lastBits > 0) {
        TELEM mask = (1 << lastBits) - 1; // Создаем маску для значимых бит
        result.pMem[MemLen - 1] &= mask; // Применяем маску, чтобы сбросить лишние биты
    }

    return result; // Возвращаем результат
}

// Перегрузка оператора ввода.
std::istream& operator>>(std::istream& input, TBitField& bf) {
    char buffer[255];
    input.getline(buffer, sizeof(buffer)); // Читаем строку из входного потока
    return input; // Возвращаем поток
}

// Перегрузка оператора вывода.
std::ostream& operator<<(std::ostream& output, const TBitField& bf) {
    for (int i = bf.BitLen - 1; i >= 0; i--) {
        output << ((bf.pMem[bf.GetMemIndex(i)] & bf.GetMemMask(i)) ? '1' : '0'); // Печатаем биты в обратном порядке
    }
    return output; // Возвращаем поток
}
