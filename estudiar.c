/*
    This is a program to help me study in the university
    The idea is while attending lectures, write possible exam
    questions, its posible answers and the right one. Storing 
    everything locally in files. 

    Finally, before the exam takes place, prompting random 
    questions to the user and its possibles answers asking 
    the user to choose the right one.


*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHARACTER_SIZE 100
#define MAX_POSSIBLE_ANSWERS 5

struct question
{
    char *question;
    char **answers; // Array of answers
    int index_correct_answer;

};

void print_menu(){
    printf("\n");
    printf("1: Create new question \n");
    printf("0: Exit program \n");
    printf("\n");

}

// If q_var is NULL then inicialize it
void create_question(){
    char *q;
    q = malloc(sizeof(char) * MAX_CHARACTER_SIZE);
    printf("Write your question: \n");
    fgets(q,MAX_CHARACTER_SIZE,stdin);
    printf("Your question was: %s", q);
    free(q);



    
}

/*
    We need a global array of all questions structures

    Every index should point to a question structure that will
    have access to que actual question and the array of answers
    with the correct answer index too.
*/

int main(){
    struct question q_global[50];
    int run_condition = 1;
    int menu_selection;

    printf("Welcome! \n");
    while(run_condition){
        print_menu();
        scanf("%d", &menu_selection);
        switch (menu_selection){
            case 0:
                printf("Bye!\n");
                run_condition = 0;
                break;
            
            case 1:
                create_question();
                break;
            
            default:
                printf("Choose an available option \n");
                break;
        }
    }
    return 0;

}