#include "ConstantTable.h"

bool doubleEquals(double a, double b);

ConstantTable::ConstantTable() {
    this->items = std::map<int, ConstantTableElement*>();
    this->items[maxId] = new ConstantTableElement(maxId, UTF8, "Code");
    maxId++;
}

int ConstantTable::findOrAddConstant(
    ConstantType type,
    std::string utf8String,
    int intValue,
    double doubleValue,
    int firstRef,
    int secondRef
) {
    // Сначала ищем существующую константу
    int foundId = findConstant(type, utf8String, intValue, doubleValue, firstRef, secondRef);

    if (foundId != -1) {
        return foundId; // Константа уже существует
    }

    // Создаем новую константу
    ConstantTableElement* newElement = new ConstantTableElement(
        maxId,
        type,
        utf8String,
        intValue,
        doubleValue,
        firstRef,
        secondRef
    );

    // Добавляем в таблицу
    items[maxId] = newElement;

    // Возвращаем номер и увеличиваем счетчик
    return maxId++;
}

int ConstantTable::findConstant(
    ConstantType type,
    std::string utf8String,
    int intValue,
    double doubleValue,
    int firstRef,
    int secondRef
) {
    for (const auto& pair : items) {
        int id = pair.first;
        ConstantTableElement* element = pair.second;

        // Если тип константы совпадает
        if (type == element->type) {
            switch (type) {
                case ConstantType::UTF8:
                    // Для UTF-8 сравниваем строки
                    if (utf8String == element->utf8String) {
                        return id;
                    }
                    break;

                case ConstantType::Class:
                    // Для Class сравниваем первую ссылку
                    if (firstRef == element->firstRef) {
                        return id;
                    }
                    break;

                case ConstantType::String:
                    // Для String сравниваем первую ссылку
                    if (firstRef == element->firstRef) {
                        return id;
                    }
                    break;

                case ConstantType::MethodRef:
                    // Для MethodRef сравниваем обе ссылки
                    if (firstRef == element->firstRef && secondRef == element->secondRef) {
                        return id;
                    }
                    break;

                case ConstantType::NameAndType:
                    // Для NameAndType сравниваем обе ссылки
                    if (firstRef == element->firstRef && secondRef == element->secondRef) {
                        return id;
                    }
                    break;

                case ConstantType::FieldRef:
                    // Для FieldRef сравниваем обе ссылки
                    if (firstRef == element->firstRef && secondRef == element->secondRef) {
                        return id;
                    }
                    break;

                case ConstantType::Integer:
                    // Для Integer сравниваем целочисленное значение
                    if (intValue == element->intValue) {
                        return id;
                    }
                    break;

                case ConstantType::Double:
                    // Для Double сравниваем вещественное значение
                    // Внимание: прямое сравнение double может быть неточным
                    // В реальном коде лучше использовать epsilon сравнение
                    if (doubleEquals(doubleValue, element->doubleValue)) {
                        return id;
                    }
                    break;
            }
        }
    }

    // Константа не найдена
    return -1;
}