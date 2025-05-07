@echo off
rem Скрипт сборки проекта с помощью CMake

echo Создание директории сборки...
if not exist build mkdir build

echo Настройка проекта CMake...
cmake -S . -B build

echo Сборка проекта...
cmake --build build --config Debug -j4

echo Копирование исполняемых файлов...
if not exist bin mkdir bin
xcopy buildbin.exe bin Y

echo Сборка завершена!
echo Исполняемые файлы находятся в директории bin
