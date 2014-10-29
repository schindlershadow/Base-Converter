/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * baseconverter.c
 * Copyright (C) 2014 SchindlerShadow <schindlershadow@schindlershadow.com>
 * 
 * BaseConverter is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BaseConverter is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * HEAP SUMMARY:
 *
 * in use at exit: 0 bytes in 0 blocks
 * total heap usage: 21 allocs, 21 frees, 1,411 bytes allocated
 *
 * All heap blocks were freed -- no leaks are possible
 *
 * ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 */


#include <stdio.h>
#include <string.h>

//converter array
char numbers[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//this public var is the number of answers provided by the input file
int ansSize = 0;

//print memory errors

void printError() {
    printf("\nERROR: MEMORY ALLOCATION FAILED: MEMORY FULL\n");
}

//set string to all NULL

void resetStr(char* str) {
    int i = 0;
    for (i = 0; i < 15; i++) {
        str[i] = '\0';
    }
}

//check if power of 2

int isPowerOf2(int num) {
    if (num == 2) {
        return 1;
    } else if (num == 4) {
        return 1;
    } else if (num == 8) {
        return 1;
    } else if (num == 16) {
        return 1;
    }
    return 0;
}

//find the power base^exp 

int pow(int base, int exp) {
    if (exp > 1)
        return (base * pow(base, exp - 1));
    if (exp == 1)
        return base;
    if (exp <= 0)
        return 1;
    return base;
}

//convert int to a string

char* intToStr(int x) {
    char *str = calloc(15, sizeof (char));
    if (str == NULL) {
        printError();
    }
    resetStr(str);
    str[14] = '\0';
    sprintf(str, "%d", x);
    //printf("\n%s", str);
    return str;

}

//convert string to int

int strToInt(char* str) {
    return atoi(str);
}

//add a char to the end of a string

char* append(char* str, char ch) {
    if (str == NULL) {
        str = (char*) calloc(15, sizeof (char));
        if (str == NULL) {
            printError();
        }
    }
    int len = strlen(str);
    str[len] = ch;
    str[len + 1] = '\0';
    return str;
}

//static grouping numbers for Converting between bases that are powers of 2

int getGroup(int base2) {
    if (base2 == 4) {
        return 2;
    } else if (base2 == 8) {
        return 3;
    } else if (base2 == 16) {
        return 4;
    } else {
        return 1;
    }
}

//builds and fills the answer array from input file

char** buildAnsArray(FILE *pf) {
    int num = 0, i = 0;
    char **arry;
    fscanf(pf, "%d", &num);
    ansSize = num;

    //alloc outer arry
    arry = (char**) calloc(num, sizeof (char*));
    if (arry == NULL) {
        printError();
    }
    for (i = 0; i < num; i++) {
        //alloc inner arry
        arry[i] = (char*) calloc(10, sizeof (char)*10);
        if (arry == NULL) {
            printError();
        }
        //get next line from file
        fscanf(pf, "%s", arry[i]);
    }

    return arry;
}

//convert to base 10

int toBase10(int num, int base) {
    int index = 0, i = 0, temp = 0;
    char arry[15], ans[15];
    resetStr(arry);
    resetStr(ans);
    sprintf(arry, "%d", num);
    for (index = strlen(arry) - 1; index >= 0; index--) {
        temp += (arry[i] - '0') * pow(base, index);
        i++;
    }
    return temp;
}

char* toBase2(int num, int base) {
    int toBase = 2;
    if (base != 10) {
        num = toBase10(num, base);
        base = 10;
    }
    if (num / toBase == 0 && num % toBase == 0) {
        return '\0';
    }
    return append(toBase2(num / toBase, base), numbers[num % toBase]);
}

//use a faster algorithm when base and exp is a power of 2

char* pow2Converter(int num, int base, int toBase) {
    //check if sane input
    if (base != 2) {
        num = strToInt(toBase2(num, base));
        base = 2;
    }
    char *str = intToStr(num);
    int groupings = getGroup(toBase), len = strlen(str);
    //base case
    if (len <= groupings) {
        resetStr(str);
        str[0] = numbers[toBase10(num, base)];
        return str;
    }

    char temp[15], temp2[15];
    resetStr(temp);
    resetStr(temp2);

    //this is where the magic happens
    strncpy(temp, str + (len - groupings), sizeof (temp));
    strncpy(temp2, str, len - groupings);
    free(str);

    char *str2 = pow2Converter(strToInt(temp2), base, toBase);
    return append(str2, numbers[toBase10(strToInt(temp), base)]);
}

//general base conversion function

char* baseConversion(int num, int base, int toBase) {
    //check if sane input
    if (toBase > 16) {
        return "";
    }
    //check if faster algorithm is available
    if (isPowerOf2(base) == 1 && isPowerOf2(toBase) == 1) {
        //make inputs sane by making input base2
        if (base != 2) {
            char *str = toBase2(num, base);
            num = strToInt(str);
            base = 2;
            free(str);
        }
        return pow2Converter(num, base, toBase);
    }
    //convert base to 10 if not in base 10
    if (base != 10) {
        num = toBase10(num, base);
        base = 10;
    }
    //base case
    if (num / toBase == 0 && num % toBase == 0) {
        return '\0';
    }

    //recursion step
    return append(baseConversion(num / toBase, base, toBase), numbers[num % toBase]);
}

//recursive binary search

int findAnswer(char** ans, char* arry, int high, int low) {
    int mid = 0;
    //base case 1
    if (low > high) {
        printf("key %s not found\n", arry);
        return 0;
    }
    mid = (low + high) / 2;
    //base case 2
    if (strcmp(ans[mid], arry) == 0) {
        return mid;
    }//recursion step 1
    else if (strcmp(ans[mid], arry) < 0) {
        findAnswer(ans, arry, high, mid + 1);
    }//recursion step 2
    else if (strcmp(ans[mid], arry) > 0) {
        findAnswer(ans, arry, mid - 1, low);
    } else {
        printf("key %s not found\n", arry);
        return 0;
    }

}

int main() {
    //Open input file
    FILE *pf = fopen("data.txt", "r");
    //build answer array
    char **ansArray = (char**) buildAnsArray(pf);
    int number = 0, base = 0, newBase = 0;

    //Main loop
    while (fscanf(pf, "%d %d %d", &number, &base, &newBase) == 3) {
        char *arry = baseConversion(number, base, newBase);
        printf("%d\n", findAnswer(ansArray, arry, ansSize, 0));
        free(arry);
        number = 0;
        base = 0;
        newBase = 0;
    }

    //frees
    int i = 0;
    for (i = 0; i < ansSize; i++) {
        free(ansArray[i]);
    }
    free(ansArray);
    //Close file
    fclose(pf);

    return (0);
}
