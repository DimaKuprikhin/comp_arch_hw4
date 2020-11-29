#include <vector>
#include <iostream>
#include <omp.h>

// Куприхин Дима, БПИ199, Вариант 14
// Задание: определить такие индексы, для которых a[i] и b[i] не имееют общих
// делителей больших единицы, т.е. НОД(a[i], b[i]) = 1.
// Входные параметры: размер массивов (>=1000), количество потоков, 
// элементы массивов.

// Для вычисления НОДa будем использовать алгоритм Евклида. Результаты будем
// сохранять в вектор, в котором значение элемента 1 значит 
// взаимнопростость чисел с этим индексом, а 0 - НОД > 1.

void inputIntVector(std::vector<int>& vector) {
    for(int i = 0; i < vector.size(); ++i){
        std::cin >> vector[i];
    }
}

// Алгоритм Евклида для поиска НОД'a двух чисел.
int gcd(int a, int b){
    if(a == 0)
        return b;
    return gcd(b % a, a);
}

int main() {
    
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    // Вводим размеры массивов и количество потоков.
    int arraySize, threadsNumber;
    // Утанавливаем false, если не хотим выводить значения индексов 
    // взаимнопростых чисел.
    bool isIndexesOutputEnable = true;
    std::cin >> arraySize >> threadsNumber;
    std::cin >> isIndexesOutputEnable;

    // Инициализируем и заполняем массивы.
    // потоков и результата.
    std::vector<int> firstArray(arraySize);
    std::vector<int> secondArray(arraySize);
    inputIntVector(firstArray);
    inputIntVector(secondArray);
    std::vector<char> result(arraySize, 0);
    
    // Массив, хранящий номера потоков, обработовших итерации цикла.
    std::vector<char> threadIndex(arraySize, -1);

    // Устанавливаем количество потоков из входных данных.
    omp_set_num_threads(threadsNumber);

    // Выполняем параллельлное вычисление НОД'ов чисел массивов.
    #pragma omp parallel for
    for(int i = 0; i < arraySize; ++i) {
        // Добавляем к текущему потоку одну итерацию.
        threadIndex[i] = omp_get_thread_num();
        if(gcd(firstArray[i], secondArray[i]) == 1) {
            result[i] = 1;
        }
    }
    

    // Массив, хранящий количество проихведенных итераций каждым потоком.
    std::vector<int> threadIterations(threadsNumber, 0);
    // Считаем количество итераций для каждого потока.
    for(int i = 0; i < arraySize; ++i) {
        ++threadIterations[threadIndex[i]];
    }
    // Выводим количество итераций каждого потока.
    for(int i = 0; i < threadIterations.size(); ++i) {
        std::cout << "Thread " << i << " processed " << threadIterations[i] <<
                    " numbers" << std::endl;
    }

    // Считаем количество взаимнопростых чисел.
    int resultSize = 0;
    for(int i = 0; i < result.size(); ++i) {
        if(result[i]) {
            ++resultSize;
        }
    }
    // Выводим результат.
    std::cout << "There are " << resultSize << 
            " coprime numbers in the arrays" << std::endl;
    if(isIndexesOutputEnable) {
        for(int i = 0; i < result.size(); ++i) {
            if(result[i]) {
                std::cout << i << " ";
            }
        }
    }

    return 0;
}
