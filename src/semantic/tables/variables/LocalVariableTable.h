#ifndef KOTLIN_COMPILER_LOCALVARIABLETABLE_H
#define KOTLIN_COMPILER_LOCALVARIABLETABLE_H

class LocalVariableTable {
public:
    int maxId = 0;
    std::map<std::string, LocalVariableTable*> items;
    int findOrAddLocalVar(std::string name,
        TypeTable * type,
        int isConst,
        int isInit);
};

#endif //KOTLIN_COMPILER_LOCALVARIABLETABLE_H