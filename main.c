#include <stdio.h>
#include <stdlib.h>




const char maxchar=127;
const char minchar=-128;
const short int maxint=32767;
const short int minint=-32768;
const long int maxlong=2147483647;
const long int minlong=-2147483648;




char SumaChar(char a, char b)
{
    if (a>0 && b>0){              //Si los 2 son positivos
        if (a <= maxchar-b) {        //a + b <= maxchar
            return(a + b);
        }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    if (a<0 && b<0){
        if (a >= minchar-b) {      //a + b >= minichar
            return(a + b);
        }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    else {                  //Si son de distinto signo ò 0 no hay problema
        return(a + b);
    }
}


char RestaChar (char a, char b)
{
    if ((a<0 && b>0) || (a>0 && b<0)){        //Si los 2 son de distintos signos o cero
        if (b < 0) {                                //Si b es negativo (-- = +)
            if (a <= maxchar+b) {                     //a - b <= maxchar
                return(a - b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
        else {              //a es negativo
            if (a >= minchar+b) {                     //a - b >= minchar
                return(a - b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
    }
    else {              //Si son de = signo o 0 no hay problema
        return(a - b);
    }
}


char MultChar (char a, char b)
{
    if (a==0 || b==0){
       return(0);
    }
    if (a>0 && b>0){               //Si los 2 son positivos, el resultado es positivo
        if (a <= maxchar/b) {                     //a * b <= maxchar
            return(a * b);
            }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    if (a<0 && b<0){        //Si los 2 son negativos, el resultado es positivo
        if (a >= maxchar/b) {                     //a * b <= maxchar
            return(a * b);
            }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    else {                          //Son de distinto signo, el resultado es negativo
        if (b>0){
            if (a >= minchar/b) {                     //a * b >= minchar
                return(a * b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
        else {                      //b es negativo, cambia la desigualdad
            if (a <= minchar/b) {                     //a * b >= minchar
                return(a * b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
    }
}


char DivChar(char a, char b)
{
    if (b != 0){
        return(a/b);
    }
    else {
        printf("No se puede dividir por '0'!!!!\n");
        return(0);
    }
}


void CalcuChar()
{
  static char x, y, operador[1];

    printf("\n\nCalculadora CHAR\n");
    printf("Esta calculadora opera con el siguiente rango de numeros: %d....%d\n", minchar, maxchar);
    do {
        printf("\nIngrese un operando....");
        scanf("%d", &x);
        printf("\nIngrese un operando....");
        scanf("%d", &y);
        printf("\nIngrese un operador: '+' '-' '*' '/'....");
        scanf("%s", operador);
        switch (operador[0]) {
            case '+': printf("    %d + %d = %d\n", x, y, SumaChar(x, y)); break;
            case '-': printf("    %d - %d = %d\n", x, y, RestaChar(x, y)); break;
            case '*': printf("    %d * %d = %d\n", x, y, MultChar(x, y)); break;
            case '/': printf("    %d / %d = %d\n", x, y, DivChar(x, y)); break;
            default: printf("\nOperador u operando no valido!!!!\n");
        }
        printf("\n\nIngrese 'Q' para salir de esta calculadora, o cualquier otra tecla para continuar....");
        scanf("%s", operador);
        printf("\n\n");
    }
    while (operador[0]!='q' && operador[0]!='Q');
}


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


short int SumaInt (int a, int b)
{
    if (a>0 && b>0){
        if (a <= maxint-b) {
            return(a + b);
        }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    if (a<0 && b<0){
        if (a >= minint-b) {
            return(a + b);
        }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    else {
        return(a + b);
    }
}


short int RestaInt (int a, int b)
{
    if ((a<0 && b>0) || (a>0 && b<0)){
        if (b < 0) {
            if (a <= maxint+b) {
                return(a - b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
        else {
            if (a >= minint+b) {
                return(a - b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
    }
    else {
        return(a - b);
    }
}


short int MultInt (int a, int b)
{
    if (a==0 || b==0){
       return(0);
    }
    if (a>0 && b>0){
        if (a <= maxint/b) {
            return(a * b);
            }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    if (a<0 && b<0){
        if (a >= maxint/b) {
            return(a * b);
            }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    else {
        if (b>0){
            if (a >= minint/b) {
                return(a * b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
        else {
            if (a <= minint/b) {
                return(a * b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
    }
}


short int DivInt (short int a, short int b)
{
    if (b != 0){
        return(a/b);
    }
    else {
        printf("No se puede dividir por '0'!!!!\n");
        return(0);
    }
}


void CalcuInt()
{
  static short int x, y;
  static char operador[1];

    printf("\n\nCalculadora INT\n");
    printf("Esta calculadora opera con el siguiente rango de numeros: %d....%d\n", minint, maxint);
    do {
        printf("\nIngrese un operando....");
        scanf("%d", &x);
        printf("\nIngrese un operando....");
        scanf("%d", &y);
        printf("\nIngrese un operador: '+' '-' '*' '/'....");
        scanf("%s", operador);
        switch (operador[0]) {
            case '+': printf("    %d + %d = %d\n", x, y, SumaInt(x, y)); break;
            case '-': printf("    %d - %d = %d\n", x, y, RestaInt(x, y)); break;
            case '*': printf("    %d * %d = %d\n", x, y, MultInt(x, y)); break;
            case '/': printf("    %d / %d = %d\n", x, y, DivInt(x, y)); break;
            default: printf("\nOperador u operando no valido!!!!\n");
        }
        printf("\n\nIngrese 'Q' para salir de esta calculadora, o cualquier otra tecla para continuar....");
        scanf("%s", operador);
        printf("\n\n");
    }
    while (operador[0]!='q' && operador[0]!='Q');
}


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


long int SumaLong (long int a, long int b)
{
    if (a>0 && b>0){
        if (a <= maxlong-b) {
            return(a + b);
        }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    if (a<0 && b<0){
        if (a >= minlong-b) {
            return(a + b);
        }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    else {
        return(a + b);
    }
}


long int RestaLong (long int a, long int b)
{
    if ((a<0 && b>0) || (a>0 && b<0)){
        if (b < 0) {
            if (a <= maxlong+b) {
                return(a - b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
        else {
            if (a >= minlong+b) {
                return(a - b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
    }
    else {
        return(a - b);
    }
}


long int MultLong (long int a, long int b)
{
    if (a==0 || b==0){
       return(0);
    }
    if (a>0 && b>0){
        if (a <= maxlong/b) {
            return(a * b);
            }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    if (a<0 && b<0){
        if (a >= maxlong/b) {
            return(a * b);
            }
        else {
            printf("OVERFLOW!!!!\n");
            return(0);
        }
    }
    else {
        if (b>0){
            if (a >= minlong/b) {
                return(a * b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
        else {
            if (a <= minlong/b) {
                return(a * b);
            }
            else {
                printf("OVERFLOW!!!!\n");
                return(0);
            }
        }
    }
}


long int DivLong (long int a, long int b)
{
    if (b != 0){
        return(a/b);
    }
    else {
        printf("No se puede dividir por '0'!!!!\n");
        return(0);
    }
}


void CalcuLong()
{
  static long int x, y;
  static char operador[1];

    printf("\n\nCalculadora LONG\n");
    printf("Esta calculadora opera con el siguiente rango de numeros: %ld....%ld\n", minlong, maxlong);
    do {
        printf("\nIngrese un operando....");
        scanf("%ld", &x);
        printf("\nIngrese un operando....");
        scanf("%ld", &y);
        printf("\nIngrese un operador: '+' '-' '*' '/'....");
        scanf("%s]", operador);
        switch (operador[0]) {
            case '+': printf("    %d + %d = %d\n", x, y, SumaLong(x, y)); break;
            case '-': printf("    %d - %d = %d\n", x, y, RestaLong(x, y)); break;
            case '*': printf("    %d * %d = %d\n", x, y, MultLong(x, y)); break;
            case '/': printf("    %d / %d = %d\n", x, y, DivLong(x, y)); break;
            default: printf("\nOperador u operando no valido!!!!\n");
        }
        printf("\n\nIngrese 'Q' para salir de esta calculadora, o cualquier otra tecla para continuar....");
        scanf("%s", operador);
        printf("\n\n");
    }
    while (operador[0]!='q' && operador[0]!='Q');
}




int main()
{
  static char calc[1];

    printf("CALCULADORA POLACA\n");
    do {
        printf("\nPara utilizar una calculadora de tipo CHAR presione 'C'\n");
        printf("Para utilizar una calculadora de tipo INTEGER presione 'I'\n");
        printf("Para utilizar una calculadora de tipo LONG presione 'L'\n");
        printf("Para terminar presione 'Q'\n");
        scanf("%s", calc);
        //scanf("%c", &basura);   //Lee caracteres basuras
        switch (calc[0]) {
            case 'C': CalcuChar(); break;
            case 'I': CalcuInt(); break;
            case 'L': CalcuLong(); break;
        }
    }
    while (calc[0]!='q' && calc[0]!='Q');
    return (0);
}
