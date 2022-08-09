#include <iostream>
#include "stdio.h"
#include "malloc.h"
#include "locale.h"
#include "time.h"
#include <vector>

#define INF 1000000000
#define size 10
using namespace std;
void output(int count, int* result, int apex, int mode)
{
    int i;
    if (mode == 0)
        printf("Дейкстра:\n");
    else
        printf("Форд-Баллман:\n");

    for (i = 0; i < count; i++)
    {
        //если путь есть
        if (result[i] < 10000)
            printf("Минимальное расстояние между вершинами %d и %d = %d\n", apex, i, result[i]);
        else
            printf("Минимальное расстояние между вершинами %d и %d = INF\n", apex, i);
    }
}

void generation()

{
    srand((unsigned)time(NULL)); //устанавливаем начальную точку генерации
                                    //случайных чисел в зависимости от времени,
                                //необходимо, чтобы числа каждый раз были разные

    
    FILE* pFile;
    
    int** buffer;
    int i=0,j=0;
    buffer = (int**)malloc(size*size * sizeof(int));
    
    for (int i = 0; i < size; i++) {
        buffer[i] = (int*)malloc(size * sizeof(int));
    }

    pFile = fopen("myfile.bin", "wb");
    
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
        {
            if (i == j)
                buffer[i][j] = 0;
            else 
            {
                buffer[i][j] = rand() % 100; //Берём остаток от деления случайно
                buffer[j][i] = buffer[i][j];  //Cгенерированного числа на 2 (0 или 1)
            }
        }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            cout << buffer[i][j] << " ";
        cout << endl;
    }
    fprintf(pFile, "%d\n", 10);
    fwrite(buffer, 1, sizeof(buffer), pFile);
    fclose(pFile);
    cout << "\nGeneration ready" << endl;

   
    for (int i = 0; i < size; i++) {
        free(buffer[i]);
    }
    free(buffer);
}

void Ford_Ballman(int CountApex, int** SourceMatrix, int Start)
{

    struct Edges {
        int u,v,w; //v|u - смежные вершины, w - вес
    };

    int i, j, start,e=size-1;
    int d[size];
    Edges edge[size];
    d[Start] = 0;
    for (i = 0; i < size; ++i)
        d[i] = INF;
    d[Start] = 0;

    for (i = 0; i < size - 1; i++)
        for (j = 0; j < e; j++)
            if (d[edge[j].u] + edge[j].w < d[edge[j].v]) //если сумма, составленная из веса ребра w(v, u) и веса хранящегося в элементе d[v]
                d[edge[j].v] = d[edge[j].u] + edge[j].w; //меньше веса d[u], то она присваивается последнему.

    for (i = 0; i < size; i++) if (d[i] == INF)
        cout << endl << Start << "->" << i + 1 << "=" << "Not";
    else cout << endl << Start << "->" << i + 1 << "=" << d[i];


    cout << "fssfsffs";


    //output(size, d, Start, 1);
}



void deikstra(int CountApex, int** SourceMatrix, int Start)
{
    int st = 0;
    vector<vector<int >> w;
    w.resize(size);
    for (int i = 0; i < size; i++)
        w[i].resize(size);

    bool visited[size];
    int D[size];
    for (int i = 0; i < size; i++)
    {
        D[i] = w[st][i];
        visited[i] = false;
    }
    D[st] = 0;
    int index = 0, u = 0;
    for (int i = 0; i < size; i++)
    {
        int min = INF;
        for (int j = 0; j < size; j++)
        {
            if (!visited[j] && D[j] < min)
            {
                min = D[j];
                index = j;
            }
        }
        u = index;
        visited[u] = true;
        for (int j = 0; j < size; j++)
        {
            if (!visited[j] && w[u][j] != INF && D[u] != INF && (D[u] + w[u][j] < D[j]))
            {
                D[j] = D[u] + w[u][j];
            }
        }
    }


    output(size, D, st, 0);
}

int main(int argc, char* argv)
{
    int apex;
    int CountApex;
    int** mputh;
    int i, j, E = 0;
    int negative = 0;
    float start, end;
    setlocale(LC_ALL, "Rus");
    FILE* in;
    char filename[20];
    generation();
    printf("Введите имя файла: ");
    scanf_s("%s", filename, 20);
    fopen_s(&in, filename, "r");

    if (!in)
        printf("Ошибка! Файл не прочитан");
    else
        printf("Читаем граф!\n");

    fscanf_s(in, "%d", &CountApex);

    mputh = (int**)malloc(CountApex * sizeof(int*));
    for (int i = 0; i < CountApex; i++) 
        mputh[i] = (int*)malloc(CountApex * sizeof(int));

    for (i = 0; i < CountApex; i++)
    {
        for (j = 0; j < CountApex; j++)
        {
            fscanf_s(in, "%d", &mputh[i][j]);
            if (mputh[i][j] < 0)
                negative++;
        }
    }

    printf("Граф считан\n");
    printf("Введите вершину, от которой ведем отсчет: ");
    scanf_s("%d", &apex);
    
    start = clock();
    deikstra(CountApex, mputh, apex);
    end = clock();
    
    printf("Алгоритм использовал %.10f секунд.\n", (end - start) / (CLOCKS_PER_SEC));

    if (negative != 0)
        printf("Матрица содержит ребра с отрицательным весом, алгоритм Дейкстры может работать некорректно\n");

    printf("\n\n");

    start = clock();

    Ford_Ballman(CountApex, mputh, apex);
    end = clock();

    printf("Алгоритм использовал %.10f секунд.\n", (end - start) / (CLOCKS_PER_SEC));
    getchar();
    getchar();
    return 0;
}
