# SimpleStudyHelper
It is a simple program where you can input questions attached with several
possible answers. 

Once you finish adding questions you can run 'exam' mode. The program will
show each of your questions in a random order and you'll have to choose the right answer.

The idea is help you study before the exams, you can ask your professors some questions, write them here and do tests whenever you want. 

## Installation
- Clone the repository `git clone git@github.com:alexcosta21/StudyHelperTerminal.git`
- Open a terminal
- Execute `gcc main.c -o main`  

## Usage 
1. Run the app with `./main`, it'll show you the menu with all the available options.

2. Select what you want to do from the menu.
    - Select '1' to create a new question.
    - Select '2' to visualize all loaded questions from this session.
    - Select '3' to save this session to a save file.
    - Select '4' to load questions from save file.
    - Select '5' to enter 'exam mode' to do a test with your own added questions and get a grade.
    - Select '6' to remove one question from the program session.
    - Select '9' to view the menu again
3. Select '0' to exit the program

## Future improvements
In future commits, I am going to add more functionalities like loading from a save file directly from the main program arguments.