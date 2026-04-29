#pragma once

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <clocale>
#include <cstdlib>

using namespace std;

//структура данных
struct Data {
    unsigned int number, price; //номер поезда, цена билета
    char start[32]; // точка отправления
    char end[32]; // точка прибытия
    char seat[32]; // тип места
};


// функции
void DataEntry(Data* (&d), int&n); //Ввод данных вручную
void ReadingData(Data* (&d), int& n, char* fileName);//чтение данных из файла
void Print(Data* d, int n); //вывод данных
void DataChange(Data* (&d), int n);//изменение данных
void DeleteData(Data* (&d), int& n);//удаление данных
void Copy(Data* (&d_n), Data* (&d_o), int n); //копия данных
void Copy(Data& d_n, Data& d_o); //копия данных элемента
void AddData(Data* (&d), int& n); //добавить данные
void DataSorting(Data* d, int n, int n1, int n2);//сортировка данных
int sortAA(const void* n1, const void* n2);//доп проверка
int sortAB(const void* n1, const void* n2);//доп проверка
int sortBA(const void* n1, const void* n2);//доп проверка
int sortBB(const void* n1, const void* n2);//доп проверка
int sortCA(const void* n1, const void* n2);//доп проверка
int sortCB(const void* n1, const void* n2);//доп проверка
void SavingData(Data* d, int n, char* fileName);//сохранение данных
void DataSearch(Data* d, int n);
void FindMax(Data* d, int n);
void FindMin(Data* d, int n);
void Srprice(Data* d, int n);
