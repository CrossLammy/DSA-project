#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

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
void refillInkMenu(PrinterSystem *printer); //รับชนิดหมึกและจำนวนหมึกจากผู้ใช้
void viewTotalIncome(const PrinterSystem *printer);// แสดงรายได้รวม

typedef enum
{
    INPUT_OK,
    INPUT_INVALID,
    INPUT_END
} InputResult;

// Consume the entire line, including invalid lines, so prompts stay in sync.
static InputResult readLine(char *buffer, size_t capacity)
{
    size_t length = 0;
    bool tooLong = false;
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (length + 1 < capacity)
            buffer[length++] = (char)ch;
        else
            tooLong = true;
        if (ch == '\0')
            tooLong = true;
    }
    buffer[length] = '\0';
    if (ferror(stdin) || (ch == EOF && length == 0 && !tooLong))
        return INPUT_END;
    if (tooLong)
        return INPUT_INVALID;
    if (length > 0 && buffer[length - 1] == '\r')
        buffer[--length] = '\0';
    return INPUT_OK;
}

static InputResult readInteger(int *value)
{
    char input[100];
    InputResult result = readLine(input, sizeof(input));
    if (result != INPUT_OK)
        return result;

    char *end;
    errno = 0;
    long parsed = strtol(input, &end, 10);
    if (end == input || errno == ERANGE || parsed < INT_MIN || parsed > INT_MAX)
        return INPUT_INVALID;
    while (isspace((unsigned char)*end))
        end++;
    if (*end != '\0')
        return INPUT_INVALID;

    *value = (int)parsed;
    return INPUT_OK;
}

int main(void)
{
    PrinterSystem printer;
    initPrinterSystem(&printer);
    bool exit = true;
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

            InputResult result = readInteger(&choice);
            if (result == INPUT_END)
            {
                printf("Input closed. Goodbye!\n");
                return 0;
            }
            if (result == INPUT_OK && choice >= 0 && choice <= 7)
            {
                break;
            }
            else
            {
                printf("Error: Please enter an integer from 0 to 7!\n");
            }
        }

        switch (choice)
        {
        case 0:
            exit = false;
            printf("Exiting Program. Goodbye!\n");
            break;
        case 1:
            {
            char file[MAX_FILENAME];
            int page;
            int typeChoice;
            bool isColor;
 
            printf("Enter file name: ");
            if (readLine(file, sizeof(file)) != INPUT_OK ||
                file[strspn(file, " \t\r\v\f")] == '\0')
            {
                printf("Error: Enter a non-empty file name of at most %d characters!\n", MAX_FILENAME - 1);
                break;
            }
 
            printf("Enter number of pages: ");
            if (readInteger(&page) != INPUT_OK || page <= 0)
            {
                printf("Error: Invalid number of pages!\n");
                break;
            }
            printf("Select print type (1 = BW, 2 = COLOR): ");
            if (readInteger(&typeChoice) != INPUT_OK)
            {
                printf("Error: Invalid print type! Please select 1 (BW) or 2 (COLOR).\n");
                break;
            }
            if (typeChoice != 1 && typeChoice != 2)
            {
                printf("Error: Invalid print type! Please select 1 (BW) or 2 (COLOR).\n");
                break;
            }
 
            isColor = (typeChoice == 2);
 
            enqueue(&printer, file, page, isColor);
            break;
        }
        case 2:
            dequeue(&printer);
            break;
        case 3:
            viewQueue(&printer);
            break;
        case 4:
            {
            int amount;
 
            printf("Enter amount of paper to add: ");
            if (readInteger(&amount) != INPUT_OK)
            {
                printf("Error: Invalid paper amount!\n");
                break;
            }
            addPaper(&printer, amount);
            break;
        }
        case 5:
            refillInkMenu(&printer);
            break;
        case 6:
            viewPrinterStatus(&printer);
            break;
        case 7:
            viewTotalIncome(&printer);
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

bool isFull(const PrinterSystem *printer)
{
    if (printer->count == MAX_SIZE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool enqueue(PrinterSystem *printer, const char file[], int page, bool isColor)
{
    if (isFull(printer))
    {
        printf("Queue is full\n");
        return false;
    }

    if (page <= 0 || file[0] == '\0')
    {
        printf("Invalid print job\n");
        return false;
    }
    if (isEmpty(printer))
    {
        printf("Queue is empty. Adding the first job.\n");
    }

    int index = printer->rear;

    snprintf(printer->fileName[index], MAX_FILENAME, "%s", file); // input string save
    printer->pages[index] = page;
    printer->isColor[index] = isColor;
    printer->jobPrice[index] = calculatePrice(page, isColor);

    printer->rear = (printer->rear + 1) % MAX_SIZE;

    printer->count++;
    printf("Added %s to queue\n", file);
    return true;
}

void viewQueue(const PrinterSystem *printer)
{
    double totalPrice = 0.0;

    if (isEmpty(printer))
    {
        printf("\nQueue is empty.\n");
        return;
    }
    printf("\n");
    printf("======================= PRINT QUEUE =======================\n");
    printf("%-4s %-25s %7s %-8s %9s %-8s\n", "No.", "File name", "Pages", "Type", "Price", "Status");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < printer->count; i++)
    {
        int index = (printer->front + i) % MAX_SIZE;

        printf(
            "%-4d %-25.25s %7d %-8s %9.2f %-8s\n",
            i + 1,
            printer->fileName[index],
            printer->pages[index],
            printer->isColor[index] ? "COLOR" : "BW",               // short hand
            printer->jobPrice[index], i == 0 ? "NEXT" : "WAITING"); // look first in queue

        totalPrice += printer->jobPrice[index];
    }
    printf("-----------------------------------------------------------\n");
    printf("Job queue: %d/%d\n", printer->count, MAX_SIZE);
    printf("Sum price: %.2f bath\n", totalPrice);
    printf("===========================================================\n");
}

// เริ่มตรงนี้นะจ๊ะ
// ตรวจว่ามีกระดาษเพียงพอสำหรับการพิมพ์ไหม
bool hasEnoughPaper(const PrinterSystem *printer, int page)
{
    return printer->paperAmount >= page;
}

// ตรวจว่ามีหมึกเพียงพอสำหรับการพิมพ์ไหม
bool hasEnoughInk(const PrinterSystem *printer, int page, bool isColor)
{
    if (isColor)
    {
        return printer->colorInkAmount >= page;
    }
    else
    {
        return printer->blackInkAmount >= page;
    }
}

// พิมพ์งานแล้วก็เอางานแรกออกจากคิว
bool dequeue(PrinterSystem *printer)
{
    if (isEmpty(printer))
    {
        printf("Queue is empty. No job to print.\n");
        return false;
    }

    int index = printer->front;
    int page = printer->pages[index];
    bool isColor = printer->isColor[index];

    // ตรวจดูกระดาษและหมึกก่อนพิมพ์
    if (!hasEnoughPaper(printer, page))
    {
        printf("Not enough paper to print %s.\n",
               printer->fileName[index]);
        return false;
    }

    if (!hasEnoughInk(printer, page, isColor))
    {
        printf("Not enough ink to print %s.\n",
               printer->fileName[index]);
        return false;
    }

    // ลดกระดาษและหมึกหลังพิมพ์
    printer->paperAmount -= page;

    if (isColor)
    {
        printer->colorInkAmount -= page;
    }
    else
    {
        printer->blackInkAmount -= page;
    }

    printf("Printed %s successfully.\n", printer->fileName[index]);

    // เพิ่มรายได้หลังพิมพ์สำเร็จ
    printer->totalIncome += printer->jobPrice[index];
    printf("Job price: %.2f\n", printer->jobPrice[index]);
    printf("Total income: %.2f\n", printer->totalIncome);

    printer->front = (printer->front + 1) % MAX_SIZE;
    printer->count--;

    return true;
}

// เพิ่มกระดาษ
bool addPaper(PrinterSystem *printer, int amount)
{
    if (amount <= 0)
    {
        printf("Invalid paper amount.\n");
        return false;
    }

    if (amount > INT_MAX - printer->paperAmount)
    {
        printf("Error: Paper amount exceeds the supported limit.\n");
        return false;
    }
    printer->paperAmount += amount;

    printf("Added %d paper(s) successfully.\n", amount);
    printf("Current paper amount: %d\n", printer->paperAmount);

    return true;
}

// เติมหมึกดำและหมึกสี
bool refillInk(PrinterSystem *printer, bool isColor, int amount)
{
    if (amount <= 0)
    {
        printf("Invalid ink amount.\n");
        return false;
    }

    int currentAmount = isColor ? printer->colorInkAmount : printer->blackInkAmount;
    if (amount > INT_MAX - currentAmount)
    {
        printf("Error: Ink amount exceeds the supported limit.\n");
        return false;
    }

    if (isColor)
    {
        printer->colorInkAmount += amount;
        printf("Added %d color ink successfully.\n", amount);
        printf("Current color ink amount: %d\n", printer->colorInkAmount);
    }
    else
    {
        printer->blackInkAmount += amount;
        printf("Added %d black ink successfully.\n", amount);
        printf("Current black ink amount: %d\n", printer->blackInkAmount);
    }

    return true;
}

//แครอท/เมย์ เริ่มตรงนี้จ้าาาาาาา
double calculatePrice(int page, bool isColor)
{
    if (isColor)
    {
        return page * COLOR_PRICE;
    }
    else
    {
        return page * BW_PRICE;
    }
}

void refillInkMenu(PrinterSystem *printer)
{
    int type;
    int amount;
    printf("1. Black Ink\n");
    printf("2. Color Ink\n");
    printf("Select type: ");

    if (readInteger(&type) != INPUT_OK)
    {
        printf("Please select only 1 or 2.\n");
        return;
    }

    if (type != 1 && type != 2)
    {
        printf("Please select only 1 or 2.\n");
        return;
    }

    printf("Enter ink amount: ");

    if (readInteger(&amount) != INPUT_OK)
    {
        printf("Please enter numbers only.\n");
        return;
    }

    if (amount <= 0)
    {
        printf("Invalid ink amount.\n");
        return;
    }

    refillInk(printer, type == 2, amount);
}

void viewPrinterStatus(const PrinterSystem *printer)
{
    printf("\n");
    printf("===================== PRINTER STATUS =====================\n");
    printf("Jobs in queue   : %d/%d\n", printer->count, MAX_SIZE); // จำนวนงานในคิว
    printf("Paper amount    : %d\n", printer->paperAmount);       // จำนวนกระดาษ
    printf("Black ink amount: %d\n", printer->blackInkAmount);    // หมึกดำ
    printf("Color ink amount: %d\n", printer->colorInkAmount);    // หมึกสี
    printf("============================================================\n");
}

void viewTotalIncome(const PrinterSystem *printer)
{
    printf("Total Income: %.2f Baht\n", printer->totalIncome);
}
