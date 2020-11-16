#include <vector>
#include <iostream>
#include <pthread.h>

// Куприхин Дима, БПИ199, Вариант 14
// Задание: определить такие индексы, для которых a[i] и b[i] не имееют общих
// делителей больших единицы, т.е. НОД(a[i], b[i]) = 1.
// Входные параметры: размер массивов (>=1000), количество потоков, 
// элементы массивов.

// Для вычисления НОДa будем использовать алгоритм Евклида. Результаты будем
// сохранять в вектор, в котором значение элемента 1 значит 
// взаимнопростость чисел с этим индексом, а 0 - НОД > 1. Каждый поток 
// записывает и читает из своего отдельной области, поэтому нам не надо 
// использовать мьютексы.

// Класс, хранящий ссылки на два массива, значения начала и конца 
// обрабатываемого отрезка [start; end), ссылку на вектор с результатом 
// программы. Для каждого потока сконструирован специальный экземпляр блока с 
// соответствующими значениями концов обрабатываемого интервала.
class ArraysBlock {
 public:
    ArraysBlock(std::vector<int>& first, std::vector<int>& second, int start,
                int end, std::vector<int>& numbers) : 
            firstArray(first),
            secondArray(second),
            start(start),
            end(end),
            coprimeNumbers(numbers) {}

    std::vector<int>& firstArray;
    std::vector<int>& secondArray;
    int start;
    int end;
    std::vector<int>& coprimeNumbers;
};

void inputIntVector(std::vector<int>& vector) {
    std::cout << "Input " << vector.size() << " elements of vector" << std::endl;
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

// Функция, которую выполняют потоки. Каждый поток обрабатывает свой отдельный
// участок массивов и записывает результат в общий вектор.
void* countCoprime(void* args) {
    ArraysBlock* block = (ArraysBlock*)args;
    // Проходимся по всем индексам этого блока.
    for(int i = block->start; i < block->end; ++i){
        int g = gcd(block->firstArray[i], block->secondArray[i]);
        if(g == 1){
            block->coprimeNumbers[i] = 1;
        }
    }
    return 0;
}

int main() {
    
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    // Вводим размеры массивов и количество потоков.
    int arraySize, threadsNumber;
    std::cout << "Input array size and number of threads" << std::endl;
    std::cin >> arraySize >> threadsNumber;

    // Инициализируем и заполняем массивы.
    // потоков и результата.
    std::vector<int> firstArray(arraySize);
    std::vector<int> secondArray(arraySize);
    inputIntVector(firstArray);
    inputIntVector(secondArray);
    std::vector<int> result(arraySize, 0);
    std::vector<pthread_t> threads(threadsNumber);

    // Инициализирум блоки. Каждому блоку выделяем равные(с точностью 
    // целочисленного деления) куски, которые будет обрабатывать 
    // соответствующий поток.
    std::vector<ArraysBlock*> blocks(threadsNumber);
    for(int i = 0; i < threadsNumber; ++i){
        blocks[i] = new ArraysBlock(firstArray, secondArray, 
                        arraySize / threadsNumber * i,
                        arraySize / threadsNumber * (i + 1), result);
    }

    // Запускаем потоки и ждем их завершения.
    for(int i = 0; i < threadsNumber; ++i){
        pthread_create(&threads[i], NULL, countCoprime, blocks[i]);
    }
    for(int i = 0; i < threadsNumber; ++i){
        pthread_join(threads[i], NULL);
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
            " coprime numbers in the arrays:\n";
    for(int i = 0; i < result.size(); ++i) {
        if(result[i]) {
            std::cout << i << " ";
        }
    }

    // Освобождаем выделенную в куче память.
    for(int i = 0; i < blocks.size(); ++i)
        delete blocks[i];

    return 0;
}
