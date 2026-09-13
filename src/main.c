#include <stdio.h>

#define MAX_SIZE 10
#define MAX_FILENAME 100
#define BW 0
#define COLOR 1
#define BW_PRICE 1.0
#define COLOR_PRICE 5.0

typedef struct
{
    char fileName[MAX_SIZE][MAX_FILENAME];
    int pages[MAX_SIZE];
    int printType[MAX_SIZE];
    double jobPrice[MAX_SIZE];

    int front;
    int rear;
    int count;

    int paperAmount;
    int blackInkAmount;
    int colorInkAmount;

    double totalIncome;
} PrinterSystem;

int main()
{
    printf("Hello !!!\n");
    return 0;
}