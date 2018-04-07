#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> 
#include <assert.h>
//-lm
enum BRACKET {
    BRACKET_OPEN = '(',
    BRACKET_CLOSE = ')'
};

enum OPERATION {
    OP_PLUS = '+',
    OP_MINUS = '-',
    OP_MULT = '*',
    OP_DIV = '/',
    OP_POWER = '^',
    AMT 
};

enum LexemType {
    OPERATION,
    NUMBER
};

struct Lexem { 
    enum LexemType type; 
    union{ 
        double num; 
        enum OPERATION op;  
    } value; 
}; 

struct LexemList {
    struct Lexem lex;
    struct LexemList * next;
};

int prioritize (const char operation) {
    switch(operation) {
        case OP_POWER:
            return 5;
            break;
        case OP_MULT:
            return 4;
            break;
        case OP_DIV:
            return 4;
            break;
        case OP_MINUS:
            return 3;
            break;
        case OP_PLUS:
            return 3;
            break;
        case BRACKET_OPEN:
            return 2;
    }
}

//записывает на стек символ и возвращает новую голову стека
struct LexemList * push (struct LexemList *head, const char operation) {
  struct LexemList *stack = (struct LexemList*)calloc(1, sizeof(struct LexemList));
  stack->lex.value.op = operation;                                
  stack->next = head;           
  return stack;                               
}

//удаляет из стека символ и возвращает его
int pop (struct LexemList **stack) {
    struct LexemList *p = *stack;                   
    char operation = p->lex.value.op;
    *stack = p->next;         
    free(p);                  
    return operation;                   
}

//проверка на унарные операции
double unary (const char lexem, const char prelexem, const char * operations) {
    for (int j = 0; j < AMT; j++) {
        if (prelexem == *(operations + j)) {
            if (lexem == OP_MINUS){
                return -1;
            }
            if (lexem == OP_PLUS) {
                return 1;
            }
        }
    }

    if (prelexem == BRACKET_OPEN || prelexem == -1) {
        if (lexem == OP_MINUS){
            return -1;
        }
        if (lexem == OP_PLUS) {
            return 1;
        }
    } 

    return 0;
}

//считывание числа из строки 
double numbers (char * lexem, char * prelexem, double signe) {
    double result = 0.;
    double fraction = 0.;
    int grade = 1;
    while (isdigit(*lexem)) {
        result *= 10;
        result += *lexem - '0';
        *prelexem = *lexem;
        *lexem = fgetc(stdin);
        if (*lexem == '.') {
            *lexem = fgetc(stdin);
            grade = 1;
            while(isdigit(*lexem)) {
                fraction += (*lexem - '0')/(pow(10, grade));
                grade++;
                *prelexem = *lexem;
                *lexem = fgetc(stdin);
            }
        }
    }

    result = result + fraction;
    result *= signe;

    return result;
}

void print (struct LexemList *head) {
    struct LexemList *pointer = head;
    while(NULL != pointer) {
        if (pointer->lex.type == OPERATION) {
            printf("%c  ", pointer->lex.value);
        }
        if (pointer->lex.type == NUMBER) {
            printf("%lf  ", pointer->lex.value);
        }
        pointer = pointer->next;
    }
}

void free_list (struct LexemList *head) {
    struct LexemList *next = head;
    struct LexemList *pointer = head;
    while(NULL != pointer) {
        pointer = pointer->next;
        free(next);
        next = pointer;
    }
}

//перевод в обратную польскую запись
struct LexemList * reverse_polish () {
    struct LexemList *head = NULL;
    struct LexemList *stack = NULL;     
    struct LexemList *pointer = head; //выход
    char operations[AMT] = {OP_DIV, OP_PLUS, OP_MULT, OP_MINUS, OP_POWER};
    double signe = 1;
    int OK = 0; //проверка на допустимые символы
    char lexem = 0; // текущий символ
    char prelexem = -1; // предыдущий символ
    while ((lexem = fgetc(stdin)) != '\n') {
        if (isspace(lexem)) {
            continue;
            OK++;
        }
        if (isdigit(lexem)) {
            struct LexemList *p = (struct LexemList*)calloc(1, sizeof(struct LexemList)); 
            p->lex.value.num =  numbers(&lexem, &prelexem, signe);
            p->lex.type = 1;
            signe = 1;

            if (head == NULL) {
                head = p;
                pointer = p;
                pointer->next = NULL;
            }
            else {
                pointer->next = p;
                pointer = p;
                pointer->next = NULL;
            } 
            OK++;
        }

        if (lexem == BRACKET_OPEN) {
            stack = push(stack, BRACKET_OPEN);
            OK++;
        }

        if (lexem == BRACKET_CLOSE) {
            if (stack == NULL) {
                printf("ERROR! Invalid bracket!\n");
                free_list(stack);
                free_list(head);
                return NULL;
            }

            while (stack->lex.value.op != BRACKET_OPEN) {
                struct LexemList *p = (struct LexemList*)calloc(1, sizeof(struct LexemList)); 
                p->lex.value.op = pop(&stack);
                pointer->next = p;
                pointer = p;
                pointer->next = NULL;
                if (stack == NULL) {
                    printf("ERROR! Invalid bracket!\n");
                    free_list(stack);
                    free_list(head);
                    return NULL;
                } 
            }
            pop(&stack);
            OK++;
        }

        for (int j = 0; j < AMT; j++) {
            if (lexem == operations[j]) {
                OK++;
                signe *= unary(lexem, prelexem, operations);
                if (signe != 0) {
                    break;
                }
                else {
                    signe = 1;
                }

                if (stack == NULL) {
                    stack = push(stack, lexem);
                }
                else {
                    if (prioritize(lexem) > prioritize(stack->lex.value.op)) {
                        stack = push(stack, lexem);
                    }
                    else {
                        while(stack != NULL && prioritize(stack->lex.value.op) >= prioritize(lexem)) {
                            struct LexemList *p = (struct LexemList*)calloc(1, sizeof(struct LexemList)); 
                            p->lex.value.op = pop(&stack);
                            pointer->next = p;
                            pointer = p;
                            pointer->next = NULL;
                        }
                        stack = push(stack, lexem);
                    }
                }
            }
        }

        if (OK == 0) {
            printf("ERROR! Invalid string! Unacceptable symbols!");
            free_list(stack);
            free_list(head);
            return NULL;
        }
        else {
            OK = 0;
        }

        if (lexem == '\n') {
            break;
        }

        prelexem = lexem;
    }

    while (stack != NULL) {
        if (stack->lex.value.op == BRACKET_OPEN) {
            printf("ERROR! Invalid bracket\n");
            free_list(stack);
            free_list(head);
            return NULL;
        }
        else {
            struct LexemList *p = (struct LexemList*)calloc(1, sizeof(struct LexemList)); 
            p->lex.value.op = pop(&stack);
            pointer->next = p;
            pointer = p;
            pointer->next = NULL;
        }
    }

    return head;
}

int find_list (struct LexemList *head, struct LexemList **last, struct LexemList **prelast,  struct LexemList *p) {
    *last = head;
    if (*last == p) {
        printf("ERROR! Invalid operations!\n");
        return 0;
    }
    else {
        while ((*last)->next != p) {
            *last = (*last)->next;
        }
    }

    *prelast = head;
    if (*prelast == *last) {
        printf("ERROR! Invalid operations!\n");
        return 0;
    }
    else {
        while ((*prelast)->next != *last) {
            *prelast = (*prelast)->next;
        }
    }
    return 1;
}

struct LexemList * calculate (struct LexemList *head) {
    struct LexemList * p = head;
    struct LexemList * last = head;
    struct LexemList * prelast = head;
    double result = 0;
    struct LexemList * fre;

    while (p != NULL) {
        switch (p->lex.value.op) {
            case OP_PLUS: 
                if (find_list(head, &last, &prelast, p) == 0) {
                    free_list(head);
                    return NULL;
                }                      
                result = last->lex.value.num + prelast->lex.value.num;
                prelast->lex.value.num = result;
                prelast->next = p->next;
                last->next = NULL;
                fre = p;
                p = p->next;
                free(fre);
                free(last);                 
                break;
                    
            case OP_MINUS:
                if (find_list(head, &last, &prelast, p) == 0) {
                    free_list(head);
                    return NULL;
                }   
                result = prelast->lex.value.num - last->lex.value.num;
                prelast->lex.value.num = result;
                prelast->next = p->next;
                last->next = NULL;
                fre = p;
                p = p->next;
                free(fre);
                free(last);                  
                break;
                    
            case OP_DIV:
                if (find_list(head, &last, &prelast, p) == 0) {;
                    free_list(head);
                    return NULL;
                }   
                if (last->lex.value.num == 0) {
                    printf("INF\n");
                    free_list(head);
                    return NULL;
                }
                result = prelast->lex.value.num / last->lex.value.num;
                prelast->lex.value.num = result;
                prelast->next = p->next;
                last->next = NULL;
                fre = p;
                p = p->next;
                free(fre);
                free(last);                   
                break;

            case OP_MULT:
                if (find_list(head, &last, &prelast, p) == 0) {
                    free_list(head);
                    return NULL;
                }   
                result = prelast->lex.value.num * last->lex.value.num;
                prelast->lex.value.num = result;
                prelast->next = p->next;
                last->next = NULL;
                fre = p;
                p = p->next;
                free(fre);
                free(last);                  
                break;

            case OP_POWER:
                if (find_list(head, &last, &prelast, p) == 0) {
                    free_list(head);
                    return NULL;
                }   
                result = pow(prelast->lex.value.num, last->lex.value.num);
                prelast->lex.value.num = result;
                prelast->next = p->next;
                last->next = NULL;
                fre = p;
                p = p->next;
                free(fre);
                free(last); 
                break;

            default:
                p = p->next;              
        }
    }

    return head;
}

int main () { 
    printf("Please type...\n");
    struct LexemList *head = reverse_polish();
    if (head == NULL) {
        free_list(head);
        return 1;
    }
    head = calculate(head);
    if (head == NULL) {
        free_list(head);
        return 1;
    }
    print(head);
    free_list(head);  

    return 0;
}