#include<stdio.h>
#include<string.h>

int main(int argc,char*argv[]){
    char * first_input = argv[1];
    int first_length = 0;
    char * second_input = argv[2];
    int second_length = 0;
    while(*first_input!='\0')
    {
        if(*first_input < 97)
            *first_input += 32;
        first_input++;
        first_length++;
    }
    while(*second_input!='\0')
    {
        if(*second_input < 97)
            *second_input += 32;
        second_input++;
        second_length++;
    }  
    second_input = argv[2];
    first_input = argv[1]; 
    if(second_length == first_length){
        printf("%d \n",0);
        return 0;
    }
    if(first_length < second_length){

        char * swapper = second_input;
        second_input = first_input;
        first_input = swapper;

        int length = second_length;
        second_length = first_length;
        first_length = length;
    }
    while(*second_input!='\0'){
        while(*first_input != *second_input && *first_input != '\0'){
            first_input++;
        }
        if(*first_input == '\0')
            {
                printf("0\n");
                return 0;
            }
        second_input++;
    }
    printf("1\n");

    
    return 1;
}