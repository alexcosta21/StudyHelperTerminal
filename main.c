/*
    This is a program to help me study in the university
    The idea is while attending lectures, write possible exam
    questions, its posible answers and the correct one. Storing 
    everything locally in files. 

    Finally, before the exam takes place, prompting random 
    questions to the user and its possible answers asking 
    the user to choose the right one.


*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define MAX_CHARACTER_SIZE 1024
#define MAX_NUMBER_QUESTIONS 50
#define MAX_POSSIBLE_ANSWERS 5
#define MAX_FILENAME_SIZE 255
#define MAX_PATH_SIZE 1024

struct answers{
    char ans_str[MAX_CHARACTER_SIZE];
};

struct question // Struct that stores our question object
{
    char question[MAX_CHARACTER_SIZE];
    struct answers ans[MAX_POSSIBLE_ANSWERS]; // Array of answers
    int index_correct_ans;

};

void print_menu(){
    printf("\n");
    printf("1: Create new question \n");
    printf("2: Prompt all stored questions and its answers \n");
    printf("3: Save questions to disk \n");
    printf("4: Load questions from save file \n");
    printf("9: Show this menu \n");
    printf("0: Exit program \n");
    printf("\n");

}

void initialize_seed(){
    time_t t;
    srand((unsigned) time(&t));
}

int random_num(int max_value){
    return (rand() % max_value);
}

void load_questions_to_program( struct question q_aux[], struct question q_global[], 
                                int q_index_aux, int *index_global){
    for(int i=0;i<q_index_aux;i++){
        strcpy(q_global[i].question, q_aux[i].question);
        for(int j = 0; j<MAX_POSSIBLE_ANSWERS;j++){
            strcpy(q_global[i].ans[j].ans_str, q_aux[i].ans[j].ans_str);
        }
    }
    *index_global = q_index_aux;
} 

void parse_line(char *l_buffer, char char_del, char *ans_line, int size){

    if((l_buffer[0]) == char_del && (l_buffer[1]) == char_del){
        for(int i = 2; i < size; i++){
            ans_line[i-2] = l_buffer[i];
        }
    }
    ans_line[strcspn(ans_line, "\n")] = 0;
}

/* Function that returns user_answer from stdin without '\n' character */
char* get_user_input(char* user_prompt, int size, char* user_answer){
    printf("%s", user_prompt);
    fgets(user_answer, size, stdin);
    user_answer[strcspn(user_answer, "\n")] = 0;
    return user_answer;
} 

void write_info_into_file (FILE *save_file, int index, struct question q[]){
    for(int i=0;i<index;i++){ // loop for all questions
        fprintf(save_file, "##%s",q[i].question); // '##' means the beginning of a new question

        for(int j=0;j<MAX_POSSIBLE_ANSWERS;j++){
            if(q[i].index_correct_ans == j){
                fprintf(save_file, "**%s", q[i].ans[j].ans_str); // '**' indicates this is the correct answer
            } else {
                fprintf(save_file, "%s", q[i].ans[j].ans_str);
            }
        }
        fprintf(save_file,"\n");
    }
}

int read_info_from_file(FILE *load_file, struct question q_global[],
                        int *index_global){
    struct question q_aux[MAX_NUMBER_QUESTIONS];
    int q_index_aux = 0;
    int ans_index = 0;
    char l_aux[MAX_CHARACTER_SIZE];
    char line_buffer[MAX_CHARACTER_SIZE];

    while (fgets(line_buffer, MAX_CHARACTER_SIZE, load_file) != NULL){
        // Reading question
        if((line_buffer[0]) == '#' && (line_buffer[1] == '#')){
            ans_index = 0;
            for (int i = 2;i<MAX_CHARACTER_SIZE;i++){
                l_aux[i - 2] = line_buffer[i];
                if (line_buffer[i] == '\0')
                    break;
            }
            strcpy(q_aux[q_index_aux].question, l_aux);
            continue;

        // Reading question answers
        } else if (line_buffer[0] != '\n'){
            if((line_buffer[0]) == '*' && (line_buffer[1] == '*')){
                for (int i = 2;i<MAX_CHARACTER_SIZE;i++){
                    l_aux[i - 2] = line_buffer[i];
                    if (line_buffer[i] == '\0')
                        break;
                }
                strcpy(q_aux[q_index_aux].ans[ans_index].ans_str, l_aux);
                q_aux[q_index_aux].index_correct_ans = ans_index;
                ans_index++;
                continue;
            } else {
                for(int i = 0; i<MAX_CHARACTER_SIZE;i++){
                    l_aux[i] = line_buffer[i];
                    if (line_buffer[i] == '\0')
                        break;               
                }
                strcpy(q_aux[q_index_aux].ans[ans_index].ans_str, l_aux);
                ans_index++;
            }
        } else {
            q_index_aux++;
        }
    }
    // Everything is stored in q_aux
    load_questions_to_program(q_aux,q_global,q_index_aux, index_global);
    fclose(load_file);
    return 1;

}

void create_question(struct question q[], int index){
    int right_ans_index;
    int control_input = 1;

    // Read user input
    fgets(q[index].question,MAX_CHARACTER_SIZE,stdin);
    for(int i = 0;i<MAX_POSSIBLE_ANSWERS;i++){
        printf("Write the possible answer with index %d : ", i);
        fgets(q[index].ans[i].ans_str,MAX_CHARACTER_SIZE,stdin);
    }
    printf("Which one is the correct answer? \n");
    while (control_input){
        printf("Write the index [0-4] of the correct answer: ");
        scanf("%d",&right_ans_index);
        getchar();
        if ((right_ans_index < 0) | (right_ans_index >4)){
            printf("Wrong number, choose one of the answers[0-4] \n");
        } else {
            q[index].index_correct_ans = right_ans_index;
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
                if(q[i].index_correct_ans == j){
                    printf("** Answer %d: %s", j, q[i].ans[j].ans_str);               
                } else{
                    printf("   Answer %d: %s", j, q[i].ans[j].ans_str);
                }
            }
            printf("\n");
        }
    }
}

int save_info_in_file(struct question q[], int index){
    FILE *save_file;
    char user_input[MAX_FILENAME_SIZE];
    char save_file_name[MAX_FILENAME_SIZE];
    char cwd[MAX_PATH_SIZE];
    char path_save_file [MAX_PATH_SIZE];
    int waiting_for_permission;

    if (index == 0){
        printf("There are not any questions to store to disk \n");
        return 0;
    } else {
        printf("Do you really want to save this session to disk? \n");
        get_user_input("[yes|no] or [y|n]: ", MAX_CHARACTER_SIZE, user_input);

        waiting_for_permission = 1;
        while(waiting_for_permission){
            if((strcmp(user_input, "no") == 0) || (strcmp(user_input, "n") == 0)){
                return 0;           

            } else if((strcmp(user_input, "yes") != 0 && (strcmp(user_input,"y") != 0))){
                get_user_input("Please answer [no|n] or [yes|y] to continue \n", MAX_CHARACTER_SIZE, user_input);
            
            } else {
                waiting_for_permission = 0;
            }
        }
        getcwd(cwd,sizeof(cwd)); // get current working directory
        strcpy(path_save_file,cwd);
        strcat(path_save_file,"/saves/");
        mkdir(path_save_file, 0700); // create 'saves' directory        

        get_user_input("Enter name of save file: \n", MAX_CHARACTER_SIZE, save_file_name);
        if (strstr(save_file_name,".txt") == NULL){
            strcat(save_file_name, ".txt");
        } 

        strcat(path_save_file,save_file_name);
        waiting_for_permission = 1;

        /* Check if save_file_path already exist */
        save_file = fopen(path_save_file, "r");
        if (save_file != NULL){
            printf("There is already a save file with the same name \n");
            printf("Do you want to replace and override its contents? \n");
            get_user_input("[yes|no] or [y|n]: ", MAX_CHARACTER_SIZE, user_input);

            // Filtering user_input to aceptable answers
            while (waiting_for_permission){
                if((strcmp(user_input, "no") == 0) || (strcmp(user_input, "n") == 0)){
                    fclose(save_file);
                    return 0;

                } else if ((strcmp(user_input, "yes") != 0 && (strcmp(user_input,"y") != 0))){
                    get_user_input("Please answer [no|n] or [yes|y] to continue \n", MAX_CHARACTER_SIZE, user_input);

                } else {
                    waiting_for_permission = 0;
                    fclose(save_file);
                    save_file = fopen(path_save_file,"w+"); // open save file with write permissions
                    write_info_into_file(save_file,index,q);
                    fclose(save_file);
                }
            }
        } else {
            save_file = fopen(path_save_file, "w+");
            write_info_into_file(save_file,index,q);
            fclose(save_file);

        }
    }
}

/*
    Gets questions info from save_file
    Fails if files doesn't exist or file doesn't follow the correct sintaxis
*/
int load_info_from_file(struct question q_global[], int *index_global){
    char file_name[MAX_FILENAME_SIZE];
    char path_load_file[MAX_PATH_SIZE];
    char cwd[MAX_PATH_SIZE];
    FILE *load_file;

    getcwd(cwd,sizeof(cwd)); // get current working directory
    strcpy(path_load_file,cwd);
    strcat(path_load_file,"/saves/");
    get_user_input("Enter name of save file you want to load from: ", MAX_CHARACTER_SIZE, file_name);
    printf("\n");
    if (strstr(file_name, ".txt") == NULL){ // Checks if file ends in .txt
        strcat(file_name, ".txt");
    }
    strcat(path_load_file,file_name); // Path to load file
    
    load_file = fopen(path_load_file, "r");
    if(load_file == NULL){
        printf("This file doesn't exist \n");
        return 0;
    } else {
        return read_info_from_file(load_file, q_global, index_global);
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
    int *p_index =  &index;

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

            case 3:
                save_info_in_file(q_global,index);
                break;
            
            case 4:
                if(load_info_from_file(q_global,p_index)){
                    printf("Success!\n");
                } else {
                    printf("Operation failed\n");
                }
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