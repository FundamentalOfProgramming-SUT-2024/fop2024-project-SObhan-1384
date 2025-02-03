#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "lsin.h"



void borderdesign(){
    for(int i = -40 ; i<41 ; i++){
        mvprintw(LINES/2 -7 , COLS/2 + i , "*");
        mvprintw(LINES/2 + 3 , COLS/2 + i , "*");
    }
    for(int i = -7 ; i<4 ; i++){
        mvprintw(LINES/2 + i , COLS/2 - 40 ,"*");
        mvprintw(LINES/2 + i , COLS/2 + 40 ,"*");
    }
}

int username_check(char * username){
    FILE *file = fopen("users.txt" , "r");
    char line[110];
    while(fgets(line , sizeof(line) , file)){
        char user[55];
        sscanf(line , "%s" , user);
        if(strcmp(user , username ) == 0){
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;

}

void username(char name[55]){   
    clear();

    while(1){
        start_color();
        attron(COLOR_PAIR(1));
        mvprintw(LINES/2 -3 , COLS/2 - 16,"Enter Your Username: ");
        attroff(COLOR_PAIR(1));
        init_color(2 , 220 , 208 , 255);
        init_pair(3 , 2 , COLOR_BLACK);
        attron(COLOR_PAIR(3));
        borderdesign();
        attroff(COLOR_PAIR(3));
        init_pair(2 , 2 , COLOR_YELLOW);
        attron(COLOR_PAIR(2));
        mvprintw(LINES/2 - 1 , COLS/2 - 35  , "*** You can only use letters, numbers and dashes in your username ***");
        attroff(COLOR_PAIR(2));
        echo();
        curs_set(TRUE);
        mvscanw(LINES/2 - 3 , COLS/2 + 5,"%s" , name);
        curs_set(FALSE);
        noecho();
        if(username_check(name) == 1){
            clear();
            attron(A_BLINK );
            mvprintw(LINES/2 - 5 , COLS/2 - 26 , "!*!*! This username has already been used !*!*!");
            attroff(A_BLINK);
            refresh();
        }
        else{
            break;
        }
        
    }
    refresh();
}

int password_check(char * password){
    int len = strlen(password);
    int valid = 0;
    for(int i = 0 ; i< len ; i++){
        if((password[i] > 64)&&(password[i] < 91)){
            valid +=1;
            break;
        }
    }
    for(int i = 0 ; i< len ; i++){
        if((password[i] > 96)&&(password[i] < 123)){
            valid +=1;
            break;
        }
    }
    for(int i = 0 ; i< len ; i++){
        if((password[i] > 47)&&(password[i] < 58)){
            valid +=1;
            break;
        }
    }
    if(valid == 3)
        return 1;
    return 0;
}

void password(char password[55]){

    clear();
    while(1){
        
        attron(COLOR_PAIR(1));
        mvprintw(LINES/2 -3 , COLS/2 - 18 , "Create a Password: ");
        attroff(COLOR_PAIR(1));
        init_color(2 , 220 , 208 , 255);
        init_pair(2 , 2 , COLOR_YELLOW);
        init_pair(3 , 2 , COLOR_BLACK);
        attron(COLOR_PAIR(3));
        borderdesign();
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(2));
        mvprintw(LINES/2 - 1 , COLS/2 - 35  , "*** Your password must contain lowercase and uppercase and numbers ***");
        mvprintw(LINES/2 , COLS/2 - 29  , "*** Enter the word random to get a password suggestion ***");
        attroff(COLOR_PAIR(2));
        
        echo();
        curs_set(TRUE);
        mvscanw(LINES/2 - 3 , COLS/2 + 1 ,"%s" , password);
        curs_set(FALSE);
        noecho();
        if((strlen(password) < 7)&&(strcmp(password , "random") != 0)){
            clear();
            attron(A_BLINK );
            mvprintw(LINES/2 - 5 , COLS/2 - 35 , "!*!*! The entered password must contain at least 7 characters !*!*!");
            attroff(A_BLINK);
            refresh();
        }
        else if(strcmp(password , "random") == 0){
            char * suggest = calloc(13 , sizeof(char));
            suggest[13] = '\0';
            int rand_capital = rand()%4 + 1;
            int rand_small = rand()%4 + 1;
            while(rand_capital > 0){
                int rand_capital_index = rand()%12;
                if(suggest[rand_capital_index] == 0){
                    suggest[rand_capital_index] = rand()%26 + 65;
                    rand_capital -= 1;
                }
            }
            while(rand_small > 0){
                int rand_small_index = rand()%12;
                if(suggest[rand_small_index] == 0){
                    suggest[rand_small_index] = rand()%26 + 97;
                    rand_small -= 1;
                }
            }

            for(int i = 0 ; i < 12 ; i++){
                if(suggest[i] == 0){
                    suggest[i] = rand()%10 + 48;
                }
            }
            clear();
            mvprintw( LINES/2 - 5 , COLS/2 - 16 , "Password suggestion: %s" , suggest);
            refresh();

        }
        else{
            if(password_check(password) == 0){
                clear();
                attron(A_BLINK );
                mvprintw(LINES/2 - 5 , COLS/2 - 39 , "!*!*! Your password must contain lowercase and uppercase and numbers !*!*!");
                attroff(A_BLINK);
                refresh();
            }
            else{
                break;
            }
        }
        
    }
}

int username_exist(char* username){
    FILE *file = fopen("users.txt" , "r");
    char line[110];
    while(fgets(line , 110 , file)){
        char user[55];
        sscanf(line , "%s" , user);
        if(strcmp(username , user) == 0){
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;

}

void sign_username(char name[55]){
    clear();

    while(1){
        start_color();
        attron(COLOR_PAIR(1));
        mvprintw(LINES/2 -3 , COLS/2 - 16,"Enter Your Username: ");
        attroff(COLOR_PAIR(1));
        init_color(2 , 220 , 208 , 255);
        init_pair(2 , 2 , COLOR_YELLOW);
        init_pair(3 , 2 , COLOR_BLACK);
        attron(COLOR_PAIR(3));
        borderdesign();
        attroff(COLOR_PAIR(3));
        echo();
        curs_set(TRUE);
        mvscanw(LINES/2 - 3 , COLS/2 + 5,"%s" , name);
        curs_set(FALSE);
        noecho();
        if (username_exist(name) == 0){
            clear();
            attron(A_BLINK );
            mvprintw(LINES/2 - 5 , COLS/2 - 23 , "!*!*! There is no user with this username !*!*!");
            attroff(A_BLINK);
            refresh();
        }
        else{
            break;
        }
    }
    refresh();
}

int password_exist(char *username , char *password){
    FILE *file = fopen("users.txt" , "r");
    char line[200];
    while(fgets(line , 200 , file)){
        char pass[55];
        char name[55];
        sscanf(line , "%s %s" , name , pass);
        if((strcmp(name , username) == 0) && (strcmp(pass , password) == 0)){
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void sign_password(char *username , char *password){
    clear();
    while(1){
        attron(COLOR_PAIR(1));
        mvprintw(LINES/2 -3 , COLS/2 - 18 , "Enter Your Password: ");
        mvprintw(LINES/2 - 1 , COLS/2 - 24 , "If you have forgottenn your password, type help");
        attroff(COLOR_PAIR(1));
        init_color(2 , 220 , 208 , 255);
        init_pair(2 , 2 , COLOR_YELLOW);
        init_pair(3 , 2 , COLOR_BLACK);
        attron(COLOR_PAIR(3));
        borderdesign();
        attroff(COLOR_PAIR(3));
        echo();
        curs_set(TRUE);
        mvscanw(LINES/2 - 3 , COLS/2 + 3,"%s" , password);
        curs_set(FALSE);
        noecho();
        if(strcmp("help" , password) == 0){
            FILE * file = fopen("users.txt" , "r");
            char line[200];
            while(fgets(line , 200 , file)){
                char user[55];
                char pass[55];
                sscanf(line , "%s %s" , user , pass);
                if(strcmp(user , username) == 0){
                    clear();
                    mvprintw(LINES/2 - 5 , COLS/2 - 14 , "Your password starts with %c%c" , pass[0] , pass[1]);
                    break;
                }
            }
        }
        else if(password_exist(username , password) == 0){
            clear();
            attron(A_BLINK );
            mvprintw(LINES/2 - 5 , COLS/2 - 23 , "!*!*! The password entered is incorrect !*!*!");
            attroff(A_BLINK);
            refresh();
        }
        else{
            break;
        }
    }
}
