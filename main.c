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

#define MAX_CHARACTER_SIZE 1024
#define MAX_NUMBER_QUESTIONS 50
#define MAX_POSSIBLE_ANSWERS 5


struct question // Struct that stores our question object
{
    char *question;
    char **answers; // Array of answers
    int index_correct_answer;

};

void print_menu(){
    printf("\n");
    printf("1: Create new question \n");
    printf("9: Show this menu \n");
    printf("0: Exit program \n");
    printf("\n");

}

void create_question(struct question q[], int index){
    char *q_string;

    // Memory allocation
    q_string = malloc(sizeof(char) * MAX_CHARACTER_SIZE);
    q[index].question = malloc(sizeof(char) * MAX_CHARACTER_SIZE);

    // Read user input
    fgets(q_string,MAX_CHARACTER_SIZE,stdin);
    strcpy(q[index].question,q_string);
    printf("Your question was: %s", q[index].question);
    
    free(q_string);

}

// Procedure that frees all previus memory allocations
void free_questions(struct question q[], int index){
    int i = 0;

    for(i;i<index;i++){
        free(q[i].question);
    }
}

/*
    We need a global array of all questions structures

    Every index should point to a question structure that will
    have access to que actual question and the array of answers
    with the correct answer index too.
*/

int main(){
    struct question q_global[MAX_NUMBER_QUESTIONS];
    int run_condition = 1;
    int menu_selection;
    int index = 0;

    printf("Welcome! \n");
    print_menu();
    while(run_condition){
        printf("Choose one option: ");
        scanf("%d", &menu_selection);
        getchar(); // This consumes the '\n' char entered by the user after clicking 'ENTER'
        switch (menu_selection){
            case 0:
                printf("Bye!\n");
                free_questions(q_global,index);
                run_condition = 0;
                break;
            
            case 1:
                printf("Write your question: ");
                create_question(q_global,index);
                index++;
                break;

            case 9:
                print_menu();
                break;
            
            default:
                printf("Choose an available option, select 9 to show menu \n");
                break;
        }
    }
    return 0;

}