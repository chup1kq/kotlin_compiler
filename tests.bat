echo. > results.txt
echo test001 Объявление переменной >> results.txt
a.exe < test\test001.txt >> results.txt

echo. >> results.txt
echo test002 Объявление переменной с лишними отступами >> results.txt
a.exe < test\test002.txt >> results.txt

echo. >> results.txt
echo test003 Несколько переменных подряд >> results.txt
a.exe < test\test003.txt >> results.txt

echo. >> results.txt
echo test004 Арифметическое выражение >> results.txt
a.exe < test\test004.txt >> results.txt

echo. >> results.txt
echo test005 Использование переменной >> results.txt
a.exe < test\test005.txt >> results.txt

echo. >> results.txt
echo test006 Ошибка: нет точки с запятой >> results.txt
a.exe < test\test006.txt >> results.txt

echo. >> results.txt
echo test007 Пробелы и табы в разных местах >> results.txt
a.exe < test\test007.txt >> results.txt

echo. >> results.txt
echo test008 Строка с пробелами >> results.txt
a.exe < test\test008.txt >> results.txt

echo. >> results.txt
echo test009 Комментарий >> results.txt
a.exe < test\test009.txt >> results.txt

echo. >> results.txt
echo test010 Многострочный комментарий >> results.txt
a.exe < test\test010.txt >> results.txt

echo. >> results.txt
echo test011 Объявление переменной внутри комментария >> results.txt
a.exe < test\test011.txt >> results.txt