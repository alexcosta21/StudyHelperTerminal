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

struct answers{ // Struct that stores our answers object
    char ans_str[MAX_CHARACTER_SIZE];
};

struct question // Struct that stores our question object
{
    char question[MAX_CHARACTER_SIZE];
    struct answers ans[MAX_POSSIBLE_ANSWERS]; // Array of answers
    int index_correct_ans;

};

/*
    Prints the programs menu
*/
void print_menu(){
    printf("\n");
    printf("1: Create new question \n");
    printf("2: Prompt all stored questions and its answers \n");
    printf("3: Save questions to disk \n");
    printf("4: Load questions from save file \n");
    printf("5: Enter exam mode \n");
    printf("9: Show this menu \n");
    printf("0: Exit program \n");
    printf("\n");

}

/*
    Initialize a new seed
*/
void initialize_seed(){
    time_t t;
    srand((unsigned) time(&t));
}

/*
    Returns a random generated number
    up to max value
*/
int get_random_num(int max_value){
    return (rand() % max_value);
}

/*
    Copies the parsed questions into 
    our global questions array
*/
void load_questions_to_program( struct question q_aux[], struct question q_global[], 
                                int q_index_aux, int *index_global){
    for(int i=0;i<=q_index_aux;i++){
        strcpy(q_global[i].question, q_aux[i].question);
        q_global[i].index_correct_ans = q_aux[i].index_correct_ans;
        for(int j = 0; j<MAX_POSSIBLE_ANSWERS;j++){
            strcpy(q_global[i].ans[j].ans_str, q_aux[i].ans[j].ans_str);
        }
    }
    *index_global = q_index_aux;
} 

/*
    Removes special characters used in save
    files format in order to load them
    into the program
*/
void parse_line(char *l_buffer, char char_del, char *ans_line, int size){

    if((l_buffer[0]) == char_del && (l_buffer[1]) == char_del){
        for(int i = 2; i < size; i++){
            ans_line[i-2] = l_buffer[i];
        }
    }
    ans_line[strcspn(ans_line, "\n")] = 0;
}

/* Function that returns user_answer from stdin without '\n' character */
char* get_user_input_string(char* user_prompt, int size, char* user_answer){
    printf("%s", user_prompt);
    fgets(user_answer, size, stdin);
    user_answer[strcspn(user_answer, "\n")] = 0;
    return user_answer;
}

int get_user_input_int(char* user_promt, int size){
    int answer;
    printf("%s\n", user_promt);
    scanf("%d",&answer);
    getchar();
    return answer;
}

/*
    Writes questions and its answers with the 
    correct format into a file
*/
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

/*
    Parses the file content to our question structure 
*/
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
    load_questions_to_program(q_aux,q_global,q_index_aux - 1, index_global);
    fclose(load_file);
    return 1;

}

/*
    Create a new question
*/
void create_question(struct question q[], int* index){
    int correct_ans_index;
    int control_input = 1;

    *index = *index + 1;
    // Read user input
    while(control_input){
        fgets(q[*index].question,MAX_CHARACTER_SIZE,stdin);
        if (!strcmp(q[*index].question, "\n")){
            printf("Please this field can't be empty, try again \n");
            printf("Write your question: ");
            } else {
                control_input = 0;
            }
    }

    for(int i = 0;i<MAX_POSSIBLE_ANSWERS;i++){
        control_input = 1;
        while(control_input){
            printf("Write the possible answer with index %d : ", i);
            fgets(q[*index].ans[i].ans_str,MAX_CHARACTER_SIZE,stdin);
            if (!strcmp(q[*index].ans[i].ans_str, "\n")){
                printf("Please this field can't be empty, try again \n");
            } else {
                control_input = 0;
            }
        }
    }
    control_input = 1;
    printf("Which one is the correct answer? \n");
    while (control_input){
        printf("Write the index [0-4] of the correct answer: ");
        scanf("%d",&correct_ans_index);
        getchar();
        if ((correct_ans_index < 0) | (correct_ans_index >4)){
            printf("Wrong number, choose one of the answers[0-4] \n");
        } else {
            q[*index].index_correct_ans = correct_ans_index;
            control_input = 0;
        }
    }
}

/*
Displays all questions and its answers that are currently stored
*/
void prompt_questions(struct question q[], int index){
    if(index == -1){
        printf("The program doesn't have any questions yet \n");

    } else {
        printf("Showing all stored questions: \n \n");
        for(int i=0;i<=index;i++){
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

/*
    Saves local questions to a save file 
*/
int save_info_in_file(struct question q[], int index){
    FILE *save_file;
    char user_input[MAX_FILENAME_SIZE];
    char save_file_name[MAX_FILENAME_SIZE];
    char cwd[MAX_PATH_SIZE];
    char path_save_file [MAX_PATH_SIZE];
    int waiting_for_permission;

    if (index == -1){
        printf("The program doesn't have any questions to store to disk\n");
        return 0;
    } else {
        printf("Do you really want to save this session to disk? \n");
        get_user_input_string("[yes|no] or [y|n]: ", MAX_CHARACTER_SIZE, user_input);

        waiting_for_permission = 1;
        while(waiting_for_permission){
            if((strcmp(user_input, "no") == 0) || (strcmp(user_input, "n") == 0)){
                return 0;           

            } else if((strcmp(user_input, "yes") != 0 && (strcmp(user_input,"y") != 0))){
                get_user_input_string("Please answer [no|n] or [yes|y] to continue \n", MAX_CHARACTER_SIZE, user_input);
            
            } else {
                waiting_for_permission = 0;
            }
        }
        getcwd(cwd,sizeof(cwd)); // get current working directory
        strcpy(path_save_file,cwd);
        strcat(path_save_file,"/saves/");
        mkdir(path_save_file, 0700); // create 'saves' directory        

        get_user_input_string("Enter name of save file: ", MAX_CHARACTER_SIZE, save_file_name);
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
            get_user_input_string("[yes|no] or [y|n]: ", MAX_CHARACTER_SIZE, user_input);

            // Filtering user_input to aceptable answers
            while (waiting_for_permission){
                if((strcmp(user_input, "no") == 0) || (strcmp(user_input, "n") == 0)){
                    fclose(save_file);
                    return 0;

                } else if ((strcmp(user_input, "yes") != 0 && (strcmp(user_input,"y") != 0))){
                    get_user_input_string("Please answer [no|n] or [yes|y] to continue \n", MAX_CHARACTER_SIZE, user_input);

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
    printf("File saved!\n");
    return 1;
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
    get_user_input_string("Enter the filename to load from: ", MAX_CHARACTER_SIZE, file_name);
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
    Checks in the randomly generated number
    had already been used
*/
 int is_index_used(int used_indexes[], int index){
    int is_used = 0;

    if (used_indexes[index] == 1 )
        return is_used = 1;
    return is_used;
 }

/*
    Empties used index array
*/
 void empty_used_index_array(int used_indexes[], int size){
    for(int i=0;i<size;i++){
        used_indexes[i] = 0;
    }
 }

/*
    Checks if the user answers are correct 
    Prints failed questions signalling the correct answer
*/
 void check_user_answers(struct question u_answer_to_check[], int num_questions, struct question q_global[]){
    int correct_answers = 0;
    int grade;

    for(int i = 0;i < num_questions;i++){
        if(q_global[i].index_correct_ans == u_answer_to_check[i].index_correct_ans){
            correct_answers++;
        } else {
            printf("You've failed this question: ");
            printf("%s\n", q_global[i].question); 
                for (int j = 0; j<MAX_POSSIBLE_ANSWERS;j++){
                    if (j == u_answer_to_check[i].index_correct_ans){
                        printf("--Your Choice-- Answer[%d]: %s", j, q_global[i].ans[j].ans_str);
                    } else if(j == q_global[i].index_correct_ans) {
                        printf("----Correct---- Answer[%d]: %s", j, q_global[i].ans[j].ans_str);
                    } else {
                        printf("                Answer[%d]: %s", j, q_global[i].ans[j].ans_str);
                    }
                }
                printf("\n");
        }
    }
    grade = (correct_answers * 10) / num_questions;
    printf("This are your results: \n");
    printf("You've got %d questions right from a total of %d questions \n", correct_answers, num_questions);
    printf("Your grade is: %d\n\n", grade);
 }

/*
    Prompts questions randomly for the user
    to answer, in the the user gets a grade
*/
int exam_mode(struct question q_global[], int index_global){
    int u_answer_to_check;
    int control_input = 1;
    int rand_index = 0;
    int num_total_q = index_global + 1; 
    int used_indexes[num_total_q]; // It stores the random indexes we have already used
    struct question user_answered_q[num_total_q]; // It stores the index of the question and

    printf("Entering exam mode\n");
    // Check if the program contains any questions
    if(num_total_q == -1){
        printf("The program doesn't have any questions yet\n");
        printf("Please write some questions using option 1 ");
        printf("or use option 4 to load questions from a save file \n");
        printf("Exiting exam mode \n");
        return 0;
    } else{
        empty_used_index_array(used_indexes, num_total_q);
        initialize_seed();
        while (num_total_q > -1){
            rand_index = get_random_num(index_global + 1);
            if (is_index_used(used_indexes, rand_index)){ // get rand number again
                if (num_total_q == 0)
                    break;
                continue;
            } else {
                // Prompt random question and its answers to user
                printf("Question: %s\n", q_global[rand_index].question); 
                for (int i = 0; i<MAX_POSSIBLE_ANSWERS;i++){
                    printf("  Answer %d: ",i);
                    printf("%s", q_global[rand_index].ans[i].ans_str);
                }
                printf("\n");
                used_indexes[rand_index] = 1;
                num_total_q--;
            
            // Get user input
            /*
                Check if the answer is right
                Keep track of the user grade, store the question index for later
                View final grade at the end, show back the failed questions
            */
                control_input = 1;
                while(control_input){
                    printf("What's your answer? [0-4] : ");
                    scanf("%d", &u_answer_to_check);
                    getchar();
                    if((u_answer_to_check < 0) | (u_answer_to_check > 4)){
                        printf("Wrong index please try again \n");
                        continue;
                    }
                    printf("\n");
                    user_answered_q[rand_index].index_correct_ans = u_answer_to_check;
                    control_input = 0;
                }
            }           
        }
        num_total_q = index_global + 1;
        check_user_answers(user_answered_q,num_total_q,q_global);
        return 1;
    }
 }

/*
    Warns user to save before loading from file
*/
 int ask_user_save_bf_loading(){
    char user_input[MAX_CHARACTER_SIZE];
    int control_input = 1;

    printf("----WARNING----\n");
    printf("If you proceed, you'll lose your work\n");
    printf("Do you want to save your current questions before loading?\n");
    get_user_input_string("[yes|no] or [y|n]: ",MAX_CHARACTER_SIZE,user_input);

    while(control_input){
        if((strcmp(user_input, "no") == 0) || (strcmp(user_input, "n") == 0)){
            return 0;

        } else if ((strcmp(user_input, "yes") != 0 && (strcmp(user_input,"y") != 0))){
            get_user_input_string("Please answer [no|n] or [yes|y] to continue \n", MAX_CHARACTER_SIZE, user_input);

        } else {
            control_input = 0;
        }
    }
    return 1;
 }

 void test_rand(){
    int max_num;
    initialize_seed();
    printf("Write max num for rand generation: ");
    scanf("%d",&max_num);
    printf("%d \n",get_random_num(max_num));
 }

/*
    We need a global array of all questions structures

    Every index should point to a question structure that will
    have access to que actual question and the array of answers
    including the correct answer index too.

*/
int main(){
    struct question q_global[MAX_NUMBER_QUESTIONS];
    int is_program_running = 1;
    int menu_selection;
    int index = -1; // initial non valid value
    int *p_index =  &index;
    int is_content_modified = 0;

    printf("Welcome! \n");
    print_menu();
    while(is_program_running){
        printf("Choose one option: ");
        scanf("%d", &menu_selection);
        getchar(); // This consumes the '\n' char entered by the user after clicking 'ENTER'   
        switch (menu_selection){
            case 0:
                printf("Bye!\n");
                is_program_running = 0;
                break;
            
            case 1:
                printf("Write your question: ");
                create_question(q_global,p_index);
                is_content_modified = 1;
                break;

            case 2:
                prompt_questions(q_global,index);
                break;

            case 3:
                if(save_info_in_file(q_global,index)){
                    is_content_modified = 0;
                }
                break;
            
            case 4:
                if(is_content_modified){
                    if(ask_user_save_bf_loading()){
                        save_info_in_file(q_global,index);
                    }
                }
                if(load_info_from_file(q_global,p_index)){
                    printf("Success!\n");
                    is_content_modified = 0;
                } else {
                    printf("Operation failed\n");
                }
                break;

            case 5:
                exam_mode(q_global, index);
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