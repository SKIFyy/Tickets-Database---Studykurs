#include "Functions.h"


int consolmenu;
void Menu() {
    cout << "Выберите действие, которое вы хотели бы совершить с билетами: " << endl
         << "(0) Выход из программы" << endl
         << "(1) Ввод" << endl
         << "(2) Вывод, уже имеющихся" << endl
         << "(3) Изменение" << endl
         << "(4) Удаление" << endl
         << "(5) Добавление" << endl
         << "(6) Сортировка" << endl
         << "(7) Сохранение" << endl
         << "(8) Поиск" << endl
         << "(9) Вывод самых дорогих и дешевых" << endl
         << "(10) Вывод средней цены билетов" << endl
         << "Введите цифру: ";
    cin >> consolmenu;

    // проверка на ввод букв
    if (getchar() != '\n') {
        system("cls"); // очистка
        cin.clear(); // Сбрасываем флаг ошибки, если таковая была
        cin.ignore(1000, '\n'); // Игнорируем оставшиеся в потоке данные
        cout << "Нужно ввести цифру!" << endl;
        system("pause"); // задержка
        system("cls"); // очистка
        Menu();
    }
}

// убираем дублировнаие этих строк:
void Save(Data* d ,int amountOfData ,char* fileName){
    system("cls");// очистка

    int n;
    cout << "Вы хотите сохранить в текстовый документ - txt или в csv?(1 или 2): ";
    cin >> n;

    if (n == 1){
        cout << "Введите название файла, куда хотите сохранить: ";
        cin >> fileName;
        strcat(fileName, ".txt");
        if (amountOfData != 0) {
            SavingData(d, amountOfData, fileName);
        }else {
            cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
        }
    }else if(n == 2){
        cout << "Введите название файла, куда хотите сохранить: ";
        cin >> fileName;
        strcat(fileName, ".csv");
        if (amountOfData != 0) {
            SavingData(d, amountOfData, fileName);
        }else {
            cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
        }
    }else{
        cout << "Такого варианта выбора не существует!" << endl;
    }
}

int main() {
    // перевод на русский
    system("chcp 1251");
    setlocale(LC_ALL, "Russian");

    system("cls"); // очистка
    cout << "Добро пожаловать в базу данных билетов на поезда!" << endl;
    system("pause"); // задержка консоли

    system("cls"); // очистка
    cout << "Здесь вы можете совершить разные дайствия с уже имеющимися билетами, или добавить новые." << endl;
    system("pause"); // задержка консоли

    system("cls"); // очистка
    cout << "Хорошей работы!" << endl;
    system("pause"); // задержка консоли

    // инициализация данных
    int _actions, _actions1, amountOfData = 0, s = 0;
    char fileName[16] = "999.csv"; // значение по умолчанию 999.csv

    // массив данных
    Data* d = new Data[amountOfData];

    ReadingData(d, amountOfData, fileName); // по умолчанию занесем начальные данные
    system("cls"); // очистка
    cout << "Внесена база данных по умолчанию." << endl;
    Menu();

    while (consolmenu != -1) {
        switch (consolmenu) {
            case 0:
                system("cls"); //очистка консоли

                if (s != 0) {
                    cout << "Ваши данные не сохранены! Хотите их сохранить?(0-Выйти, 1-Сохранить):";
                    cin >> _actions;
                    if (_actions == 0) {
                        exit(0);
                    } else if (_actions == 1) {
                        Save(d, amountOfData, fileName);
                        s = 0; // обнуление флага изменения данных
                    } else {
                        cout << "Такого варианта выбора не существует!" << endl;
                    }
                }else{
                    exit(0);
                }

                system("pause"); // задержка консоли
                system("cls"); // снова очистка
                Menu();
            case 1:
                system("cls"); //очистка консоли
                cout << "Хотите ввести данные вручную или из файла? (Выберите 1 или 2): ";
                cin >> _actions;

                system("cls"); //очистка консоли

                if (_actions == 1) {
                    //ввод вручную
                    DataEntry(d, amountOfData);

                    s += 1; // флаг изменения данных
                } else if (_actions == 2) {
                    system("cls");// очистка

                    //ввод из файла
                    cout << "Вы хотите загрузить данные из текстового документа - txt или из csv?(1 или 2): ";
                    cin >> _actions;

                    system("cls");// очистка

                    if (_actions == 1) {

                        cout << "Введите название файла откуда хотите загрузить данные: ";
                        cin >> fileName;
                        strcat(fileName, ".txt");

                        ReadingData(d, amountOfData, fileName);
                    }else if(_actions == 2){

                        cout << "Введите название файла откуда хотите загрузить данные: ";
                        cin >> fileName;
                        strcat(fileName, ".csv");

                        ReadingData(d, amountOfData, fileName);
                    }else{
                        cout << "Такого варианта выбора не существует!" << endl;
                    }
                } else {
                    cout << "Такого варианта выбора не существует!" << endl;
                }

                system("pause"); // задержка консоли
                system("cls"); // снова очистка
                Menu();
                break;
            ////////////////////////////////////////////////////
            case 2:
                system("cls"); // очистка

                if (amountOfData != 0) {
                    Print(d, amountOfData);
                } else {
                    cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
                }
                system("pause"); // задержка
                system("cls"); // очистка
                Menu();
                break;
            ////////////////////////////////////////////////////
            case 3:
                system("cls"); // очистка

                Print(d, amountOfData);

                if (amountOfData != 0) {
                    DataChange(d, amountOfData);

                    Print(d, amountOfData);

                    s += 1; // флаг изменения данных
                } else {
                    cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
                }

                system("pause");
                system("cls");
                Menu();
                break;
            ////////////////////////////////////////////////////
            case 4:
                system("cls"); // снова очистка

                Print(d, amountOfData);

                if (amountOfData != 0) {
                    DeleteData(d, amountOfData);

                    s += 1; // флаг изменения данных
                } else {
                    cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
                }

                Print(d, amountOfData);

                system("pause"); // задержка
                system("cls"); // очистка
                Menu();
                break;
            ////////////////////////////////////////////////////
            case 5:
                system("cls"); // очистка

                Print(d, amountOfData);

                if (amountOfData != 0) {
                    AddData(d, amountOfData);
                    amountOfData++;

                    Print(d, amountOfData);

                    s += 1; // флаг изменения данных
                } else {
                    cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
                }

                system("pause"); // задержка
                system("cls"); // очистка
                Menu();
                break;
            ////////////////////////////////////////////////////
            case 6:
                system("cls");// очистка

                Print(d, amountOfData);

                if (amountOfData != 0) {
                    cout << "По какому столбцу вы бы хотели отсортировать билеты?(от 1 до 5): ";
                    cin >> _actions;


                    if (_actions >= 1 && _actions <= 5){

                        cout << "Вы бы хотели отсортировать по возрастанию или убыванию, от А до Я или от Я до А?(Выберите 1 или 2): ";
                        cin >> _actions1;

                        if (_actions1 == 1 || _actions1 == 2) {
                            DataSorting(d, amountOfData, _actions, _actions1);

                            Print(d, amountOfData);

                            s += 1; // флаг изменения данных
                        }else{
                            cout << "Такого варианта выбора не существует!" << endl;
                        }
                    }else{
                        cout << "Такого варианта выбора не существует!" << endl;
                    }
                } else {
                    cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
                }

                system("pause"); // задержка
                system("cls"); // очистка
                Menu();
                break;
            ////////////////////////////////////////////////////
            case 7:

                Save(d, amountOfData, fileName);
                s = 0; // обнуление флага изменения данных

                system("pause"); // задержка
                system("cls"); // очистка
                Menu();
                break;
            ////////////////////////////////////////////////////
            case 8:
                system("cls"); // очистка

                Print(d, amountOfData);

                if (amountOfData != 0) {
                    DataSearch(d, amountOfData);

                } else {
                    cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
                }

                system("pause"); // задержка
                system("cls"); // очистка
                Menu();
                break;
            ////////////////////////////////////////////////////
            case 9:
                system("cls"); // очистка

                Print(d, amountOfData);

                cout << "Вы хотите найти самый дорогой или самый дешевый билет?(Введите 1 или 2): ";
                cin >> _actions;
                if (amountOfData != 0) {
                    if (_actions == 1) {
                        FindMax(d, amountOfData);
                    } else if (_actions == 2) {
                        FindMin(d, amountOfData);
                    } else {
                        cout << "Такого варианта выбора не существует!" << endl;
                    }
                } else {
                    cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
                }

                system("pause"); // задержка
                system("cls"); // очистка
                Menu();
                break;
            ////////////////////////////////////////////////////
            case 10:
                system("cls"); // очистка

                Print(d, amountOfData);

                if (amountOfData != 0) {
                    Srprice(d, amountOfData);
                } else {
                    cout << "В данных ничего нет! Введите данные с помощью кнопки в меню." << endl;
                }
                system("pause"); // задержка
                system("cls"); // очистка
                Menu();
                break;
            ////////////////////////////////////////////////////
            default:
                system("cls"); // очистка
                cout << "Вы неверно ввели номер действия!" << endl;
                system("pause"); // задержка
                system("cls"); // очистка
                Menu();
                break;
        }
    }
}