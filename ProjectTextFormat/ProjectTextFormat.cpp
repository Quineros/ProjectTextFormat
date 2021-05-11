//Программа форматирует абзац текста по ширине. Окончание абзаца определяется следующими друг за другом знаками точки и переноса строки.
//Задаваемая ширина строки должна быть не меньше длины самого длинного слова в абзаце
//Последняя строка абзаца выравнивается по ширине

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>

using namespace std;
const int MaxWordLength = 256;
int firstlinespace = 0;


struct Word { //Структура, содержащая в себе символы слова и его длину
    int len;
    char letters[MaxWordLength];
};

vector <Word> FileRead() { //Функция чтения данных из файла
    vector <Word> invector;
    ifstream infile("input.txt");

    Word currword;
    currword.len = 0;
    char currletter;

    infile.unsetf(ios::skipws); //Отключение игнорирования разделительных символов при чтении из файла
    infile >> currletter;
    while (currletter == ' ') {  //Чтение красной строки
        firstlinespace++;
        infile >> currletter;
    }
    infile.close();
    infile.open("input.txt");
    infile.setf(ios::skipws); //Включение игнорирования разделительных символов

    while (infile) { //Чтение слов из файла
        infile >> currword.letters;
        currword.len = strlen(currword.letters);
        invector.push_back(currword); //Запись слов в вектор
    }
    infile.close();
    return invector;
};


void ConsoleToFile() { //Функция записи входных данных из консоли в файл
    ofstream outfile("input.txt");
    cin.unsetf(ios::skipws); //Отключение игнорирования разделительных символов при чтении из консоли
    char currletter = ' ';
    bool lastletterisapoint = 0;
    while ((!lastletterisapoint) or (currletter != '\n')) { //Посимвольная запись текста из консоли в файл до момента, когда друг за другом следуют точка и перенос строки
        lastletterisapoint = (currletter == '.');
        cin >> currletter;
        outfile << currletter;
    }
    outfile.close();
};


void FormatText(vector <Word> wordvector, int textwidth) { //Функция выравнивания абзаца по ширине
    ofstream resfile("output.txt");
    int linespace = 0, extraspace = 0, linewordcount = 0, linecount = 0;
    int currlinelen = firstlinespace;
    bool lastline = 1;
    for (int i = 0; i < firstlinespace; i++) resfile << ' ';
    wordvector.erase(wordvector.begin() + wordvector.size() - 1);
    while (wordvector.size() > 0) {
        lastline = 1;
        for (int i = 0; i < wordvector.size(); i++) { //Получение максимального количество слов, помещающихся на 1 строке
            if (currlinelen + wordvector[i].len + i > textwidth) {
                linewordcount = i;
                lastline = 0;
                break;
            }
            else {
                currlinelen += wordvector[i].len;
                linewordcount = i + 1;
            }
        }
        if (linewordcount == 0) linewordcount = wordvector.size();
        if (lastline) {                                              //Вывод последней строки, выровненной по левому краю
            for (int j = 0; j < wordvector.size() - 1; j++) {
                resfile << wordvector[j].letters << ' ';
            }
            resfile << wordvector[wordvector.size() - 1].letters << '\n';
            wordvector.erase(wordvector.begin(), wordvector.begin() + linewordcount);
        }
        else {
            if (linewordcount > 1) { //Получение количества пробелов между словами, необходимых для выравнивания строки
                linespace = (textwidth - currlinelen) / (linewordcount - 1);
                extraspace = (textwidth - currlinelen) % (linewordcount - 1);
            }
            else linespace = textwidth - currlinelen;
            for (int j = 0; j < (linewordcount - 1); j++) { //Запись строки, за исключением последнего слова, в файл
                resfile << wordvector[j].letters;
                for (int k = 0; k < linespace; k++) resfile << ' ';
                if (extraspace > 0) {
                    resfile << ' ';
                    extraspace--;
                }
            }
            if (linewordcount > 0) resfile << wordvector[linewordcount - 1].letters << '\n'; //Запись последнего слова строки
            wordvector.erase(wordvector.begin(), wordvector.begin() + linewordcount);
        }
        linewordcount = 0;
        currlinelen = 0;
        linespace = 0;
        extraspace = 0;
        linecount++;

    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    vector <Word> wordsvector;
    char inputformat = ' ';
    int linewidth;
    cout << "Выберите формат входных данных:" << endl;
    cout << "c - Ввод через консоль" << endl;
    cout << "f - Чтение данных из файла input.txt" << endl << endl;

    while ((inputformat != 'c') and (inputformat != 'f')) { //Выбор формата ввода текста
        cin >> inputformat;
        switch (inputformat) {
        case 'c':
            ConsoleToFile();
            wordsvector = FileRead();
            break;
        case 'f':
            wordsvector = FileRead();
            break;
        default:
            cout << endl << "Неопознанная команда, повторите ввод" << endl;
            break;
        }
    }

    cout << endl << "Введите ширину отформатированного текста:" << endl << endl;
    cin >> linewidth;
    FormatText(wordsvector, linewidth);
    cout << endl << "Результат записан в файл output.txt" << endl;
}