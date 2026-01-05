#include "TypeTable.h"

TypeTable::TypeTable()
    : type(__CLASS),
      classTableElement(nullptr),
      className(""),
      arraySize(0),
      toDescriptionNode(nullptr)
{
}

TypeTable::TypeTable(NodeDescription * description)
    : classTableElement(nullptr),
      className(""),
      arraySize(0),
      toDescriptionNode(description)
{
    if (!description) {
        type = __CLASS;
        return;
    }

    type = description->type;

    if (type == __ARRAY && description->complexNode) {
        arraySize = description->complexNode->id;
    }
}
