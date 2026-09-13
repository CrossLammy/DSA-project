#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 10
#define MAX_FILENAME 100
#define BW_PRICE 1.0
#define COLOR_PRICE 5.0

typedef struct
{
    char fileName[MAX_SIZE][MAX_FILENAME];
    int pages[MAX_SIZE];
    bool isColor[MAX_SIZE];
    double jobPrice[MAX_SIZE];

    int front;
    int rear;
    int count;

    int paperAmount;
    int blackInkAmount;
    int colorInkAmount;

    double totalIncome;
} PrinterSystem;

void initPrinterSystem(PrinterSystem *printer);
bool isEmpty(const PrinterSystem *printer);
bool isFull(const PrinterSystem *printer);

double calculatePrice(int page, bool isColor);
bool hasEnoughPaper(const PrinterSystem *printer, int page);
bool hasEnoughInk(const PrinterSystem *printer, int page, bool isColor);

bool enqueue(PrinterSystem *printer, const char file[], int page, bool isColor);
bool dequeue(PrinterSystem *printer);
void viewQueue(const PrinterSystem *printer);

bool addPaper(PrinterSystem *printer, int amount);
bool refillInk(PrinterSystem *printer, bool isColor, int amount);
void viewPrinterStatus(const PrinterSystem *printer);
void viewTotalIncome(const PrinterSystem *printer);

int main()
{
}

void initPrinterSystem(PrinterSystem *printer)
{
}