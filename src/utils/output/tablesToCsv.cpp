#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

#include "../../semantic/tables/constant/ConstantTable.h"
#include "../../semantic/tables/constant/ConstantTableElement.h"
#include "../../semantic/tables/class/ClassTable.h"


void constantTableToCsv(ConstantTable* constantTable, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return;
    }

    out << "id,type,utf8String,intValue,doubleValue,firstRef,secondRef\n";

    for (const auto& pair : constantTable->items) {
        ConstantTableElement* el = pair.second;
        if (!el) continue;

        out << el->id << ",";
        out << el->type << ",";

        if (!el->utf8String.empty()) {
            out << "\"" << el->utf8String << "\"";
        }
        out << ",";

        out << el->intValue << ",";
        out << el->doubleValue << ",";
        out << el->firstRef << ",";
        out << el->secondRef << "\n";
    }

    out.close();
    std::cout << "Constant table saved to " << filename << "\n";
}

// rootDir — имя директории, которую нужно создать / пересоздать
void classTableToCSV(ClassTable* classTable, const std::string& rootDir) {

    // Удаляем директорию, если существует
    try {
        if (fs::exists(rootDir)) {
            fs::remove_all(rootDir);
        }
        fs::create_directories(rootDir);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
        return;
    }

    // Перебираем все классы
    for (const auto& [classId, classElem] : classTable->items) {

        if (!classElem || !classElem->constants) {
            continue;
        }

        // Директория класса
        fs::path classDir = fs::path(rootDir) / classId;

        try {
            fs::create_directories(classDir);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Cannot create dir " << classDir
                      << ": " << e.what() << "\n";
            continue;
        }

        // Файл constants.csv
        fs::path csvPath = classDir / "constants.csv";

        // Используем уже существующий метод
        constantTableToCsv(classElem->constants, csvPath.string());
    }

    std::cout << "All constant tables dumped to directory: " << rootDir << "\n";
}

void methodTableToCSV(MethodTable* methodTable, const std::string& rootDir) {

}