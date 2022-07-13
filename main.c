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

struct answers{
    char answer_string[MAX_CHARACTER_SIZE];
};

struct question // Struct that stores our question object
{
    char question[MAX_CHARACTER_SIZE];
    struct answers ans[MAX_POSSIBLE_ANSWERS]; // Array of answers
    int index_correct_answer;

};

void print_menu(){
    printf("\n");
    printf("1: Create new question \n");
    printf("2: Prompt all stored questions and its answers \n");
    printf("9: Show this menu \n");
    printf("0: Exit program \n");
    printf("\n");

}

void create_question(struct question q[], int index){
    char q_string[MAX_CHARACTER_SIZE];
    int i = 0;
    int right_ans_index;
    int control_input = 1;

    // Read user input
    fgets(q[index].question,MAX_CHARACTER_SIZE,stdin);
    for(i;i<MAX_POSSIBLE_ANSWERS;i++){
        printf("Write the possible answer with index %d : ", i);
        fgets(q[index].ans[i].answer_string,MAX_CHARACTER_SIZE,stdin);
    }
    printf("Which one is the right answer? \n");
    while (control_input){
        printf("Write the index [0-4] of the right answer: ");
        scanf("%d",&right_ans_index);
        getchar();
        if (right_ans_index <0 | right_ans_index >4){
            printf("Wrong number, choose one of the answers[0-4] \n");
        } else {
            q[index].index_correct_answer = right_ans_index;
            control_input = 0;
        }
    }
}
/*
Displays all questions and its answers that are currently stored
*/

void prompt_questions(struct question q[], int index){
    if(index == 0){
        printf("There are not any questions stored \n");
    } else{
        printf("Showing all stored questions: \n \n");
        for(int i=0;i<index;i++){
            printf("Question %d: %s \n", i, q[i].question);
            for(int j=0;j<MAX_POSSIBLE_ANSWERS;j++){
                if(q[i].index_correct_answer == j){
                    printf("** Answer %d: %s", j, q[i].ans[j].answer_string);               
                } else{
                    printf("   Answer %d: %s", j, q[i].ans[j].answer_string);
                }
            }
            printf("\n");
        }
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
                run_condition = 0;
                break;
            
            case 1:
                printf("Write your question: ");
                create_question(q_global,index);
                index++;
                break;

            case 2:
                prompt_questions(q_global,index);
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