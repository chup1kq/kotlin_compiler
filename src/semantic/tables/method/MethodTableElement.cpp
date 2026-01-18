#include "MethodTableElement.h"

MethodTableElement::MethodTableElement(
    int nameConst,
    int descConst,
    std::string name,
    std::string desc
)
    : methodName(nameConst),
      descriptor(descConst),
      strName(std::move(name)),
      strDesc(std::move(desc)),
      start(nullptr),
      retType(nullptr),
      localVarTable(new LocalVariableTable()),
      isFirst(0)
{
}

MethodTableElement::MethodTableElement(int methodName, int descriptorName, std::string strName, std::string strDesc,
    StmtListNode *start, SemanticType *retType, vector<FuncParam*> params) {
    this->methodName = methodName;
    this->descriptor = descriptorName;
    this->strName = std::move(strName);
    this->strDesc = std::move(strDesc);
    this->start = start;
    this->retType = retType;
    this->localVarTable = new LocalVariableTable();
    this->params = std::move(params);
}

std::string MethodTableElement::getParamsFromDescriptor() {
    const std::string& desc = strDesc;
    if (desc.empty() || desc[0] != '(') {
        return "";
    }

    size_t closeParenPos = desc.find(')', 1);  // Ищем ) после (
    if (closeParenPos == std::string::npos) {
        return "";  // Нет закрывающей скобки
    }

    return desc.substr(0, closeParenPos + 1);  // "(params)"
}
