#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
    int count; // fast check if full

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
    PrinterSystem printer;
    initPrinterSystem(&printer);
    bool exit = true;
    char input[100];
    int choice;

    while (exit)
    {
        while (true)
        {
            printf("========== Printer Job Scheduling System ==========\n");
            printf("1. Add Print Job\n");
            printf("2. Print First Job in Queue\n");
            printf("3. Display All Jobs in Queue\n");
            printf("4. Refill Paper\n");
            printf("5. Refill Ink\n");
            printf("6. Display Printer Status\n");
            printf("7. Display Total Income\n");
            printf("0. Exit Program\n");
            printf("===================================================\n");
            printf("Select Menu: ");

            fgets(input, sizeof(input), stdin); // input string
            choice = atoi(input);               // convert to number
            if (choice >= 1 && choice <= 7)
            {
                break;
            }
            else if (choice == 0)
            {
                exit = false;
                printf("Exiting Program. Goodbye!\n");
                break;
            }
            else
            {
                printf("Error: Please enter numbers only!\n");
            }
        }

        switch (choice)
        {
        case 1:
            // code here
            break;
        case 2:
            // code here
            break;
        case 3:
            // code here
            break;
        case 4:
            // code here
            break;
        case 5:
            // code here
            break;
        case 6:
            // code here
            break;
        case 7:
            // code here
            break;
        case 0:
            printf(isEmpty(&printer));
            printf("close done ~~~\n");
            break;

        default:
            printf("Error: Invalid menu! Please select 0-7.\n");
            break;
        }
    }
}

// set init data
void initPrinterSystem(PrinterSystem *printer)
{
    printer->front = 0;
    printer->rear = 0;
    printer->count = 0;
    printer->paperAmount = 100;
    printer->blackInkAmount = 100;
    printer->colorInkAmount = 100;
    printer->totalIncome = 0;
}

bool isEmpty(const PrinterSystem *printer)
{
    if (printer->count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}