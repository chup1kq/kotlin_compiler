#include <iostream>
#include <fstream>

#include "../../semantic/tables/constant/ConstantTable.h"
#include "../../semantic/tables/constant/ConstantTableElement.h"

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
