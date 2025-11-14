#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <locale>
#include <windows.h>

using namespace std;

// Глобальные переменные
int coins;
int bob_coins = 0;
int tom_coins = 0;
mutex mtx;

void sharing(string thief_name) {
    while (coins > 0) {
        mtx.lock();

        if (coins > 0) {
            if (thief_name == "Боб") {
                if (bob_coins <= tom_coins) {
                    coins--;
                    bob_coins++;
                    cout << "Боб взял монету. У Боба: " << bob_coins
                        << ", у Тома: " << tom_coins
                        << ", осталось: " << coins << endl;
                }
            }
            else {
                if (tom_coins <= bob_coins) {
                    coins--;
                    tom_coins++;
                    cout << "Том взял монету. У Боба: " << bob_coins
                        << ", у Тома: " << tom_coins
                        << ", осталось: " << coins << endl;
                }
            }
        }

        mtx.unlock();
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

int main() {
    // Устанавливаем русскую локаль для консоли
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    // Запрос количества монет у пользователя
    cout << "=== Программа раздела монет ===" << endl;
    cout << "Введите количество монет для дележа: ";
    cin >> coins;

    int initial_coins = coins; // Сохраняем начальное количество

    cout << "\nНачальное количество монет: " << coins << endl;
    cout << "Начало дележа..." << endl << endl;

    // Создаем потоки для воров
    thread bob(sharing, "Боб");
    thread tom(sharing, "Том");

    // Ожидаем завершения потоков
    bob.join();
    tom.join();

    // Выводим итоговые результаты
    cout << endl << "=== Результаты дележа ===" << endl;
    cout << "Начальное количество: " << initial_coins << " монет" << endl;
    cout << "Боб получил: " << bob_coins << " монет" << endl;
    cout << "Том получил: " << tom_coins << " монет" << endl;
    cout << "Всего распределено: " << (bob_coins + tom_coins) << " монет" << endl;
    cout << "Разница: " << abs(bob_coins - tom_coins) << " монет" << endl;

    // Проверяем корректность распределения
    if (bob_coins + tom_coins == initial_coins) {
        cout << "Дележ прошел успешно!" << endl;
    }
    else {
        cout << "Ошибка в распределении монет!" << endl;
    }

    // Анализ справедливости распределения
    cout << endl << "=== Анализ распределения ===" << endl;
    if (bob_coins == tom_coins) {
        cout << "Монеты распределены поровну" << endl;
    }
    else if (abs(bob_coins - tom_coins) == 1) {
        cout << "Распределение максимально справедливое (разница 1 монета)" << endl;
    }
    else {
        cout << "Распределение неравномерное" << endl;
    }

    system("pause");
    return 0;
}