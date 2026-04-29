#include "Functions.h"

void DataEntry(Data* (&d), int&n) {
    cout << "Введите кол-во билетов, данные которых хотите ввести: ";
    cin >> n;

    // выделяем память
    d = new Data[n];

    for (int i = 0; i < n; i++) {
        cout << "Введите номер поезда: ";
        cin >> d[i].number;

        cout << "Введите цену билета в рублях: ";
        cin >> d[i].price;

        cout << "Введите пункт отправления: ";
        cin >> d[i].start;

        //создание временной переменной
        char temp[32];

        cout << "Введите пункт прибытия: ";
        cin >> temp;

        while (strcmp(temp, d[i].start) == 0) {
            cout << "Пункт отправления и прибытия не могут совпадать!" << endl;

            cout << "Введите пункт прибытия: ";
            cin >> temp;
        }

        strcpy( d[i].end, temp);

        cout << "Введите тип места: ";
        cin >> d[i].seat;

        cout << "-----------------------------------" << endl;
    }
}
void ReadingData(Data* (&d), int& n, char* fileName) {

    FILE* file = fopen(fileName, "r"); // открываем файл

    if (file != nullptr) {

        n = 10;

        //выделяем память
        d = new Data[n];

        int i = 0;
        while (fscanf(file,"%d,%d,%[^,],%[^,],%s", &d[i].number, &d[i].price, d[i].start, d[i].end, d[i].seat) == 5) {
            i++;
            if (i == n){
                //создание временного массива
                Data* temp;
                temp = new Data[n];
                //копируем массив во временный массив
                Copy(temp, d, n);
                delete[] d;
                //удваиваем кол-во структур
                n *= 2;
                //создаем массив с удвоенным местом
                d = new Data[n];
                //Копируем из времнного массива в новый массив данные и оставляем остальное место пустым для послед. записи
                Copy(d, temp, n/2);
                delete[] temp; // удаляем временный массив
            }
        }
        n = i;
        if (n != 0) {
            cout << "Данные считаны из файла: " << fileName << " !" << endl;
        }else{
            cout << "Файл: " << fileName << " пустой, или в нем неверный формат данных!" << endl;
        }
    }else {
        cout << "Во время открытия файла произошла ошибка или файл был не найден!" << endl;
    }
    fclose(file);
}
void Print(Data* d, int n){

    cout << "---------------------------------------------------------------------------------------" << endl;
    cout.setf(ios::left);
    cout << "| " << "№" << "  |";
    cout.width(15);
    cout << "Номер поезда" << "|";
    cout.width(15);
    cout << "Цена билета(р.)" << "|";
    cout.width(17);
    cout << "Пункт отправления" << "|";
    cout.width(17);
    cout << "Пункт назначения" << "|";
    cout.width(12);
    cout << "Тип места" << "|";
    cout.unsetf(ios::left);
    cout << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < n; i++){

        cout.setf(ios::left);
        if (i+1 < 10) {
            cout << "| 0" << i + 1 << " |";
        }else{
            cout << "| " << i + 1 << " |";
        }
        cout.width(15);
        cout << d[i].number << "|";
        cout.width(15);
        cout << d[i].price << "|";
        cout.width(17);
        cout << d[i].start << "|";
        cout.width(17);
        cout << d[i].end << "|";
        cout.width(12);
        cout << d[i].seat << "|";
        cout.unsetf(ios::left);
        cout << endl;
        cout << "---------------------------------------------------------------------------------------" << endl;
    }
}
void DataChange(Data* (&d), int n) {

    int _n;
    cout << "Введите номер билета, который хотите изменить (от 1 до " << n << "): ";
    cin >> _n;
    _n--;

    //проверка правильного ввода
    if (_n >= 0 && _n < n) {

        //создаем временный массив
        Data* temp;
        temp = new Data[1];

        //копируем билет для дальнейшего его отображения пользователю
        Copy(temp[0], d[_n]);

        system("cls"); // чистим консоль
        Print(temp, 1);

        int _actions;
        cout << "Вы бы хотели полностью перезаписать билет или изменить только 1 поле?(Выберите 1 или 2): ";
        cin >> _actions;

        if (_actions == 1){
            cout << "Введите номер поезда: ";
            cin >> d[_n].number;

            cout << "Введите цену билета в рублях: ";
            cin >> d[_n].price;

            cout << "Введите пункт отправления: ";
            cin >> d[_n].start;

            cout << "Введите пункт прибытия: ";
            cin >> d[_n].end;

            cout << "Введите тип места: ";
            cin >> d[_n].seat;

            system("cls"); // чистим консоль

            cout << "Данные изменены!" << endl;
        }else if (_actions == 2){
            cout << "Какое поле вы бы хотели изменить?(от 1 до 5)" << endl
                 << "(1) Номер поезда" << endl
                 << "(2) Цена билета" << endl
                 << "(3) Пункт отправления" << endl
                 << "(4) Пункт назначения" << endl
                 << "(5) Тип места" << endl
                 << "Введите цифру: ";
            cin >> _actions;


            if (_actions == 1) {

                cout << "Введите новый номер поезда: ";
                cin >> d[_n].number;

                system("cls"); // чистим консоль

                cout << "Данные изменены!" << endl;
            }else if (_actions == 2) {
                cout << "Введите новую цену билета в рублях: ";
                cin >> d[_n].price;

                system("cls"); // чистим консоль

                cout << "Данные изменены!" << endl;
            }else if (_actions == 3) {

                //создание временной переменной
                char temp[32];

                cout << "Введите новый пункт отправления: ";
                cin >> temp;

                system("cls"); // чистим консоль

                while (strcmp(temp, d[_n].end) == 0) {
                    cout << "Пункт отправления и прибытия не могут совпадать!" << endl;

                    cout << "Введите новый пункт отправления: ";
                    cin >> temp;
                }
                strcpy( d[_n].start, temp);

                system("cls");

                cout << "Данные изменены!" << endl;

            }else if (_actions == 4) {

                //создание временной переменной
                char temp[32];

                cout << "Введите новый пункт прибытия: ";
                cin >> temp;

                system("cls"); // чистим консоль

                while (strcmp(temp,d[_n].start) == 0){
                    cout << "Пункт отправления и прибытия не могут совпадать!" << endl;

                    cout << "Введите новый пункт прибытия: ";
                    cin >> temp;
                }
                strcpy( d[_n].end, temp);

                system("cls");

                cout << "Данные изменены!" << endl;

            }else if (_actions == 5) {
                cout << "Введите новый тип места: ";
                cin >> d[_n].seat;

                system("cls"); // чистим консоль

                cout << "Данные изменены!" << endl;
            }else{
                cout << "Вы неверно ввели номер поля!" << endl;
            }
        }else{
            cout << "Вы неверно ввели номер действия!" << endl;
        }

        delete[]temp;//удаляем временный массив

    }else {
        cout << "Вы неверно ввели номер элемента!" << endl;
    }
}
void DeleteData(Data* (&d), int& n) {

    int _n;
    cout << "Введите номер элемента (от 1 до " << n << "): ";
    cin >> _n;
    _n--;

    system("cls");

    // проверка правильности ввода
    if (_n >= 0 && _n < n) {

        //создание временного массива
        Data* temp = new Data[n];

        Copy(temp, d, n);

        //выделим новую память
        --n;
        d = new Data[n];

        int q = 0;

        //запомним данные кроме не нужного
        for (int i = 0; i <= n; i++) {
            if (i != _n) {
                d[q] = temp[i];
                ++q;
            }
        }

        system("cls"); // очистка
        delete[]temp;
        cout << "Данные о элементе удалены!" << endl;
    }
    else {
        cout << "Вы неверно ввели номер!" << endl;
    }
}
void Copy(Data* (&d_n), Data* (&d_o), int n) {

    for (int i = 0; i < n; i++) {
        d_n[i] = d_o[i];
    }
}
void Copy(Data& d_n, Data& d_o){

    d_n.number = d_o.number;
    d_n.price = d_o.price;
    strncpy(d_n.start, d_o.start, 32);
    strncpy(d_n.end, d_o.end, 32);
    strncpy(d_n.seat, d_o.seat, 32);

}
void AddData(Data* (&d), int& n) {

    //создаем врменный массив
    Data* temp;
    temp = new Data[n];

    //копируем данные во временный массив
    Copy(temp, d, n);

    //выделяем новую память
    n++;
    d = new Data[n];

    //вернем данные
    Copy(d, temp, --n);

    cout << "Введите номер поезда: ";
    cin >> d[n].number;

    cout << "Введите цену билета в рублях: ";
    cin >> d[n].price;

    cout << "Введите пункт отправления: ";
    cin >> d[n].start;

    //создание временной переменной
    char _temp[32];

    cout << "Введите пункт прибытия: ";
    cin >> _temp;

    while (strcmp(_temp, d[n].start) == 0) {
        cout << "Пункт отправления и прибытия не могут совпадать!" << endl;

        cout << "Введите пункт прибытия: ";
        cin >> _temp;
    }

    strcpy( d[n].end, _temp);

    cout << "Введите тип места: ";
    cin >> d[n].seat;

    system("cls");
    cout << "Данные были добавлены!" << endl;

    delete[]temp;
}
int sortAA(const void* n1, const void* n2){
    return strcmp(((Data*)n1)->start, ((Data*)n2)->start);
}
int sortAB(const void* n1, const void* n2){
    return strcmp(((Data*)n2)->start, ((Data*)n1)->start);
}
int sortBA(const void* n1, const void* n2){
    return strcmp(((Data*)n1)->end, ((Data*)n2)->end);
}
int sortBB(const void* n1, const void* n2){
    return strcmp(((Data*)n2)->end, ((Data*)n1)->end);
}
int sortCA(const void* n1, const void* n2){
    return strcmp(((Data*)n1)->seat, ((Data*)n2)->seat);
}
int sortCB(const void* n1, const void* n2){
    return strcmp(((Data*)n2)->seat, ((Data*)n1)->seat);
}
void DataSorting(Data* d, int n, int n1, int n2) {

    //временная переменная
    Data temp;

    if (n1 == 1) {
            //сортировка ( метод пузыря )
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (n2 == 1) {
                        //условие сортировки
                        if (d[i].number > d[j].number) {
                            Copy(temp, d[j]);
                            Copy(d[j], d[i]);
                            Copy(d[i], temp);
                        }
                    } else {
                        //условие сортировки
                        if (d[i].number < d[j].number) {
                            Copy(temp, d[j]);
                            Copy(d[j], d[i]);
                            Copy(d[i], temp);
                        }
                    }
                }
            }
    } else if (n1 == 2) {
            //сортировка ( метод пузыря )
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (n2 == 1) {
                        //условие сортировки
                        if (d[i].price > d[j].price) {
                            Copy(temp, d[j]);
                            Copy(d[j], d[i]);
                            Copy(d[i], temp);
                        }
                    } else {
                        //условие сортировки
                        if (d[i].price < d[j].price) {
                            Copy(temp, d[j]);
                            Copy(d[j], d[i]);
                            Copy(d[i], temp);
                        }
                    }
                }
            }
    } else if (n1 == 3) {

        if (n2 == 1) {
            qsort(d, n, sizeof(d[0]), sortAA);
        }else{
            qsort(d, n, sizeof(d[0]), sortAB);
        }

    } else if (n1 == 4) {

        if (n2 == 1) {
            qsort(d, n, sizeof(d[0]), sortBA);
        }else{
            qsort(d, n, sizeof(d[0]), sortBB);
        }

    } else if (n1 == 5) {

        if (n2 == 1) {
            qsort(d, n, sizeof(d[0]), sortCA);
        }else{
            qsort(d, n, sizeof(d[0]), sortCB);
        }

    }
    cout << "Данные отсортированы!" << endl;

}
void SavingData(Data* d, int n, char* fileName) {

    //создаем поток для записи
    ofstream record(fileName, ios::out); // открывает и очищает файл

    if(record){

        for (int i = 0; i < n; i++) {
            record << d[i].number << ",";
            record << d[i].price << ",";
            record << d[i].start << ",";
            record << d[i].end << ",";
            if (i < n - 1) {
                record << d[i].seat << endl;
            }
            else {
                record << d[i].seat;
            }
        }
        cout << "Данные сохранены в файл " << fileName << endl;
    }
    else {
        cout << "Во время открытия файла произошла ошибка!" << endl;
    }

    record.close();
}
void DataSearch(Data* d, int n) {

    //создаем временный массив
    Data* temp;
    temp = new Data[n];

    int s = 0;
    char punkt[32];
    cout << "Введите пункт, по которому хотите найти билеты: ";
    cin >> punkt;

    for (int i = 0; i < n; i++){
        if ((strcmpi(punkt, d[i].start) == 0) || (strcmpi(punkt, d[i].end) == 0 )) {
            temp[s] = d[i];
            s++;
        }
    }
    if (s != 0) {
        cout << "Нашлось " << s << " совпадения(-й):" << endl;
        Print(temp, s);
    }else{
        cout << "Совпадений не найдено!" << endl;
        return;
    }
    delete[]temp;
}
void FindMax(Data* d, int n) {

    //создаем временный массив
    Data* temp;
    temp = new Data[n];

    Copy(temp, d, n);

    // сортируем временный массив от большего к меньшему
    DataSorting(temp, n, 2, 2);
    system("cls"); // очистка

    cout << "Самый дорогой билет:" << endl;
    // выводим самое первое значение то есть самое большое
    Print(temp, 1);
    delete[]temp;
}
void FindMin(Data* d, int n) {

    //создаем временный массив
    Data* temp;
    temp = new Data[n];

    Copy(temp, d, n);

    // сортируем временный массив от меньшего к большему
    DataSorting(temp, n, 2, 1);
    system("cls"); // очистка

    cout << "Самый дешевый билет:" << endl;
    // выводим самое первое значение то есть самое меньшее
    Print(temp, 1);
    delete[]temp;
}
void Srprice(Data* d, int n){

    int s = 0;
    //найдем сумму всех цен
    for (int i = 0; i < n; i++){
        s += d[i].price;
    }
    int m = (s*100)/n;
    cout << "Средняя цена равна: " << m/100 << "," << m/ 10 % 10 << m%10 << "рублей" << endl;
}