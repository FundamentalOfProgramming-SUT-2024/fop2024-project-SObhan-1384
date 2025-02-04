#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "lsin.h"
#include <locale.h>
#include <regex.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


//S for spell rooms
//T for treasure rooms
//agar treasure = 1 marhale akhar. agar 0 marhale aval.
//x = tale haye otag treasure;
//X = tale haye digar otagh ha;

// \U00002692 gorz
// \U0001F5E1 khanjar
// \U0001FA84 asaa
// \U00002694 shamshir
// \U000027B3 tir


typedef struct
{
    int x , y;
}loc;

typedef struct doshman{
    loc jash;
    char model;
    int joon;
    int marhale;
}doshman;

typedef struct{
    char name[55];//
    loc player;//
    int enemy_num;//
    doshman* enemy;//
    loc homes[6][2];//
    char* map;//
    char* under_map;//
    char* show_map;//
    time_t sec_5; //*
    time_t sec_1; //*
    time_t start_telesm; //*
    int telesmha[3];//
    int selah[6];//
    int ghazaha[4];//
    int current_selah;//
    int current_telesm;//  
    int hunger;//
    int *mamooli;//
    int *alaa;//
    int *jadooyi;//
    int health;//
    int point;//
    int golds;//
    int cheat;//*
    int levels;//
    int color;//
    int song;//
    int end;//*
    time_t start_payam;//*
    int payam;//*
    int tabaghe;
    int playing;
}Game;

void start_game(Game * g);
void second_menu(Game * rogue );
void settings(Game * g);
void music(Game * g);
void color(Game *g);
void dif(Game *g);
void handle_show(Game * g , loc a);
void handle_enemy(Game * g);
void win_page(Game * g);
void fight_room(Game *g);
void draw_health(Game *g);
void selah_menu(Game *g);
void telesm_menu(Game * g);
void food_menu(Game * g);  
void saving(Game * g); 
void resume_game(Game * g);
int finding_home(Game * g , loc a);
void handle_weapon(Game * g);
void printf_payam(Game * g);
void lose_page(Game *g);
void treasure_room(Game * g);
void profile(Game * g);


Mix_Music * moosighi;

void play_music(Game * g){
    Mix_OpenAudio (44100 , MIX_DEFAULT_FORMAT , 2 , 2048);
    if(g->playing == 1){
        if(g->song == 0){
            moosighi = Mix_LoadMUS("sacrifice.mp3");
        }
        if(g->song == 1){
            moosighi = Mix_LoadMUS("carryonwaywardson.mp3");
        }
        if(g->song == 2){
            moosighi = Mix_LoadMUS("mar.mp3");
        }
        Mix_PlayMusic(moosighi , -1);
    }
    else{
        Mix_PlayMusic(moosighi , -1);
        Mix_HaltMusic();
    }

}

int loc_to_int(loc a){
    return a.x + (COLS)* a.y;
}

int email_validation(char *email){
    const char* pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
    regex_t regex;
    if(regcomp(&regex, pattern , REG_EXTENDED) != 0)
        return 0;
    int result = regexec(&regex , email , 0 , NULL , 0);
    if(result == 0)
        return 1;
    return 0;
}

void email_input(char eml[55]){
    clear();
    while(1){
        
        attron(COLOR_PAIR(1));
        mvprintw(LINES/2 -3 , COLS/2 - 18, "Enter Your Email: ");
        attroff(COLOR_PAIR(1));
        init_color(2 , 220 , 208 , 255);
        init_pair(2 , 2 , COLOR_YELLOW);
        init_pair(3 , 2 , COLOR_BLACK);
        attron(COLOR_PAIR(3));
        borderdesign();
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(2));
        mvprintw(LINES/2 - 1 , COLS/2 - 30 , "*** Receive Emails to provide you with a better service ***");
        attroff(COLOR_PAIR(2));
        
        echo();
        curs_set(TRUE);
        mvscanw(LINES/2 - 3 , COLS/2  ,"%s" , eml);
        curs_set(FALSE);
        noecho();
        if( email_validation(eml)== 0){
            clear();
            attron(A_BLINK);
            mvprintw(LINES/2 - 5 , COLS/2 - 20 , "!*!*! The email entered is invalid !*!*!");
            attroff(A_BLINK);
            refresh();
        }
        else{
            break;
        }
        
    }
}

void log_in(Game *g){
    char esm[55];
    char pass[55];
    char eml[55];
    username(esm);
    password(pass);
    email_input(eml);

    FILE *file = fopen("users.txt" , "a");
    //name , password , email , point , gold , experience;
    fprintf(file , "%s %s %s %d %d\n" , esm , pass , eml , 0 , 0);
    fclose(file);

    for(int i = 0 ; i<=strlen(esm) ; i++){
        g->name[i] = esm[i];
    }

    char *filename = calloc(60 , sizeof(char));
    for(int i = 0 ; i < strlen(g->name) ; i++)
        filename[i] = g->name[i];
    
    filename[strlen(g->name)] = '.';
    filename[strlen(g->name) + 1] = 't';
    filename[strlen(g->name) + 2] = 'x';
    filename[strlen(g->name) + 3] = 't';
    filename[strlen(g->name) + 4] = '\0';

    FILE * file_avalie = fopen(filename , "w");
    fprintf(file_avalie , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    fclose(file_avalie);

    clear();
    refresh();  
}

void sign_in(Game *g){
    char username[55];
    char password[55];
    sign_username(username);
    sign_password(username , password);
    for(int i = 0 ; i<=strlen(username) ; i++){
        g->name[i] = username[i];
    }
    g->name[strlen(username)] = '\0';
    clear();
    refresh();
}

int first_menu(Game *g){
    start_color();
    init_pair(1 , COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    const char *firstmenu[3]= {"Sign in" , "Log in" , "Guest"};
    int choice = 0;
    while(1){
        clear();
        mvprintw(LINES/2 - 10 , COLS/2 - 17 , " ____    ___    ____  _   _  _____");
        mvprintw(LINES/2 - 9 , COLS/2 - 17 , "|  _ \\  / _ \\  / ___|| | | || ____|");
        mvprintw(LINES/2 - 8 , COLS/2 - 17 , "| |_) || | | || |  _ | | | ||  _|  ");
        mvprintw(LINES/2 - 7 , COLS/2 - 17 , "|  _ < | |_| || |_| || |_| || |___ ");
        mvprintw(LINES/2 - 6 , COLS/2 - 17 , "|_| \\_\\ \\___/  \\____| \\___/ |_____|");
        for(int i = 0 ; i<3 ; i++){
            if(i == choice)
                attron(A_REVERSE);
            mvprintw(LINES/2 -3 + i , COLS/2 -4 , "%s" , firstmenu[i]);
            if(i == choice)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 2 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    if(choice == 1){
        sign_in(g);
    }
    if(choice == 0)
        log_in(g);
    if(choice == 2)
        g->name[0] = '\0';
    
    refresh();
}

void music(Game *g){
    char *song[3]= {"Sacrifice, The Weeknd" , "Carry on Wayward Son, Kansas" , "Mohreye Mar, Saeed asayesh"};
    init_pair(1 , COLOR_MAGENTA, COLOR_BLACK);
    init_pair(3 , COLOR_BLACK , COLOR_YELLOW);
    noecho();
    curs_set(FALSE);
    int choice = 0;
    int final_choice = g->song;
    while(1){
        attron(COLOR_PAIR(1));
        clear();
        for(int i = 0 ; i <3 ; i ++){
            if(i == choice)
                attron(A_REVERSE);
            mvprintw(LINES/2 -3 + i , COLS/2 -10 ,"%s" , song[i]);
            if(i == choice)
                attroff(A_REVERSE);
        }
        if(g->playing == 0){
            mvprintw(LINES/2 , COLS/2 - 5, "Music: OFF");
        }
        if(g->playing == 1){
            mvprintw(LINES/2 , COLS/2 - 5, "Music: ON");
        }
        init_pair(2 , COLOR_BLACK , COLOR_YELLOW);
        attron(COLOR_PAIR(2));
        mvprintw(LINES/2 - 5 , COLS/2 - 10 , "%s" , song[final_choice]);
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));
        mvprintw(LINES/2 - 7 , COLS/2 - 11 , "*** Pess 'q' to exit ***");
        attroff(COLOR_PAIR(3));
        int a = getch();
        if(a == 'q'){
            settings(g);
            break;
        }
        if((a == KEY_UP))
            choice = (choice + 2)%3;
        if((a == KEY_DOWN))
            choice = (choice + 1)%3;
        if(a == 'x'){
            g->playing = (g->playing +1)%2;
        }
        if(a == '\n'){
            final_choice = choice;
            g->song = final_choice;
        }
    }
}

void color(Game *g){
    char *colors[5]= {"WHITE" , "RED" , "GREEN" , "YELLOW" , "BLUE"};
    init_pair(1 , COLOR_MAGENTA, COLOR_BLACK);
    noecho();
    curs_set(FALSE);
    init_pair(3 , COLOR_BLACK , COLOR_YELLOW);
    int choice = 0;
    int final_choice = g->color;
    while(1){
        clear();
        attron(COLOR_PAIR(1));
        for(int i = 0 ; i <5 ; i ++){
            if(i == choice)
                attron(A_REVERSE);
            mvprintw(LINES/2 -3 + i , COLS/2 -3 ,"%s" , colors[i]);
            if(i == choice)
                attroff(A_REVERSE);
        }
        init_pair(2 , COLOR_BLACK , COLOR_WHITE);
        attron(COLOR_PAIR(2));
        mvprintw(LINES/2 - 5 , COLS/2 - 3 , "%s" , colors[final_choice]);
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));
        mvprintw(LINES/2 - 7 , COLS/2 - 13 , "*** Pess 'q' to exit ***");
        attroff(COLOR_PAIR(3));
        int a = getch();
        if(a == 'q'){
            settings(g);
            break;
        }
        if(a == KEY_UP)
            choice = (choice + 4)%5;
        else if(a == KEY_DOWN)
            choice = (choice + 1)%5;
        else if(a == '\n'){
            final_choice = choice;
            g->color = final_choice;
        }
        refresh();
    }
}

void dif(Game *g){
    char *difs[3]= {"Easy" , "Medium" , "Hard"};
    init_pair(1 , COLOR_MAGENTA, COLOR_BLACK);
    noecho();
    curs_set(FALSE);
    init_pair(3 , COLOR_BLACK , COLOR_YELLOW);
    int choice = 0;
    int final_choice = g->levels;
    while(1){
        clear();
        attron(COLOR_PAIR(1));
        for(int i = 0 ; i <3 ; i ++){
            if(i == choice)
                attron(A_REVERSE);
            mvprintw(LINES/2 -3 + i , COLS/2 -2 ,"%s" , difs[i]);
            if(i == choice)
                attroff(A_REVERSE);
        }
        init_pair(2 , COLOR_BLACK , COLOR_WHITE);
        attron(COLOR_PAIR(2));
        mvprintw(LINES/2 - 5 , COLS/2 - 3 , "%s" , difs[final_choice]);
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));
        mvprintw(LINES/2 - 7 , COLS/2 - 13 , "*** Pess 'q' to exit ***");
        attroff(COLOR_PAIR(3));
        int a = getch();
        if(a == 'q'){
            settings(g);
            break;
        }
        if(a == KEY_UP)
            choice = (choice + 2)%3;
        else if(a == KEY_DOWN)
            choice = (choice + 1)%3;
        else if(a == '\n'){
            final_choice = choice;
            g->levels = final_choice;
        }
        refresh();
    }
}

void settings(Game *g){
    clear();
    start_color();
    init_pair(1 , COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    char *set[3] = {"Difficulity" , "Color" , "Song"};
    init_pair(2 , COLOR_BLACK , COLOR_YELLOW);
    attron(COLOR_PAIR(2));
    mvprintw(LINES/2 - 5 , COLS/2 - 13 , "*** Pess 'q' to exit ***");
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(1));
    keypad(stdscr , TRUE);
    int choice_set = 0;
    while(1){
        for(int i = 0 ; i < 3 ; i++){
            if(i == choice_set)
                attron(A_REVERSE);
            mvprintw(LINES/2 -3 + i , COLS/2 -4 ,"%s" , set[i]);
            if(i == choice_set)
                attroff(A_REVERSE);
        }
        int a = getch();
        if(a == 'q'){
            choice_set = -1;
            second_menu(g);
            break;
        }
        if(a == KEY_UP)
            choice_set = (choice_set + 2)%3;
        if(a == KEY_DOWN)
            choice_set = (choice_set + 1)%3;
        if(a == '\n'){
            break;
        }
    }
    if(choice_set == 2)
        music(g);
    else if(choice_set == 1)
        color(g);
    else if(choice_set == 0)
        dif(g);
    refresh();
}

void scoreboard(Game *g){
    char **names = malloc(1000 * sizeof(char *));
    int scores[1000];
    int golds[1000];
    char pass[55];
    char email[55];

    int line_counter = 0;

    FILE * file = fopen("users.txt" , "r");
    char line[200];
    while(fgets(line , 200 , file)){
        char *esm = malloc(55 * sizeof(char));
        sscanf(line , "%s %s %s %d %d" , esm , pass , email , &scores[line_counter] , &golds[line_counter]);
        names[line_counter] = esm;
        line_counter +=1 ;
    }

    fclose(file);

    clear();

    int new_line_counter = 0;
    char **new_names = malloc(200 * sizeof(char *));
    int experience[200];
    int new_scores[200];
    int new_golds[200];

    for(int i = 0 ; i < line_counter ; i++){
        int valid = 1;
        for(int j = 0 ; j < new_line_counter ; j++){
            if(strcmp(names[i],new_names[j]) == 0){
                new_scores[j] += scores[i];
                new_golds[j] += golds[i];
                experience[j] += 1;
                valid = 0;
            }
        }
        if(valid == 1){
            new_names[new_line_counter] = names[i];
            new_scores[new_line_counter] = scores[i];
            new_golds[new_line_counter] = golds[i];
            experience[new_line_counter] = 0;
            new_line_counter += 1;
        }
    }

    for(int i = 0 ; i < new_line_counter ; i++){
        for(int j = 0 ; j < i ; j++){
            if(new_scores[i] > new_scores[j]){
                int komak = new_scores[i];
                new_scores[i] = new_scores[j];
                new_scores[j] = komak;

                komak = new_golds[i];
                new_golds[i] = new_golds[j];
                new_golds[j] = komak;

                
                komak = experience[i];
                experience[i] = experience[j];
                experience[j] = komak;

                char *nam = malloc(55 * sizeof(char));
                nam = new_names[i];
                new_names[i] = new_names[j];
                new_names[j] = nam;
            }
        }
    }

    clear();
    while(1){
        init_pair(1 , COLOR_MAGENTA , COLOR_BLACK);
        attron(COLOR_PAIR(1));
        mvprintw(3 , COLS/2 - 49 , "USERNAME                                         SCORES |GOLDS  |EXPERIENCE ");
        attroff(COLOR_PAIR(1));
        int rotbe = 1;
        init_color(23 , 500 , 400 , 20);
        init_color(21 , 187 , 194 , 204);
        init_color(22 , 205 , 127 , 50);
        init_pair(30 , 23 , COLOR_BLACK);
        init_pair(31 , 21 , COLOR_BLACK);
        init_pair(32 , 22 , COLOR_BLACK);
        for(int i = 0 ; i < new_line_counter ; i++){
            if(strcmp(g->name , new_names[i]) == 0)
                attron(A_BLINK | A_ITALIC);
            
            if(rotbe == 1){
                attron(COLOR_PAIR(30));
                mvprintw(5 + i , COLS/2 - 63 , "%s LEGENDRY   %s" , "\U0001F947", new_names[i]);
                mvprintw(5+ i , COLS/2 , "%d" ,  new_scores[i]);
                mvprintw(5 + i , COLS/2 + 8 , "%d" , new_golds[i]);
                mvprintw(5 + i , COLS/2 + 16 , "%d" , experience[i]);
                attroff(COLOR_PAIR(30));
            }
            if(rotbe == 2){
                attron(COLOR_PAIR(31));
                mvprintw(5 + i , COLS/2 - 67 , "%s GRAND MASTER   %s" , "\U0001F948", new_names[i]);
                mvprintw(5+ i , COLS/2 , "%d" ,  new_scores[i]);
                mvprintw(5 + i , COLS/2 + 8 , "%d" , new_golds[i]);
                mvprintw(5 + i , COLS/2 + 16 , "%d" , experience[i]);
                attroff(COLOR_PAIR(31));
            }
            if(rotbe == 3){
                attron(COLOR_PAIR(32));
                mvprintw(5 + i , COLS/2 - 61 , "%s MASTER   %s" , "\U0001F949", new_names[i]);
                mvprintw(5+ i , COLS/2 , "%d" ,  new_scores[i]);
                mvprintw(5 + i , COLS/2 + 8 , "%d" , new_golds[i]);
                mvprintw(5 + i , COLS/2 + 16 , "%d" , experience[i]);
                attroff(COLOR_PAIR(32));
            }
            if(rotbe > 3)
                mvprintw(5 + i , COLS/2 - 49 , "%s" , new_names[i]);
                mvprintw(5+ i , COLS/2 , "%d" ,  new_scores[i]);
                mvprintw(5 + i , COLS/2 + 8 , "%d" , new_golds[i]);
                mvprintw(5 + i , COLS/2 + 16 , "%d" , experience[i]);
            
            if(strcmp(g->name , new_names[i]) == 0)
                attroff(A_BLINK | A_ITALIC);
            
            if((new_scores[i] > new_scores[i + 1]))
                rotbe += 1;
        }
        char a = getch();
        if(a == 'q')
            break;
    }
    second_menu(g);

}

void second_menu(Game *rogue){
    Mix_HaltMusic();
    clear();
    start_color();
    init_pair(1 , COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    const char* secondmenu[5] = {"New game" , "Previous game" ,  "Score Board" , "Profile" , "Settings"};
    int choice = 0;
    noecho();
    while(1){
        clear();
        mvprintw(LINES/2 - 10 , COLS/2 - 17 , " ____    ___    ____  _   _  _____");
        mvprintw(LINES/2 - 9 , COLS/2 - 17 , "|  _ \\  / _ \\  / ___|| | | || ____|");
        mvprintw(LINES/2 - 8 , COLS/2 - 17 , "| |_) || | | || |  _ | | | ||  _|  ");
        mvprintw(LINES/2 - 7 , COLS/2 - 17 , "|  _ < | |_| || |_| || |_| || |___ ");
        mvprintw(LINES/2 - 6 , COLS/2 - 17 , "|_| \\_\\ \\___/  \\____| \\___/ |_____|");
        for(int i = 0 ; i < 5 ; i++){
            if(i == choice)
                attron(A_REVERSE);
            mvprintw(LINES/2 - 3 + i, COLS/2 -4 , "%s" , secondmenu[i]);
            if(i == choice)
                attroff(A_REVERSE);
        }
        int a = getch();
        if(a == '\n')
            break;       
        if(a == KEY_UP)
            choice = (choice + 4)%5;
        if(a == KEY_DOWN)
            choice = (choice + 1)%5;
    }
    if(choice == 4){
        settings(rogue);
    }
    if(choice == 3){
        profile(rogue);
    }
    if(choice == 1)
        resume_game(rogue);
    if(choice == 2)
        scoreboard(rogue);
    if(choice == 0){
        rogue->end = 0;
        rogue->tabaghe = 0;
        start_game(rogue);
    }
    
    refresh();


}

loc random_door_up(loc a , loc b , Game *g){
    int m = a.x + 1;
    int n = b.x - 1;
    loc out;
    out.x = rand()%(n - m + 1) + m;
    out.y = a.y;
    g->map[loc_to_int(out)] = '+';
    out.y -=1;
    return out;
}

loc random_door_down(loc a , loc b , Game *g){
    int m = a.x + 1;
    int n = b.x - 1;
    loc out;
    out.x = rand()%(n - m + 1) + m;
    out.y = b.y;
    g->map[loc_to_int(out)] = '+';
    out.y +=1;
    return out;
}

loc random_door_right(loc a , loc b , Game *g){
    int m = a.y + 1;
    int n = b.y - 1;
    loc out;
    out.y = rand()%(n - m + 1) + m;
    out.x = b.x;
    g->map[loc_to_int(out)] = '+';
    out.x +=1;
    return out;
}

loc random_door_left(loc a , loc b , Game *g){
    int m = a.y + 1;
    int n = b.y - 1;
    loc out;
    out.y = rand()%(n - m + 1) + m;
    out.x = a.x;
    g->map[loc_to_int(out)] = '+';
    out.x -=1;
    return out;
}

void random_way(loc a , loc b , Game* g){

    int index_x;
    int index_y;
    if(a.x >= b.x)
        index_x = -1;
    if(a.x < b.x)
        index_x = 1;
    if(a.y >= b.y)
        index_y = -1;
    if(a.y < b.y)
        index_y = 1;
    int ops_x = (b.x - a.x)*index_x;
    int ops_y = (b.y - a.y)*index_y;
    int ops = ops_x + ops_y;
    int location = loc_to_int(a);
    g->map[location] = '#';
    for(int i = 1 ; i<=ops ; i++){
        int move = rand()%2;
        if(((move == 1)&&(ops_x > 0))||(ops_y == 0)){
            location += index_x;
            g->map[location] = '#';
            ops_x -=1;
        }
        else{
            location += index_y*(COLS);
            g->map[location] = '#';
            ops_y -= 1;
        }
    }
}

void random_map(Game *g , int spell , int treasure){
    //random rooms
    loc home1[6];
    loc home2[6];
    for(int i = 0 ; i< 6 ; i++){
        home1[i].y = 3 + rand()%(LINES/2 - 14) + (i/3)*(LINES/2);
        home1[i].x = 6 + rand()%(COLS/3 - 16) + (i%3)*(COLS/3);
        home2[i].y = rand()%(LINES/2  - (home1[i].y)%(LINES/2) -10) + (home1[i].y + 5);
        home2[i].x = rand()%(COLS/3 - (home1[i].x)%(COLS/3) -10) + (home1[i].x + 5);
        g->homes[i][0] = home1[i];
        g->homes[i][1] = home2[i];
    }


    for(int i = 0 ; i<6 ; i++){
        for(int j = home1[i].x + 1 ; j< home2[i].x ; j++){
            for(int k = home1[i].y + 1 ; k < home2[i].y ; k++){
                g->map[j + k*(COLS)] = '.';
            }
        }
    }

    for(int j = 0 ; j<6 ; j++){
        for(int i = home1[j].x ; i <= home2[j].x ; i++){
            g->map[(home1[j].y)*(COLS) + i] = '_';
            g->map[(home2[j].y)*(COLS) + i] = '_';
        }
    }
    for(int j = 0 ; j<6 ; j++){
        for(int i = home1[j].y ; i <= home2[j].y ; i++){
            g->map[i*(COLS) + home1[j].x] = '|';
            g->map[i*(COLS) + home2[j].x] = '|';
        }
    }

    //random ways
    if (spell==0){
        int random = rand()%6;
        if(random == 0){
            random_way(random_door_down(home1[1] , home2[1] , g) , random_door_up(home1[4] , home2[4] , g) , g);
            random_way(random_door_right(home1[1] , home2[1] , g) , random_door_left(home1[2] , home2[2] , g) , g);
            random_way(random_door_left(home1[1] , home2[1] , g) , random_door_right(home1[0] , home2[0] , g) , g);
            random_way(random_door_left(home1[4] , home2[4] , g) , random_door_right(home1[3] , home2[3] , g) , g);
            random_way(random_door_right(home1[4] , home2[4] , g) , random_door_left(home1[5] , home2[5] , g) , g);
        }

        else if(random == 1){
            random_way(random_door_right(home1[1] , home2[1] , g) , random_door_left(home1[2] , home2[2] , g) , g);
            random_way(random_door_left(home1[1] , home2[1] , g) , random_door_right(home1[0] , home2[0] , g) , g);
            random_way(random_door_down(home1[1] , home2[1] , g) , random_door_up(home1[4] , home2[4] , g) , g);
            random_way(random_door_left(home1[4] , home2[4] , g) , random_door_right(home1[3] , home2[3] , g) , g);
            random_way(random_door_down(home1[2] , home2[2] , g) , random_door_up(home1[5] , home2[5] , g) , g);
        }
        else if(random == 2){
            random_way(random_door_left(home1[1] , home2[1] , g) , random_door_right(home1[0] , home2[0] , g) , g);
            random_way(random_door_down(home1[1] , home2[1] , g) , random_door_up(home1[4] , home2[4] , g) , g);
            random_way(random_door_left(home1[4] , home2[4] , g) , random_door_right(home1[3] , home2[3] , g) , g);
            random_way(random_door_right(home1[4] , home2[4] , g) , random_door_left(home1[5] , home2[5] , g) , g);
            random_way(random_door_down(home1[2] , home2[2] , g) , random_door_up(home1[5] , home2[5] , g) , g);
            random_way(random_door_down(home1[0] , home2[0] , g) , random_door_up(home1[3] , home2[3] , g) , g);
        }
        else if(random == 3){
            random_way(random_door_left(home1[4] , home2[4] , g) , random_door_right(home1[3] , home2[3] , g) , g);
            random_way(random_door_right(home1[4] , home2[4] , g) , random_door_left(home1[5] , home2[5] , g) , g);
            random_way(random_door_down(home1[2] , home2[2] , g) , random_door_up(home1[5] , home2[5] , g) , g);
            random_way(random_door_down(home1[0] , home2[0] , g) , random_door_up(home1[3] , home2[3] , g) , g);
            random_way(random_door_down(home1[1] , home2[1] , g) , random_door_up(home1[4] , home2[4] , g) , g);
        }
        else if(random == 4){
            random_way(random_door_down(home1[1] , home2[1] , g) , random_door_up(home1[4] , home2[4] , g) , g);
            random_way(random_door_right(home1[1] , home2[1] , g) , random_door_left(home1[2] , home2[2] , g) , g);
            random_way(random_door_left(home1[1] , home2[1] , g) , random_door_right(home1[0] , home2[0] , g) , g);
            random_way(random_door_left(home1[4] , home2[4] , g) , random_door_right(home1[3] , home2[3] , g) , g);
            random_way(random_door_right(home1[4] , home2[4] , g) , random_door_left(home1[5] , home2[5] , g) , g);
            random_way(random_door_down(home1[0] , home2[0] , g) , random_door_up(home1[3] , home2[3] , g) , g);
        }
        else if(random == 5){
            random_way(random_door_down(home1[1] , home2[1] , g) , random_door_up(home1[4] , home2[4] , g) , g);
            random_way(random_door_right(home1[1] , home2[1] , g) , random_door_left(home1[2] , home2[2] , g) , g);
            random_way(random_door_left(home1[1] , home2[1] , g) , random_door_right(home1[0] , home2[0] , g) , g);
            random_way(random_door_left(home1[4] , home2[4] , g) , random_door_right(home1[3] , home2[3] , g) , g);
            random_way(random_door_right(home1[4] , home2[4] , g) , random_door_left(home1[5] , home2[5] , g) , g);
            random_way(random_door_down(home1[2] , home2[2] , g) , random_door_up(home1[5] , home2[5] , g) , g);
        }
    }

    //random spell room;
    int home;
    if(spell == 1){
        int random = rand()%4;
        if(random == 0){
            random_way(random_door_right(home1[1] , home2[1] , g) , random_door_left(home1[2] , home2[2] , g) , g);
            random_way(random_door_left(home1[1] , home2[1] , g) , random_door_right(home1[0] , home2[0] , g) , g);
            random_way(random_door_down(home1[1] , home2[1] , g) , random_door_up(home1[4] , home2[4] , g) , g);
            random_way(random_door_left(home1[4] , home2[4] , g) , random_door_right(home1[3] , home2[3] , g) , g);
            loc aval = random_door_down(home1[2] , home2[2] , g);
            loc dovom = random_door_up(home1[5] , home2[5] , g);
            random_way(aval , dovom , g);
            aval.y -=1;
            dovom.y +=1;
            g->under_map[loc_to_int(aval)] = '?';
            g->map[loc_to_int(aval)] = '_';
            for(int i = home1[5].x ; i <= home2[5].x ; i++){
                for(int j = home1[5].y ; j<= home2[5].y ; j++){
                    loc inja;
                    inja.x = i;
                    inja.y = j;
                    g->under_map[loc_to_int(inja)] = 'S';
                }
            }
            home = 5;
        }
        if(random == 1){
            random_way(random_door_down(home1[1] , home2[1] , g) , random_door_up(home1[4] , home2[4] , g) , g);
            loc aval = random_door_left(home1[4] , home2[4] , g);
            loc dovom = random_door_right(home1[3] , home2[3] , g);
            random_way(random_door_right(home1[1] , home2[1] , g) , random_door_left(home1[2] , home2[2] , g) , g);
            random_way(random_door_left(home1[1] , home2[1] , g) , random_door_right(home1[0] , home2[0] , g) , g);
            random_way(aval , dovom , g);
            random_way(random_door_right(home1[4] , home2[4] , g) , random_door_left(home1[5] , home2[5] , g) , g);
            aval.x +=1;
            dovom.x -=1;
            g->under_map[loc_to_int(aval)] = '?';
            g->map[loc_to_int(aval)] = '|';
            for(int i = home1[3].x ; i <= home2[3].x ; i++){
                for(int j = home1[3].y ; j<= home2[3].y ; j++){
                    loc inja;
                    inja.x = i;
                    inja.y = j;
                    g->under_map[loc_to_int(inja)] = 'S';
                }
            }
            home = 3;
        }
        if(random == 2){
            random_way(random_door_down(home1[1] , home2[1] , g) , random_door_up(home1[4] , home2[4] , g) , g);
            random_way(random_door_right(home1[1] , home2[1] , g) , random_door_left(home1[2] , home2[2] , g) , g);
            loc aval = random_door_left(home1[1] , home2[1] , g);
            loc dovom = random_door_right(home1[0] , home2[0] , g);
            random_way(aval , dovom, g);
            random_way(random_door_left(home1[4] , home2[4] , g) , random_door_right(home1[3] , home2[3] , g) , g);
            random_way(random_door_right(home1[4] , home2[4] , g) , random_door_left(home1[5] , home2[5] , g) , g);
            random_way(random_door_down(home1[2] , home2[2] , g) , random_door_up(home1[5] , home2[5] , g) , g);
            aval.x +=1;
            dovom.x -=1;
            g->under_map[loc_to_int(aval)] = '?';
            g->map[loc_to_int(aval)] = '|';
            for(int i = home1[0].x ; i <= home2[0].x ; i++){
                for(int j = home1[0].y ; j<= home2[0].y ; j++){
                    loc inja;
                    inja.x = i;
                    inja.y = j;
                    g->under_map[loc_to_int(inja)] = 'S';
                }
            }
            home = 0;
        }
        if(random == 3){
            random_way(random_door_left(home1[4] , home2[4] , g) , random_door_right(home1[3] , home2[3] , g) , g);
            random_way(random_door_right(home1[4] , home2[4] , g) , random_door_left(home1[5] , home2[5] , g) , g);
            random_way(random_door_down(home1[2] , home2[2] , g) , random_door_up(home1[5] , home2[5] , g) , g);
            random_way(random_door_down(home1[0] , home2[0] , g) , random_door_up(home1[3] , home2[3] , g) , g);
            loc aval = random_door_down(home1[1] , home2[1] , g);
            loc dovom = random_door_up(home1[4] , home2[4] , g);
            random_way(aval , dovom , g);
            aval.y -=1;
            dovom.y +=1;
            g->under_map[loc_to_int(dovom)] = '?';
            g->map[loc_to_int(dovom)] = '_';

            for(int i = home1[1].x ; i <= home2[1].x ; i++){
                for(int j = home1[1].y ; j<= home2[1].y ; j++){
                    loc inja;
                    inja.x = i;
                    inja.y = j;
                    g->under_map[loc_to_int(inja)] = 'S';
                }
            }
            home = 1;

        }

        int baghimoondeh = 6;
        while(baghimoondeh > 0){
            loc telesm;
            telesm.x = home1[home].x + 1 + rand()%(home2[home].x - home1[home].x - 1);
            telesm.y = home1[home].y + 1 + rand()%(home2[home].y - home1[home].y - 1);
            if((g->map[loc_to_int(telesm)] == '.') && (g->under_map[loc_to_int(telesm)] != 'x')){
                int random_telesm = rand()%3 + 48;
                g->map[loc_to_int(telesm)] = random_telesm;
                baghimoondeh -=1;
            }
        }

    }
    int treasure_room = -1;

    int stairs_room;
    if(treasure != 1){
        //random stairs
        while(1){
            int random = rand()%6;

            if(random != home){
                stairs_room = random;
                int x_rand = rand()%(home2[random].x - home1[random].x -3) + home1[random].x + 2;
                int y_rand = rand()%(home2[random].y - home1[random].y -3) + home1[random].y + 2;
                loc stairs;
                stairs.x = x_rand;
                stairs.y = y_rand;
                g->map[loc_to_int(stairs)] = '<';
                break;
            }
        }
    }


    if(treasure == 1){
        treasure_room = rand()%6;
        for(int i = home1[treasure_room].x ; i <= home2[treasure_room].x ; i++){
            for(int j = home1[treasure_room].y ; j<= home2[treasure_room].y ; j++){
                loc inja;
                inja.x = i;
                inja.y = j;
                g->under_map[loc_to_int(inja)] = 'T';
            }
        }
        loc PAYAN;
        PAYAN.x = home2[treasure_room].x - 2;
        PAYAN.y = home2[treasure_room].y - 2;
        g->map[loc_to_int(PAYAN)] = 'P';

        int baghimoondeh = 6;
        while(baghimoondeh > 0){
            loc tale_treasure;
            tale_treasure.x = home1[treasure_room].x + 1 + rand()%(home2[treasure_room].x - home1[treasure_room].x - 1);
            tale_treasure.y = home1[treasure_room].y + 1 + rand()%(home2[treasure_room].y - home1[treasure_room].y - 1);
            if((g->map[loc_to_int(tale_treasure)] == '.') && (g->under_map[loc_to_int(tale_treasure)] != 'x')){
                g->under_map[loc_to_int(tale_treasure)] = 'x';
                baghimoondeh -=1;
            }
        }

        for(int i = 0 ; i < 3 ; i++){
            loc talaa;
            int siah = rand()%2;
            talaa.x = home1[treasure_room].x + 1 + rand()%(home2[treasure_room].x - home1[treasure_room].x - 1);
            talaa.y = home1[treasure_room].y + 1 + rand()%(home2[treasure_room].y - home1[treasure_room].y - 1);
            if((g->map[loc_to_int(talaa)] == '.') && (g->under_map[loc_to_int(talaa)] == 'T') && (siah == 0))
                g->map[loc_to_int(talaa)] = '*';
            if((g->map[loc_to_int(talaa)] == '.') && (g->under_map[loc_to_int(talaa)] == 'T') && (siah == 1))
                g->map[loc_to_int(talaa)] = 'G';
            if((g->map[loc_to_int(talaa) + 1] == '.') && (g->under_map[loc_to_int(talaa) + 1] == 'T'))
                g->map[loc_to_int(talaa) + 1] = '*';
            if((g->map[loc_to_int(talaa) - 1] == '.') && (g->under_map[loc_to_int(talaa) - 1] == 'T'))
                g->map[loc_to_int(talaa) - 1] = '*';
            if((g->map[loc_to_int(talaa) + COLS] == '.') && (g->under_map[loc_to_int(talaa) + COLS] == 'T'))
                g->map[loc_to_int(talaa) + COLS] = '*';
            if((g->map[loc_to_int(talaa) - COLS] == '.') && (g->under_map[loc_to_int(talaa) - COLS] == 'T'))
                g->map[loc_to_int(talaa) - COLS] = '*';
            if((g->map[loc_to_int(talaa) + 1 + COLS] == '.') && (g->under_map[loc_to_int(talaa) + 1 + COLS] == 'T'))
                g->map[loc_to_int(talaa) + 1 + COLS] = '*';
            if((g->map[loc_to_int(talaa) + 1 - COLS] == '.') && (g->under_map[loc_to_int(talaa) + 1 - COLS] == 'T'))
                g->map[loc_to_int(talaa) + 1 - COLS] = '*';
            if((g->map[loc_to_int(talaa) - 1 - COLS] == '.') && (g->under_map[loc_to_int(talaa) - 1 - COLS] == 'T'))
                g->map[loc_to_int(talaa) - 1 - COLS] = '*';
            if((g->map[loc_to_int(talaa) - 1 + COLS] == '.') && (g->under_map[loc_to_int(talaa) - 1 + COLS] == 'T'))
                g->map[loc_to_int(talaa) - 1 + COLS] = '*';
        } 

    }


    //random pillar
    for(int i = 0 ; i < 6 ; i ++){
        if(i != stairs_room){
            int vojood = rand()%2;
            if(vojood == 1){
                loc pilar;
                pilar.x = 2 + rand()%2 + home1[i].x ;
                pilar.y = 2 + rand()%2 + home1[i].y;
                if(g->map[loc_to_int(pilar)] == '.')
                    g->map[loc_to_int(pilar)] = 'O';
            }
        }
    }

    //random tale 
    for(int i = 0 ; i<6 ; i++){
        if((i != home) && (i != treasure_room)){
            int vojood = rand()%2;
            if(vojood == 1){
                loc tale;
                tale.x = home1[i].x + 2 + rand()%(home2[i].x - home1[i].x - 3);
                tale.y = home1[i].y + 2 + rand()%(home2[i].y - home1[i].y - 3);
                if((g->map[loc_to_int(tale)] == '.')&&(g->under_map[loc_to_int(tale)] != 'X'))
                    g->under_map[loc_to_int(tale)] = 'X';
            }
        }
    }

    //random telesm
    for(int i = 0 ; i<6 ; i++){
        if((i != home) && (i != treasure_room)){
            int vojood = rand()%2;
            
            if(vojood == 1){
                loc tale;
                tale.x = home1[i].x + 2 + rand()%(home2[i].x - home1[i].x - 3);
                tale.y = home1[i].y + 2 + rand()%(home2[i].y - home1[i].y - 3);
                int telesm = rand()%3 + 48;
                if((g->map[loc_to_int(tale)] == '.')&&(g->under_map[loc_to_int(tale)] != 'X'))
                    g->map[loc_to_int(tale)] = telesm;
            }
        }
    }

    //random talaa
    for(int i = 0 ; i<6 ; i++){
        if((i != home) && (i != treasure_room)){
            int vojood = rand()%8;
            if((vojood == 1)||(vojood == 2) || (vojood == 3)){
                loc talaa;
                talaa.x = home1[i].x + 2 + rand()%(home2[i].x - home1[i].x - 3);
                talaa.y = home1[i].y + 2 + rand()%(home2[i].y - home1[i].y - 3);
                if((g->map[loc_to_int(talaa)] == '.')&&(g->under_map[loc_to_int(talaa)] != 'X'))
                    g->map[loc_to_int(talaa)] = '*';
            }
            if(vojood == 4){
                loc talaa;
                talaa.x = home1[i].x + 2 + rand()%(home2[i].x - home1[i].x - 3);
                talaa.y = home1[i].y + 2 + rand()%(home2[i].y - home1[i].y - 3);
                if((g->map[loc_to_int(talaa)] == '.')&&(g->under_map[loc_to_int(talaa)] != 'X'))
                    g->map[loc_to_int(talaa)] = 'G';
            }
        }
    }

   

    // //random khanjar
    // for(int i = 0 ; i<6 ; i++){
    //     if((i != home) && (i != treasure_room)){
    //         int vojood = rand()%10;
    //         if(vojood == 1){
    //             loc khanjar;
    //             khanjar.x = home1[i].x + 1 + rand()%(home2[i].x - home1[i].x - 1);
    //             khanjar.y = home1[i].y + 1 + rand()%(home2[i].y - home1[i].y - 1);
    //             if((g->map[loc_to_int(khanjar)] == '.')&&(g->under_map[loc_to_int(khanjar)] != 'X')&&(g->map[loc_to_int(khanjar) + 1] == '.'))
    //                 g->map[loc_to_int(khanjar)] = 'k';
    //         }
    //     }
    // }

    // //random asaa
    // for(int i = 0 ; i<6 ; i++){
    //     if((i != home) && (i != treasure_room)){
    //         int vojood = rand()%10;
    //         if(vojood == 1){
    //             loc asaa;
    //             asaa.x = home1[i].x + 1 + rand()%(home2[i].x - home1[i].x - 1);
    //             asaa.y = home1[i].y + 1 + rand()%(home2[i].y - home1[i].y - 1);
    //             if((g->map[loc_to_int(asaa)] == '.')&&(g->under_map[loc_to_int(asaa)] != 'X')&&(g->map[loc_to_int(asaa) + 1] == '.'))
    //                 g->map[loc_to_int(asaa)] = 'a';
    //         }
    //     }
    // }

    // //random tir
    // for(int i = 0 ; i<6 ; i++){
    //     if((i != home) && (i != treasure_room)){
    //         int vojood = rand()%6;
    //         if(vojood == 1){
    //             loc tir;
    //             tir.x = home1[i].x + 1 + rand()%(home2[i].x - home1[i].x - 1);
    //             tir.y = home1[i].y + 1 + rand()%(home2[i].y - home1[i].y - 1);
    //             if((g->map[loc_to_int(tir)] == '.')&&(g->under_map[loc_to_int(tir)] != 'X')&&(g->map[loc_to_int(tir) + 1] == '.'))
    //                 g->map[loc_to_int(tir)] = 't';
    //         }
    //     }
    // }

    // //random shamshir
    // for(int i = 0 ; i<6 ; i++){
    //     if((i != home) && (i != treasure_room)){
    //         int vojood = rand()%6;
    //         if(vojood == 1){
    //             loc shamshir;
    //             shamshir.x = home1[i].x + 1 + rand()%(home2[i].x - home1[i].x - 1);
    //             shamshir.y = home1[i].y + 1 + rand()%(home2[i].y - home1[i].y - 1);
    //             if((g->map[loc_to_int(shamshir)] == '.')&&(g->under_map[loc_to_int(shamshir)] != 'X')&&(g->map[loc_to_int(shamshir) + 1] == '.'))
    //                 g->map[loc_to_int(shamshir)] = 's';
    //         }
    //     }
    // }

    //random aslahe 

    for(int i = 0 ; i<6 ; i++){
        if((i != home) && (i != treasure_room)){
            int vojood = rand()%5;
            char aslaheha[4] = {'s', 't' , 'a' , 'k'};
            if((vojood == 1)||(vojood == 0) || (vojood == 2)){
                int random_selah = rand()%4;
                loc shamshir;
                shamshir.x = home1[i].x + 2 + rand()%(home2[i].x - home1[i].x - 3);
                shamshir.y = home1[i].y + 2 + rand()%(home2[i].y - home1[i].y - 3);
                if((g->map[loc_to_int(shamshir)] == '.')&&(g->under_map[loc_to_int(shamshir)] != 'X')&&(g->map[loc_to_int(shamshir) + 1] == '.'))
                    g->map[loc_to_int(shamshir)] = aslaheha[random_selah];
            }
        }
    }

    //random food

    for(int i = 0 ; i<6 ; i++){
        if((i != home) && (i != treasure_room)){
            int vojood = rand()%5;
            char foods[4] = {'4', '5' , '6' , '7'};
            if((vojood == 1)||(vojood == 0) || (vojood == 2)){
                int random_food = rand()%4;
                loc food;
                food.x = home1[i].x + 2 + rand()%(home2[i].x - home1[i].x - 3);
                food.y = home1[i].y + 2 + rand()%(home2[i].y - home1[i].y - 3);
                if((g->map[loc_to_int(food)] == '.')&&(g->under_map[loc_to_int(food)] != 'X')&&(g->map[loc_to_int(food) + 1] == '.'))
                    g->map[loc_to_int(food)] = foods[random_food];
            }
        }
    }


    //random start
    loc start;
    int ebteda;
    while(1){
        ebteda = rand()%6;
        if((ebteda != stairs_room)&& (ebteda != home) && (ebteda != treasure_room)){
            start.x = rand()%(home2[ebteda].x - home1[ebteda].x -3) + home1[ebteda].x + 2;
            start.y = rand()%(home2[ebteda].y - home1[ebteda].y -3) + home1[ebteda].y + 2;
            if((g->under_map[loc_to_int(start)] == 0) && (g->map[loc_to_int(start)] == '.')){
                g->player = start;
                break;
            }
        }
    }

    //random doshman
    for(int i = 0 ; i<6 ; i++){
        if((i != home) && (i != treasure_room)){
            int vojood = rand()%2;
            char subset[5] = {'D', 'F' , 'G' , 'S' , 'U'};
            if((vojood == 1)){
                int random_dosh = rand()%5;
                loc dosh;
                dosh.x = home1[i].x + 1 + rand()%(home2[i].x - home1[i].x - 1);
                dosh.y = home1[i].y + 1 + rand()%(home2[i].y - home1[i].y - 1);
                if((g->map[loc_to_int(dosh)] == '.')&&(g->under_map[loc_to_int(dosh)] != 'X')&&(g->map[loc_to_int(dosh) + 1] == '.') && (dosh.x != g->player.x) && (dosh.y != g->player.y)){
                    g->enemy[g->enemy_num].jash = dosh;
                    g->enemy[g->enemy_num].model = subset[random_dosh];
                    //g->enemy[g->enemy_num].marhale = 0;
                    if(random_dosh == 0){
                        g->enemy[g->enemy_num].joon = 5;
                        g->enemy[g->enemy_num].marhale= 1;
                    }
                    if(random_dosh == 1){
                        g->enemy[g->enemy_num].joon = 10;
                        g->enemy[g->enemy_num].marhale = 1;
                    }
                    if(random_dosh == 2){
                        g->enemy[g->enemy_num].joon = 15;
                        g->enemy[g->enemy_num].marhale = 5;
                    }
                    if(random_dosh == 3){
                        g->enemy[g->enemy_num].joon = 20;
                        g->enemy[g->enemy_num].marhale = 1;
                    }
                    if(random_dosh == 4){
                        g->enemy[g->enemy_num].joon = 30;
                        g->enemy[g->enemy_num].marhale = 5;
                    }

                    g->enemy_num += 1;
                }
            }
        }
    }
}

void handle_move(Game *g){

    handle_show(g , g->player);
    int a = getch();
    if(g->current_telesm != 1){
        if(a == KEY_UP){
            if ((g->map[loc_to_int(g->player) - COLS] != '|') && (g->map[loc_to_int(g->player) - COLS] != '_') && (g->map[loc_to_int(g->player) - COLS] != 0) && (g->map[loc_to_int(g->player) - COLS] != 'O'))
                g->player.y -= 1;
            if (g->under_map[loc_to_int(g->player) - COLS] == '?'){
                g->map[loc_to_int(g->player) - COLS] = '?';
                g->player.y -= 1;
            }
            handle_enemy(g);
        }
        else if(a == KEY_DOWN){
            if ((g->map[loc_to_int(g->player) + COLS] != '|') && (g->map[loc_to_int(g->player) + COLS] != '_') && (g->map[loc_to_int(g->player) + COLS] != 0) && (g->map[loc_to_int(g->player) + COLS] != 'O'))
                g->player.y += 1;
            if (g->under_map[loc_to_int(g->player) + COLS] == '?'){
                g->map[loc_to_int(g->player) + COLS] = '?';
                g->player.y += 1;
            }
            handle_enemy(g);
        }
        else if(a == KEY_RIGHT){
            if ((g->map[loc_to_int(g->player) + 1] != '|') && (g->map[loc_to_int(g->player) + 1] != '_') && (g->map[loc_to_int(g->player) + 1] != 0) && (g->map[loc_to_int(g->player) + 1] != 'O'))
                g->player.x += 1;
            if (g->under_map[loc_to_int(g->player) + 1] == '?'){
                g->map[loc_to_int(g->player) +1] = '?';
                g->player.x += 1;
            }
            handle_enemy(g);
        }
        else if(a == KEY_LEFT){
            if ((g->map[loc_to_int(g->player) - 1] != '|') && (g->map[loc_to_int(g->player) - 1] != '_') && (g->map[loc_to_int(g->player) - 1] != 0) && (g->map[loc_to_int(g->player) - 1] != 'O'))
                g->player.x -= 1;
            if (g->under_map[loc_to_int(g->player) - 1] == '?'){
                g->map[loc_to_int(g->player) -1] = '?';
                g->player.x -= 1;
            }
            handle_enemy(g);
        }
        else if(a == KEY_HOME){
            if ((g->map[loc_to_int(g->player) - 1] != '|') && (g->map[loc_to_int(g->player) - 1] != '_') && (g->map[loc_to_int(g->player) - 1] != 0) && (g->map[loc_to_int(g->player) - 1] != 'O')){
                if ((g->map[loc_to_int(g->player) - COLS] != '|') && (g->map[loc_to_int(g->player) - COLS] != '_') && (g->map[loc_to_int(g->player) - COLS] != 0) && (g->map[loc_to_int(g->player) - COLS] != 'O')){
                    if ((g->map[loc_to_int(g->player) - COLS - 1] != '|') && (g->map[loc_to_int(g->player) - COLS - 1] != '_') && (g->map[loc_to_int(g->player) - COLS - 1] != 0) && (g->map[loc_to_int(g->player) - COLS - 1] != 'O')){
                        g->player.x -= 1;
                        g->player.y -= 1;
                    }
                }
            }
            handle_enemy(g);
        }
        else if(a == KEY_END){
            if ((g->map[loc_to_int(g->player) - 1] != '|') && (g->map[loc_to_int(g->player) - 1] != '_') && (g->map[loc_to_int(g->player) - 1] != 0) && (g->map[loc_to_int(g->player) - 1] != 'O')){
                if ((g->map[loc_to_int(g->player) + COLS] != '|') && (g->map[loc_to_int(g->player) + COLS] != '_') && (g->map[loc_to_int(g->player) + COLS] != 0) && (g->map[loc_to_int(g->player) + COLS] != 'O')){
                    if ((g->map[loc_to_int(g->player) + COLS - 1] != '|') && (g->map[loc_to_int(g->player) + COLS - 1] != '_') && (g->map[loc_to_int(g->player) + COLS - 1] != 0) && (g->map[loc_to_int(g->player) + COLS - 1] != 'O')){
                        g->player.x -= 1;
                        g->player.y += 1;
                    }
                }
            }
            handle_enemy(g);
        }
        else if(a == KEY_PPAGE){
            if ((g->map[loc_to_int(g->player) + 1] != '|') && (g->map[loc_to_int(g->player) + 1] != '_') && (g->map[loc_to_int(g->player) + 1] != 0) && (g->map[loc_to_int(g->player) + 1] != 'O')){
                if ((g->map[loc_to_int(g->player) - COLS] != '|') && (g->map[loc_to_int(g->player) - COLS] != '_') && (g->map[loc_to_int(g->player) - COLS] != 0) && (g->map[loc_to_int(g->player) - COLS] != 'O')){
                    if ((g->map[loc_to_int(g->player) - COLS + 1] != '|') && (g->map[loc_to_int(g->player) - COLS + 1] != '_') && (g->map[loc_to_int(g->player) - COLS + 1] != 0) && (g->map[loc_to_int(g->player) - COLS + 1] != 'O')){
                        g->player.x += 1;
                        g->player.y -= 1;
                    }
                }
            }
            handle_enemy(g);
        }
        else if(a == KEY_NPAGE){
            if ((g->map[loc_to_int(g->player) + 1] != '|') && (g->map[loc_to_int(g->player) + 1] != '_') && (g->map[loc_to_int(g->player) + 1] != 0) && (g->map[loc_to_int(g->player) + 1] != 'O')){
                if ((g->map[loc_to_int(g->player) + COLS] != '|') && (g->map[loc_to_int(g->player) + COLS] != '_') && (g->map[loc_to_int(g->player) + COLS] != 0) && (g->map[loc_to_int(g->player) + COLS] != 'O')){
                    if ((g->map[loc_to_int(g->player) + COLS + 1] != '|') && (g->map[loc_to_int(g->player) + COLS + 1] != '_') && (g->map[loc_to_int(g->player) + COLS + 1] != 0) && (g->map[loc_to_int(g->player) + COLS + 1] != 'O')){
                        g->player.x += 1;
                        g->player.y += 1;
                    }
                }
            }
            handle_enemy(g);
        }
    }
    if(g->current_telesm == 1){

        int c = 2;
        while(c > 0){
            handle_show(g , g->player);
            loc ghabli = g->player;
            if(a == KEY_UP){
                if ((g->map[loc_to_int(g->player) - COLS] != '|') && (g->map[loc_to_int(g->player) - COLS] != '_') && (g->map[loc_to_int(g->player) - COLS] != 0) && (g->map[loc_to_int(g->player) - COLS] != 'O'))
                    g->player.y -= 1;
            }
            else if(a == KEY_DOWN){
                if ((g->map[loc_to_int(g->player) + COLS] != '|') && (g->map[loc_to_int(g->player) + COLS] != '_') && (g->map[loc_to_int(g->player) + COLS] != 0) && (g->map[loc_to_int(g->player) + COLS] != 'O'))
                    g->player.y += 1;
            }
            else if(a == KEY_RIGHT){
                if ((g->map[loc_to_int(g->player) + 1] != '|') && (g->map[loc_to_int(g->player) + 1] != '_') && (g->map[loc_to_int(g->player) + 1] != 0) && (g->map[loc_to_int(g->player) + 1] != 'O'))
                    g->player.x += 1;
            }
            else if(a == KEY_LEFT){
                if ((g->map[loc_to_int(g->player) - 1] != '|') && (g->map[loc_to_int(g->player) - 1] != '_') && (g->map[loc_to_int(g->player) - 1] != 0) && (g->map[loc_to_int(g->player) - 1] != 'O'))
                    g->player.x -= 1;
            }
            else if(a == KEY_HOME){
                if ((g->map[loc_to_int(g->player) - 1] != '|') && (g->map[loc_to_int(g->player) - 1] != '_') && (g->map[loc_to_int(g->player) - 1] != 0) && (g->map[loc_to_int(g->player) - 1] != 'O')){
                    if ((g->map[loc_to_int(g->player) - COLS] != '|') && (g->map[loc_to_int(g->player) - COLS] != '_') && (g->map[loc_to_int(g->player) - COLS] != 0) && (g->map[loc_to_int(g->player) - COLS] != 'O')){
                        if ((g->map[loc_to_int(g->player) - COLS - 1] != '|') && (g->map[loc_to_int(g->player) - COLS - 1] != '_') && (g->map[loc_to_int(g->player) - COLS - 1] != 0) && (g->map[loc_to_int(g->player) - COLS - 1] != 'O')){
                            g->player.x -= 1;
                            g->player.y -= 1;
                        }
                    }
                }
            }
            else if(a == KEY_END){
                if ((g->map[loc_to_int(g->player) - 1] != '|') && (g->map[loc_to_int(g->player) - 1] != '_') && (g->map[loc_to_int(g->player) - 1] != 0) && (g->map[loc_to_int(g->player) - 1] != 'O')){
                    if ((g->map[loc_to_int(g->player) + COLS] != '|') && (g->map[loc_to_int(g->player) + COLS] != '_') && (g->map[loc_to_int(g->player) + COLS] != 0) && (g->map[loc_to_int(g->player) + COLS] != 'O')){
                        if ((g->map[loc_to_int(g->player) + COLS - 1] != '|') && (g->map[loc_to_int(g->player) + COLS - 1] != '_') && (g->map[loc_to_int(g->player) + COLS - 1] != 0) && (g->map[loc_to_int(g->player) + COLS - 1] != 'O')){
                            g->player.x -= 1;
                            g->player.y += 1;
                        }
                    }
                }
            }
            else if(a == KEY_PPAGE){
                if ((g->map[loc_to_int(g->player) + 1] != '|') && (g->map[loc_to_int(g->player) + 1] != '_') && (g->map[loc_to_int(g->player) + 1] != 0) && (g->map[loc_to_int(g->player) + 1] != 'O')){
                    if ((g->map[loc_to_int(g->player) - COLS] != '|') && (g->map[loc_to_int(g->player) - COLS] != '_') && (g->map[loc_to_int(g->player) - COLS] != 0) && (g->map[loc_to_int(g->player) - COLS] != 'O')){
                        if ((g->map[loc_to_int(g->player) - COLS + 1] != '|') && (g->map[loc_to_int(g->player) - COLS + 1] != '_') && (g->map[loc_to_int(g->player) - COLS + 1] != 0) && (g->map[loc_to_int(g->player) - COLS + 1] != 'O')){
                            g->player.x += 1;
                            g->player.y -= 1;
                        }
                    }
                }
            }
            else if(a == KEY_NPAGE){
                if ((g->map[loc_to_int(g->player) + 1] != '|') && (g->map[loc_to_int(g->player) + 1] != '_') && (g->map[loc_to_int(g->player) + 1] != 0) && (g->map[loc_to_int(g->player) + 1] != 'O')){
                    if ((g->map[loc_to_int(g->player) + COLS] != '|') && (g->map[loc_to_int(g->player) + COLS] != '_') && (g->map[loc_to_int(g->player) + COLS] != 0) && (g->map[loc_to_int(g->player) + COLS] != 'O')){
                        if ((g->map[loc_to_int(g->player) + COLS + 1] != '|') && (g->map[loc_to_int(g->player) + COLS + 1] != '_') && (g->map[loc_to_int(g->player) + COLS + 1] != 0) && (g->map[loc_to_int(g->player) + COLS + 1] != 'O')){
                            g->player.x += 1;
                            g->player.y += 1;
                        }
                    }
                }
            } 
            if((ghabli.x == g->player.x)&&(ghabli.y == g->player.y))
                break;  
            if(a == 'q')
                break; 
            if(g->map[loc_to_int(g->player)] == '<'){
                g->end = 2; 
                break;
            } 
            if(g->map[loc_to_int(g->player)] == 'P'){
                g->end = 2;
                treasure_room(g);
                break;
            }  
            if(g->under_map[loc_to_int(g->player)] == 'x'){
                g->map[loc_to_int(g->player)] = '^';
                g->under_map[loc_to_int(g->player)] = 'T';
                fight_room(g);
                break;
            }
            if(g->under_map[loc_to_int(g->player)] == 'X'){
                g->map[loc_to_int(g->player)] = '^';
                g->under_map[loc_to_int(g->player)] = 0;
                fight_room(g);
                break;
            } 
            c -= 1;   
        }
    }

    if(a == 'f'){
        int b = getch();
        while(1){
            handle_show(g , g->player);
            loc ghabli = g->player;
            if(b == KEY_UP){
                if ((g->map[loc_to_int(g->player) - COLS] != '|') && (g->map[loc_to_int(g->player) - COLS] != '_') && (g->map[loc_to_int(g->player) - COLS] != 0) && (g->map[loc_to_int(g->player) - COLS] != 'O'))
                    g->player.y -= 1;
            }
            else if(b == KEY_DOWN){
                if ((g->map[loc_to_int(g->player) + COLS] != '|') && (g->map[loc_to_int(g->player) + COLS] != '_') && (g->map[loc_to_int(g->player) + COLS] != 0) && (g->map[loc_to_int(g->player) + COLS] != 'O'))
                    g->player.y += 1;
            }
            else if(b == KEY_RIGHT){
                if ((g->map[loc_to_int(g->player) + 1] != '|') && (g->map[loc_to_int(g->player) + 1] != '_') && (g->map[loc_to_int(g->player) + 1] != 0) && (g->map[loc_to_int(g->player) + 1] != 'O'))
                    g->player.x += 1;
            }
            else if(b == KEY_LEFT){
                if ((g->map[loc_to_int(g->player) - 1] != '|') && (g->map[loc_to_int(g->player) - 1] != '_') && (g->map[loc_to_int(g->player) - 1] != 0) && (g->map[loc_to_int(g->player) - 1] != 'O'))
                    g->player.x -= 1;
            }
            else if(b == KEY_HOME){
                if ((g->map[loc_to_int(g->player) - 1] != '|') && (g->map[loc_to_int(g->player) - 1] != '_') && (g->map[loc_to_int(g->player) - 1] != 0) && (g->map[loc_to_int(g->player) - 1] != 'O')){
                    if ((g->map[loc_to_int(g->player) - COLS] != '|') && (g->map[loc_to_int(g->player) - COLS] != '_') && (g->map[loc_to_int(g->player) - COLS] != 0) && (g->map[loc_to_int(g->player) - COLS] != 'O')){
                        if ((g->map[loc_to_int(g->player) - COLS - 1] != '|') && (g->map[loc_to_int(g->player) - COLS - 1] != '_') && (g->map[loc_to_int(g->player) - COLS - 1] != 0) && (g->map[loc_to_int(g->player) - COLS - 1] != 'O')){
                            g->player.x -= 1;
                            g->player.y -= 1;
                        }
                    }
                }
            }
            else if(b == KEY_END){
                if ((g->map[loc_to_int(g->player) - 1] != '|') && (g->map[loc_to_int(g->player) - 1] != '_') && (g->map[loc_to_int(g->player) - 1] != 0) && (g->map[loc_to_int(g->player) - 1] != 'O')){
                    if ((g->map[loc_to_int(g->player) + COLS] != '|') && (g->map[loc_to_int(g->player) + COLS] != '_') && (g->map[loc_to_int(g->player) + COLS] != 0) && (g->map[loc_to_int(g->player) + COLS] != 'O')){
                        if ((g->map[loc_to_int(g->player) + COLS - 1] != '|') && (g->map[loc_to_int(g->player) + COLS - 1] != '_') && (g->map[loc_to_int(g->player) + COLS - 1] != 0) && (g->map[loc_to_int(g->player) + COLS - 1] != 'O')){
                            g->player.x -= 1;
                            g->player.y += 1;
                        }
                    }
                }
            }
            else if(b == KEY_PPAGE){
                if ((g->map[loc_to_int(g->player) + 1] != '|') && (g->map[loc_to_int(g->player) + 1] != '_') && (g->map[loc_to_int(g->player) + 1] != 0) && (g->map[loc_to_int(g->player) + 1] != 'O')){
                    if ((g->map[loc_to_int(g->player) - COLS] != '|') && (g->map[loc_to_int(g->player) - COLS] != '_') && (g->map[loc_to_int(g->player) - COLS] != 0) && (g->map[loc_to_int(g->player) - COLS] != 'O')){
                        if ((g->map[loc_to_int(g->player) - COLS + 1] != '|') && (g->map[loc_to_int(g->player) - COLS + 1] != '_') && (g->map[loc_to_int(g->player) - COLS + 1] != 0) && (g->map[loc_to_int(g->player) - COLS + 1] != 'O')){
                            g->player.x += 1;
                            g->player.y -= 1;
                        }
                    }
                }
            }
            else if(b == KEY_NPAGE){
                if ((g->map[loc_to_int(g->player) + 1] != '|') && (g->map[loc_to_int(g->player) + 1] != '_') && (g->map[loc_to_int(g->player) + 1] != 0) && (g->map[loc_to_int(g->player) + 1] != 'O')){
                    if ((g->map[loc_to_int(g->player) + COLS] != '|') && (g->map[loc_to_int(g->player) + COLS] != '_') && (g->map[loc_to_int(g->player) + COLS] != 0) && (g->map[loc_to_int(g->player) + COLS] != 'O')){
                        if ((g->map[loc_to_int(g->player) + COLS + 1] != '|') && (g->map[loc_to_int(g->player) + COLS + 1] != '_') && (g->map[loc_to_int(g->player) + COLS + 1] != 0) && (g->map[loc_to_int(g->player) + COLS + 1] != 'O')){
                            g->player.x += 1;
                            g->player.y += 1;
                        }
                    }
                }
            } 
            if((ghabli.x == g->player.x)&&(ghabli.y == g->player.y))
                break;  
            if(b == 'q')
                break; 
            if(g->map[loc_to_int(g->player)] == '<'){
                g->end = 2; 
                break;
            } 
            if(g->map[loc_to_int(g->player)] == 'P'){
                g->end = 2;
                treasure_room(g);
                break;
            }  
            if(g->map[loc_to_int(g->player)] == 'Q'){
                win_page(g);
                g->end =1;
            }
            if(g->under_map[loc_to_int(g->player)] == 'x'){
                g->map[loc_to_int(g->player)] = '^';
                g->under_map[loc_to_int(g->player)] = 'T';
                fight_room(g);
                break;
            }
            if(g->under_map[loc_to_int(g->player)] == 'X'){
                g->map[loc_to_int(g->player)] = '^';
                g->under_map[loc_to_int(g->player)] = 0;
                fight_room(g);
                break;
            }    
        }
        handle_enemy(g);
    }
    if(a == 'w')
        g->current_selah = -1;
    if(a == 'q'){
        g->end = 1;
        saving(g);
        second_menu(g);
    }
    if(a == 'm'){
        g->cheat = (g->cheat + 1)%2;
    }
    if(g->map[loc_to_int(g->player)] == '<'){
        g->end = 2;
        g->tabaghe += 1;
    }
    if(g->map[loc_to_int(g->player)] == 'P'){
        treasure_room(g);
        g->end =2;
    }
    if(g->map[loc_to_int(g->player)] == 'Q'){
        win_page(g);
        g->end =1;
    }
    if(g->under_map[loc_to_int(g->player)] == 'x'){
        g->map[loc_to_int(g->player)] = '^';
        g->under_map[loc_to_int(g->player)] = 'T';
        g->health -= 2;
        fight_room(g);
    }
    if(g->under_map[loc_to_int(g->player)] == 'X'){
        g->map[loc_to_int(g->player)] = '^';
        g->under_map[loc_to_int(g->player)] = 0;
        g->health -= 2;
        fight_room(g);
    }
    if(a == 'e'){
        if(g->map[loc_to_int(g->player)] == '*'){
            g->golds += 1;
            g->point += 1*(g->levels + 1);
            g->start_payam = time(NULL);
            g->payam = 1;
            g->map[loc_to_int(g->player)] = '.';
        }
        else if(g->map[loc_to_int(g->player)] == 'G'){
            g->golds += 5;
            g->point += 5*(g->levels + 1);
            g->start_payam = time(NULL);
            g->payam = 2;
            g->map[loc_to_int(g->player)] = '.';
            g->start_payam = time(NULL);
        }
        else if(g->map[loc_to_int(g->player)] == 'k'){
            g->selah[1] += 12;
            g->map[loc_to_int(g->player)] = '.';
            g->payam = 8;
            g->start_payam = time(NULL);
        }
        else if(g->map[loc_to_int(g->player)] == 'b'){
            g->selah[1] += 1;
            g->map[loc_to_int(g->player)] = '.';
            g->payam = 8;
            g->start_payam = time(NULL);
        }
        else if(g->map[loc_to_int(g->player)] == 'a'){
            g->selah[2] += 8;
            g->map[loc_to_int(g->player)] = '.';
            g->payam = 8;
            g->start_payam = time(NULL);
        }
        else if(g->map[loc_to_int(g->player)] == 'c'){
            g->selah[2] += 1;
            g->map[loc_to_int(g->player)] = '.';
            g->payam = 8;
            g->start_payam = time(NULL);
        }
        else if(g->map[loc_to_int(g->player)] == 's'){
            g->selah[3] = 1;
            g->map[loc_to_int(g->player)] = '.';
            g->payam = 8;
            g->start_payam = time(NULL);
        }
        else if(g->map[loc_to_int(g->player)] == 't'){
            g->selah[4] += 20;
            g->map[loc_to_int(g->player)] = '.';
            g->payam = 8;
            g->start_payam = time(NULL);
        }
        else if(g->map[loc_to_int(g->player)] == 'd'){
            g->selah[4] += 1;
            g->map[loc_to_int(g->player)] = '.';
            g->payam = 8;
            g->start_payam = time(NULL);    
        }
        else if(g->map[loc_to_int(g->player)] == '0'){
            g->telesmha[0] += 1;
            g->map[loc_to_int(g->player)] = '.';
        }
        else if(g->map[loc_to_int(g->player)] == '1'){
            g->telesmha[1] += 1;
            g->map[loc_to_int(g->player)] = '.';
        }
        else if(g->map[loc_to_int(g->player)] == '2'){
            g->telesmha[2] += 1;
            g->map[loc_to_int(g->player)] = '.';
        }
        else if(g->map[loc_to_int(g->player)] == '4'){
            g->mamooli[g->ghazaha[0]] = time(NULL);
            g->ghazaha[0] += 1;
            g->map[loc_to_int(g->player)] = '.';
        }
        else if(g->map[loc_to_int(g->player)] == '5'){
            g->ghazaha[1] += 1;
            g->map[loc_to_int(g->player)] = '.';
        }
        else if(g->map[loc_to_int(g->player)] == '6'){
            g->alaa[g->ghazaha[2]] = time(NULL);
            g->ghazaha[2] += 1;
            g->map[loc_to_int(g->player)] = '.';
        }
        else if(g->map[loc_to_int(g->player)] == '7'){
            g->jadooyi[g->ghazaha[3]] = time(NULL);
            g->ghazaha[3] += 1;
            g->map[loc_to_int(g->player)] = '.';
        }

    }

    if(a == 'i')
        selah_menu(g);
    if(a == 'g')
        g->current_selah = 0;
    if(a == 'k')
        g->current_selah = 1;
    if(a == 'a')
        g->current_selah = 2;
    if(a == 's')
        g->current_selah = 3;
    if(a == 't')
        g->current_selah = 4;
    if(a == 'j')
        telesm_menu(g);
    if(a == 'u')
        food_menu(g);
    
    if(a == 32){
        handle_weapon(g);
    }

}

void handle_enemy(Game * g){ 
    for(int i = 0 ; i < g->enemy_num ; i++){
        if(finding_home(g , g->player) == finding_home(g , g->enemy[i].jash)){
            loc buffer = g->enemy[i].jash;
            if((g->enemy[i].model == 'D') && (g->enemy[i].marhale > 0) && (g->enemy[i].joon > 0)){
                if ((buffer.x + 1 == g->player.x) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x + 1) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y + 1 == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y == g->player.y + 1)){
                    //zereshk
                }
                else if ((buffer.x < g->player.x) && (g->map[loc_to_int(buffer) + 1] == '.')){
                    buffer.x += 1;
                }
                else if ((buffer.x > g->player.x) && (g->map[loc_to_int(buffer) - 1] == '.')){
                    buffer.x -= 1;
                }
                else if ((buffer.y < g->player.y) && (g->map[loc_to_int(buffer) + COLS] == '.')){
                    buffer.y += 1;
                }
                else if ((buffer.y > g->player.y) && (g->map[loc_to_int(buffer) - COLS] == '.')){
                    buffer.y -= 1;
                }

                if ((buffer.x + 1 == g->player.x) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x + 1) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y + 1 == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y == g->player.y + 1)){
                    //zereshk
                }
                else if ((buffer.x < g->player.x) && (g->map[loc_to_int(buffer) + 1] == '.')){
                    buffer.x += 1;
                }
                else if ((buffer.x > g->player.x) && (g->map[loc_to_int(buffer) - 1] == '.')){
                    buffer.x -= 1;
                }
                else if ((buffer.y < g->player.y) && (g->map[loc_to_int(buffer) + COLS] == '.')){
                    buffer.y += 1;
                }
                else if ((buffer.y > g->player.y) && (g->map[loc_to_int(buffer) - COLS] == '.')){
                    buffer.y -= 1;
                }
                if(((g->player.x - buffer.x)*(g->player.x - buffer.x) <= 1) && ((g->player.y - buffer.y)*(g->player.y - buffer.y) <= 1)){
                    g->health -= 3;
                    g->payam = 3;
                    g->start_payam = time(NULL);
                }
            }

            if((g->enemy[i].model == 'F') && (((buffer.x - g->player.x)*(buffer.x - g->player.x)) <= 9) && (((buffer.y - g->player.y)*(buffer.y - g->player.y)) <= 9) && (g->enemy[i].marhale > 0) && (g->enemy[i].joon > 0)){
                if ((buffer.x + 1 == g->player.x) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x + 1) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y + 1 == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y == g->player.y + 1)){
                    //zereshk
                }
                else if ((buffer.x < g->player.x) && (g->map[loc_to_int(buffer) + 1] == '.')){
                    buffer.x += 1;
                }
                else if ((buffer.x > g->player.x) && (g->map[loc_to_int(buffer) - 1] == '.')){
                    buffer.x -= 1;
                }
                else if ((buffer.y < g->player.y) && (g->map[loc_to_int(buffer) + COLS] == '.')){
                    buffer.y += 1;
                }
                else if ((buffer.y > g->player.y) && (g->map[loc_to_int(buffer) - COLS] == '.')){
                    buffer.y -= 1;
                }

                if(((g->player.x - buffer.x)*(g->player.x - buffer.x) <= 1) && ((g->player.y - buffer.y)*(g->player.y - buffer.y) <= 1)){
                    g->health -= 5;
                    g->payam = 3;
                    g->start_payam = time(NULL);
                }
            }

            if((g->enemy[i].model == 'G') && (g->enemy[i].marhale > 0) && (g->enemy[i].joon > 0)){
                if ((buffer.x + 1 == g->player.x) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x + 1) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y + 1 == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y == g->player.y + 1)){
                    //zereshk
                }
                else if ((buffer.x < g->player.x) && (g->map[loc_to_int(buffer) + 1] == '.')){
                    buffer.x += 1;
                }
                else if ((buffer.x > g->player.x) && (g->map[loc_to_int(buffer) - 1] == '.')){
                    buffer.x -= 1;
                }
                else if ((buffer.y < g->player.y) && (g->map[loc_to_int(buffer) + COLS] == '.')){
                    buffer.y += 1;
                }
                else if ((buffer.y > g->player.y) && (g->map[loc_to_int(buffer) - COLS] == '.')){
                    buffer.y -= 1;
                }
                g->enemy[i].marhale -= 1;
                g->enemy[i].jash = buffer;

                if(((g->player.x - buffer.x)*(g->player.x - buffer.x) <= 1) && ((g->player.y - buffer.y)*(g->player.y - buffer.y) <= 1)){
                    g->health -= 8;
                    g->payam = 3;
                    g->start_payam = time(NULL);
                }
            }

            if((g->enemy[i].model == 'S') && (g->enemy[i].marhale > 0) && (g->enemy[i].joon > 0)){
                if ((buffer.x + 1 == g->player.x) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x + 1) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y + 1 == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y == g->player.y + 1)){
                    //zereshk
                }
                else if ((buffer.x < g->player.x) && (g->map[loc_to_int(buffer) + 1] == '.')){
                    buffer.x += 1;
                }
                else if ((buffer.x > g->player.x) && (g->map[loc_to_int(buffer) - 1] == '.')){
                    buffer.x -= 1;
                }
                else if ((buffer.y < g->player.y) && (g->map[loc_to_int(buffer) + COLS] == '.')){
                    buffer.y += 1;
                }
                else if ((buffer.y > g->player.y) && (g->map[loc_to_int(buffer) - COLS] == '.')){
                    buffer.y -= 1;
                }

                if(((g->player.x - buffer.x)*(g->player.x - buffer.x) <= 1) && ((g->player.y - buffer.y)*(g->player.y - buffer.y) <= 1)){
                    g->health -= 10;
                    g->payam = 3;
                    g->start_payam = time(NULL);
                }
            }

            if((g->enemy[i].model == 'U') && (((buffer.x - g->player.x)*(buffer.x - g->player.x) + (buffer.y - g->player.y) * (buffer.y - g->player.y)) <= 2) && (g->enemy[i].marhale > 0) && (g->enemy[i].joon > 0)){
                if ((buffer.x + 1 == g->player.x) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x + 1) && (buffer.y == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y + 1 == g->player.y)){
                    //zereshk
                }
                else if ((buffer.x == g->player.x) && (buffer.y == g->player.y + 1)){
                    //zereshk
                }
                else if ((buffer.x < g->player.x) && (g->map[loc_to_int(buffer) + 1] == '.')){
                    buffer.x += 1;
                }
                else if ((buffer.x > g->player.x) && (g->map[loc_to_int(buffer) - 1] == '.')){
                    buffer.x -= 1;
                }
                else if ((buffer.y < g->player.y) && (g->map[loc_to_int(buffer) + COLS] == '.')){
                    buffer.y += 1;
                }
                else if ((buffer.y > g->player.y) && (g->map[loc_to_int(buffer) - COLS] == '.')){
                    buffer.y -= 1;
                }
                g->enemy[i].marhale -= 1;

                if(((g->player.x - buffer.x)*(g->player.x - buffer.x) <= 1) && ((g->player.y - buffer.y)*(g->player.y - buffer.y) <= 1)){
                    g->health -= 15;
                    g->payam = 3;
                    g->start_payam = time(NULL);
                }
            }


            g->enemy[i].jash = buffer;
            
        }
    }
}

void handle_weapon(Game * g){
    int shelik = 0;
    if(g->current_selah == 0){
        for(int i = 0 ; i < g->enemy_num ; i++){
            if(((g->player.x - g->enemy[i].jash.x)*(g->player.x - g->enemy[i].jash.x) + (g->player.y - g->enemy[i].jash.y)*(g->player.y - g->enemy[i].jash.y) == 1) || (((g->player.y - g->enemy[i].jash.y)*(g->player.y - g->enemy[i].jash.y) == 1)  && (g->player.x - g->enemy[i].jash.x)*(g->player.x - g->enemy[i].jash.x) == 1)){
                if(g->selah[0] > 0){
                    g->enemy[i].joon -= 5;
                    g->selah[0] -= 1;
                    g->start_payam = time(NULL);
                    g->payam = 5;
                    if(g->current_telesm == 2)
                        g->enemy[i].joon -= 5;
                    if(g->enemy[i].joon < 1){
                        g->payam = 9;
                        g->point += 5;
                        g->start_payam = time(NULL);
                    }
                    
                }
                else{
                    g->start_payam = time(NULL);
                    g->payam = 6;
                }
            }
        }
    }

    if(g->current_selah == 3){
        for(int i = 0 ; i < g->enemy_num ; i++){
            if(((g->player.x - g->enemy[i].jash.x)*(g->player.x - g->enemy[i].jash.x) + (g->player.y - g->enemy[i].jash.y)*(g->player.y - g->enemy[i].jash.y) == 1) || (((g->player.y - g->enemy[i].jash.y)*(g->player.y - g->enemy[i].jash.y) == 1)  && (g->player.x - g->enemy[i].jash.x)*(g->player.x - g->enemy[i].jash.x) == 1)){
                if(g->selah[3] > 0){
                    g->enemy[i].joon -= 10;
                    g->start_payam = time(NULL);
                    g->payam = 5;
                    if(g->current_telesm == 2)
                        g->enemy[i].joon -= 10;
                    if(g->enemy[i].joon < 1){
                        g->payam = 9;
                        g->point += 5;
                        g->start_payam = time(NULL);
                    }
                }
                else{
                    g->start_payam = time(NULL);
                    g->payam = 6;
                }
            }
        }
    }

    if(g->current_selah == 1){
        if(g->selah[1] > 0){
            int a = getch();
            if(a == KEY_RIGHT){
                for(int i = 1 ; i < 6 ; i ++){
                    if((g->map[loc_to_int(g->player) + i] == '|')||(g->map[loc_to_int(g->player) + i] == 'O') || (g->map[loc_to_int(g->player) + i] == '+')){
                        g->map[loc_to_int(g->player) + i - 1] = 'b';
                        shelik = 1;
                        g->selah[1] -= 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) + i)){
                            shelik = 1;
                            g->enemy[j].joon -= 12;
                            g->selah[1] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 12;
                            i = 10;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->map[loc_to_int(g->player) + 5] = 'b';
                    g->selah[1] -= 1;
                }
            }

            if(a == KEY_LEFT){
                for(int i = 1 ; i < 6 ; i ++){
                    if((g->map[loc_to_int(g->player) - i] == '|')||(g->map[loc_to_int(g->player) - i] == 'O')  || (g->map[loc_to_int(g->player) - i] == '+')){
                        g->map[loc_to_int(g->player) - i + 1] = 'b';
                        shelik = 1;
                        g->selah[1] -= 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) - i)){
                            g->enemy[j].joon -= 12;
                            shelik = 1;
                            g->selah[1] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 12;
                            i = 10;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->map[loc_to_int(g->player) - 5] = 'b';
                    g->selah[1] -= 1;
                }
            }

            if(a == KEY_DOWN){
                for(int i = 1 ; i < 6 ; i ++){
                    if((g->map[loc_to_int(g->player) + i*COLS] == '_')||(g->map[loc_to_int(g->player) + i*COLS] == 'O')  || (g->map[loc_to_int(g->player) + i*COLS] == '+')){
                        g->map[loc_to_int(g->player) + (i - 1)*COLS] = 'b';
                        g->selah[1] -= 1;
                        shelik = 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) + i*COLS)){
                            g->enemy[j].joon -= 12;
                            g->selah[1] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 12;
                            i = 10;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[1] -= 1;
                    g->map[loc_to_int(g->player) + 5*COLS] = 'b';
                }
            
            }

            if(a == KEY_UP){
                for(int i = 1 ; i < 6 ; i ++){
                    if((g->map[loc_to_int(g->player) - i*COLS] == '_')||(g->map[loc_to_int(g->player) - i*COLS] == 'O') || (g->map[loc_to_int(g->player) - i*COLS] == '+')){
                        g->map[loc_to_int(g->player) - (i-1)*COLS] = 'b';
                        g->selah[1] -= 1;
                        shelik = 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) -i*COLS)){
                            g->enemy[j].joon -= 12;
                            g->selah[1] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 12;
                            i = 10;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[1] -= 1;
                    g->map[loc_to_int(g->player) - 5*COLS] = 'b';
                }
            }
        }

        else{
            g->start_payam = time(NULL);
            g->payam = 6;
        }
    }



    if(g->current_selah == 2){
        if(g->selah[2] > 0){
            int a = getch();
            if(a == KEY_RIGHT){
                for(int i = 1 ; i < 11 ; i ++){
                    if((g->map[loc_to_int(g->player) + i] == '|')||(g->map[loc_to_int(g->player) + i] == 'O') || (g->map[loc_to_int(g->player) + i] == '+')){
                        g->map[loc_to_int(g->player) + i - 1] = 'c';
                        shelik = 1;
                        g->selah[2] -= 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) + i)){
                            g->enemy[j].joon -= 15;
                            g->enemy[j].marhale = -1;
                            g->selah[2] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 15;
                            i = 20;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[2] -= 1;
                    g->map[loc_to_int(g->player) + 10] = 'c';
                }
            }

            if(a == KEY_LEFT){
                for(int i = 1 ; i < 11 ; i ++){
                    if((g->map[loc_to_int(g->player) - i] == '|')||(g->map[loc_to_int(g->player) - i] == 'O') || (g->map[loc_to_int(g->player) - i] == '+')){
                        g->map[loc_to_int(g->player) - i + 1] = 'c';
                        g->selah[2] -= 1;
                        shelik = 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) - i)){
                            g->enemy[j].joon -= 15;
                            g->enemy[j].marhale = -1;
                            g->selah[2] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 15;
                            i = 20;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[2] -= 1;
                    g->map[loc_to_int(g->player) - 10] = 'c';
                }
            }

            if(a == KEY_DOWN){
                for(int i = 1 ; i < 11 ; i ++){
                    if((g->map[loc_to_int(g->player) + i*COLS] == '_')||(g->map[loc_to_int(g->player) + i*COLS] == 'O') || (g->map[loc_to_int(g->player) + i*COLS] == '+')){
                        g->map[loc_to_int(g->player) + (i - 1)*COLS] = 'c';
                        g->selah[2] -= 1;
                        shelik = 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) + i*COLS)){
                            g->enemy[j].joon -= 15;
                            g->selah[2] -= 1;
                            g->enemy[j].marhale = -1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 15;
                            i = 20;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[2] -= 1;
                    g->map[loc_to_int(g->player) + 10*COLS] = 'c';
                }
            }

            if(a == KEY_UP){
                for(int i = 1 ; i < 11 ; i ++){
                    if((g->map[loc_to_int(g->player) - i*COLS] == '_')||(g->map[loc_to_int(g->player) - i*COLS] == 'O') || (g->map[loc_to_int(g->player) - i*COLS] == '+')){
                        g->map[loc_to_int(g->player) - (i-1)*COLS] = 'c';
                        shelik = 1;
                        g->selah[2] -= 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) -i*COLS)){
                            g->enemy[j].joon -= 15;
                            g->enemy[j].marhale = -1;
                            g->selah[2] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 15;
                            i = 20;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[2] -= 1;
                    g->map[loc_to_int(g->player) - 10*COLS] = 'c';
                }
            }
        }
        else{
            g->start_payam = time(NULL);
            g->payam = 6;
        }
    }


    if(g->current_selah == 4){
        if(g->selah[4] > 0){
            int a = getch();
            if(a == KEY_RIGHT){
                for(int i = 1 ; i < 6 ; i ++){
                    if((g->map[loc_to_int(g->player) + i] == '|')||(g->map[loc_to_int(g->player) + i] == 'O') || (g->map[loc_to_int(g->player) + i] == '+')){
                        g->map[loc_to_int(g->player) + i - 1] = 'd';
                        g->selah[4] -= 1;
                        shelik = 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) + i)){
                            g->enemy[j].joon -= 5;
                            g->selah[4] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 5;
                            i = 10;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[4] -= 1;
                    g->map[loc_to_int(g->player) + 5] = 'd';
                }
            }

            if(a == KEY_LEFT){
                for(int i = 1 ; i < 6 ; i ++){
                    if((g->map[loc_to_int(g->player) - i] == '|')||(g->map[loc_to_int(g->player) - i] == 'O') || (g->map[loc_to_int(g->player) - i] == '+')){
                        g->map[loc_to_int(g->player) - i + 1] = 'd';
                        g->selah[4] -= 1;
                        shelik = 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) - i)){
                            g->enemy[j].joon -= 5;
                            g->selah[4] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 5;
                            i = 10;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[4] -= 1;
                    g->map[loc_to_int(g->player) - 5] = 'd';
                }
            }

            if(a == KEY_DOWN){
                for(int i = 1 ; i < 6 ; i ++){
                    if((g->map[loc_to_int(g->player) + i*COLS] == '_')||(g->map[loc_to_int(g->player) + i*COLS] == 'O') || (g->map[loc_to_int(g->player) + i*COLS] == '+')){
                        g->map[loc_to_int(g->player) + (i - 1)*COLS] = 'd';
                        g->selah[4] -= 1;
                        shelik = 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) + i*COLS)){
                            g->enemy[j].joon -= 5;
                            g->selah[4] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 5;
                            i = 10;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[4] -= 1;
                    g->map[loc_to_int(g->player) + 5*COLS] = 'd';
                }
            }

            if(a == KEY_UP){
                for(int i = 1 ; i < 6 ; i ++){
                    if((g->map[loc_to_int(g->player) - i*COLS] == '_')||(g->map[loc_to_int(g->player) - i*COLS] == 'O') || (g->map[loc_to_int(g->player) - i*COLS] == '+')){
                        g->map[loc_to_int(g->player) - (i-1)*COLS] = 'd';
                        g->selah[4] -= 1;
                        shelik = 1;
                        break;
                    }
                
                    for(int j = 0 ; j < g->enemy_num ; j++){
                        if(loc_to_int(g->enemy[j].jash) == (loc_to_int(g->player) -i*COLS)){
                            g->enemy[j].joon -= 5;
                            g->selah[4] -= 1;
                            g->start_payam = time(NULL);
                            g->payam = 5;
                            i = 10;
                            shelik = 1;
                            if(g->current_telesm == 2)
                                g->enemy[j].joon -= 5;
                            if(g->enemy[j].joon < 1){
                                g->payam = 9;
                                g->point += 5;
                                g->start_payam = time(NULL);
                            }
                            break;
                        }
                    } 
                }
                if(shelik == 0){
                    g->selah[4] -= 1;
                    g->map[loc_to_int(g->player) - 5*COLS] = 'd';
                }
            }
        }

        else{
            g->start_payam = time(NULL);
            g->payam = 6;
        }
    }
    if(g->current_selah == -1){
        g->payam = 7;
        g->start_payam = time(NULL);
    }

}

int finding_home(Game *g , loc a){
    for(int i = 0 ; i < 6 ; i++){
        if ((a.x <= g->homes[i][1].x) && (a.x >= g->homes[i][0].x) && (a.y <= g->homes[i][1].y) && (a.y >= g->homes[i][0].y) )
            return i;
    }
}

void handle_show(Game *g , loc a){
    int x = loc_to_int(a);
    if((g->map[x] == '#')||(g->map[x] == '+') || (g->map[x] == '?')){
        if((g->map[x+1] == '#')||(g->map[x + 1] == '+'))
            g->show_map[x+1] = '.';
        if((g->map[x-1] == '#')||(g->map[x - 1] == '+'))
            g->show_map[x-1] = '.';
        if((g->map[x+ COLS] == '#')||(g->map[x + COLS] == '+'))
            g->show_map[x+ COLS] = '.';
        if((g->map[x- COLS] == '#')||(g->map[x - COLS] == '+'))
            g->show_map[x -COLS] = '.';
    }
    if(g->map[x] == '+'){
        int k = finding_home(g , a);
        for(int i = g->homes[k][0].x ; i <= g->homes[k][1].x ; i++){
            for(int j = g->homes[k][0].y ; j <= g->homes[k][1].y ; j++){
                loc nemidoonam;
                nemidoonam.x = i;
                nemidoonam.y = j;
                g->show_map[loc_to_int(nemidoonam)] = '.';
            }
        }
    }
    if((g->show_map[x] != '.')&&(g->map[x] == '.')){
        int k = finding_home(g , a);
        for(int i = g->homes[k][0].x ; i <= g->homes[k][1].x ; i++){
            for(int j = g->homes[k][0].y ; j <= g->homes[k][1].y ; j++){
                loc nemidoonam;
                nemidoonam.x = i;
                nemidoonam.y = j;
                g->show_map[loc_to_int(nemidoonam)] = '.';
            }
        }
    }
}

void draw_map(Game *g){
    start_color();
    init_color(2 , 34 , 650 , 34);
    init_color(1 , 650 , 34 , 34);
    init_pair(1 , COLOR_YELLOW , COLOR_BLACK);
    init_pair(2 , COLOR_GREEN, COLOR_BLACK);
    init_pair(3 , COLOR_RED , COLOR_BLACK);
    init_pair(4 , COLOR_MAGENTA , COLOR_BLACK);
    init_pair(5 , COLOR_BLACK , COLOR_YELLOW);

    for(int i = 0 ; i < COLS*LINES ; i++){
        if((g->show_map[i] == '.')|| (g->cheat == 1)){
            if((g->under_map[i] == 'S')&&(g->map[i] == '.'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '|'))
                attron(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '_'))
                attron(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '+'))
                attron(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '['))
                attron(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == ']'))
                attron(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '{'))
                attron(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '}'))
                attron(COLOR_PAIR(3));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '.'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '|'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '_'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '+'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '['))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == ']'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '{'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '}'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '*'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 'T')&&(g->map[i] == 'G'))
                attron(COLOR_PAIR(5));
            else if((g->under_map[i] == 'x')&&(g->map[i] == '.'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 'x')&&(g->map[i] == '|'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'x')&&(g->map[i] == '_'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'x')&&(g->map[i] == '+'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'y')&&(g->map[i] == '.'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '^'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 'y')&&(g->map[i] == '|'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'y')&&(g->map[i] == '_'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'y')&&(g->map[i] == '+'))
                attron(COLOR_PAIR(4));
            else if((g->under_map[i] == 'X')&&(g->map[i] == '.'))
                attron(COLOR_PAIR(2));
            else if((g->under_map[i] == 0)&&(g->map[i] == '.'))
                attron(COLOR_PAIR(2));
            else if((g->under_map[i] == 0)&&(g->map[i] == '^'))
                attron(COLOR_PAIR(2));
            else if((g->under_map[i] == 0)&&(g->map[i] == '<'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '|'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '['))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == ']'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '{'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '}'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '_'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '+'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '*'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == 'G'))
                attron(COLOR_PAIR(5));
            else if((g->under_map[i] == '?')&&(g->map[i] == '_'))
                attron(COLOR_PAIR(1));
            else if((g->under_map[i] == '?')&&(g->map[i] == '|'))
                attron(COLOR_PAIR(1));
            
            mvprintw(i/COLS , i%COLS , "%c" , g->map[i]);
            if(g->map[i] == '#')
                mvprintw(i/COLS , i%COLS , "%s" , "\U00002592");

            if(g->map[i] == '|')
                mvprintw(i/COLS , i%COLS , "%s" , "│");
            if(g->map[i] == '_')
                mvprintw(i/COLS , i%COLS , "%s" , "─");

            for(int j = 0 ; j < 6; j++){
                if(loc_to_int(g->homes[j][0]) == i)
                    mvprintw(i/COLS , i%COLS , "%s" , "┌");
                if(loc_to_int(g->homes[j][1]) == i)
                    mvprintw(i/COLS , i%COLS , "%s" , "┘");
                if(g->homes[j][0].y*COLS + g->homes[j][1].x == i)
                    mvprintw(i/COLS , i%COLS , "%s" , "┐");
                if(g->homes[j][1].y*COLS + g->homes[j][0].x == i)
                    mvprintw(i/COLS , i%COLS , "%s" , "└");
            }

            if((g->under_map[i] == 'S')&&(g->map[i] == '.'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '|'))
                attroff(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '_'))
                attroff(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '+'))
                attroff(COLOR_PAIR(3));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '.'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '|'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '_'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '+'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '*'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 'T')&&(g->map[i] == 'G'))
                attroff(COLOR_PAIR(5));
            else if((g->under_map[i] == 'x')&&(g->map[i] == '.'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 'x')&&(g->map[i] == '|'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'x')&&(g->map[i] == '_'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'x')&&(g->map[i] == '+'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'y')&&(g->map[i] == '.'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '^'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 'y')&&(g->map[i] == '|'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'y')&&(g->map[i] == '_'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'y')&&(g->map[i] == '+'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'X')&&(g->map[i] == '.'))
                attroff(COLOR_PAIR(2));
            else if((g->under_map[i] == 0)&&(g->map[i] == '.'))
                attroff(COLOR_PAIR(2));
            else if((g->under_map[i] == 0)&&(g->map[i] == '^'))
                attroff(COLOR_PAIR(2));
            else if((g->under_map[i] == 0)&&(g->map[i] == '<'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '|'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '_'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '+'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '*'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == 'G'))
                attroff(COLOR_PAIR(5));
            else if((g->under_map[i] == '?')&&(g->map[i] == '_'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == '?')&&(g->map[i] == '|'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '['))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == ']'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '{'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 0)&&(g->map[i] == '}'))
                attroff(COLOR_PAIR(1));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '['))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == ']'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '{'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'T')&&(g->map[i] == '}'))
                attroff(COLOR_PAIR(4));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '['))
                attroff(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == ']'))
                attroff(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '{'))
                attroff(COLOR_PAIR(3));
            else if((g->under_map[i] == 'S')&&(g->map[i] == '}'))
                attroff(COLOR_PAIR(3));
            if(g->map[i] == 'P'){
                attron(COLOR_PAIR(3));
                mvprintw(i/COLS , i%COLS , "%s" , "\U000003A6");
                attroff(COLOR_PAIR(3));
            }
            if(g->map[i] == 'g')
                mvprintw(i/COLS , i%COLS , "%s" , "\U00002692");
            if(g->map[i] == 'k')
                mvprintw(i/COLS , i%COLS , "%s" , "\U0001F5E1");
            if(g->map[i] == 'a')
                mvprintw(i/COLS , i%COLS , "%s" , "\U00000399");
            if(g->map[i] == 's')
                mvprintw(i/COLS , i%COLS , "%s" , "\U00002694");
            if(g->map[i] == 't')
                mvprintw(i/COLS , i%COLS , "%s" , "\U000027B3");
            if(g->map[i] == '0'){
                attron(COLOR_PAIR(2));
                mvprintw(i/COLS , i%COLS , "%s" , "\U000003B6");
                attroff(COLOR_PAIR(2));
            }
            if(g->map[i] == '1')
                mvprintw(i/COLS , i%COLS , "%s" , "\U000003BE");
            if(g->map[i] == '2'){
                attron(COLOR_PAIR(3));
                mvprintw(i/COLS , i%COLS , "%s" , "\U000003F4");
                attroff(COLOR_PAIR(3));
            }
            if(g->map[i] == '4')
                mvprintw(i/COLS , i%COLS , "%s" , "\U0000039B");//mamooli
            if(g->map[i] == '5')
                mvprintw(i/COLS , i%COLS , "%s" , "\U0000039B");//fased
            if(g->map[i] == '6')
                mvprintw(i/COLS , i%COLS , "%s" , "\U000003A3");//alaa
            if(g->map[i] == '7')
                mvprintw(i/COLS , i%COLS , "%s" , "\U0000039E");//jadooie      
        }   
    }
    init_color(52 , 124 , 255 , 2);
    init_pair(120 , 52 , COLOR_BLACK);
    //rgb(124, 255, 218)
    attron(COLOR_PAIR(120));
    int n = g->payam;
    mvprintw(LINES - 3, 1 , "┌");
    mvprintw(LINES -1 , 1 , "└");
    mvprintw(LINES - 3 , 35 , "┐");
    mvprintw(LINES - 1 , 35 , "┘");
    mvprintw(LINES - 2 , 1 , "│");
    mvprintw(LINES -2  , 35 , "│");
    for(int i = 2 ; i<35 ; i++){
        mvprintw(LINES - 1 , i , "─");
        mvprintw(LINES - 3 , i , "─");
    }

    mvprintw(LINES - 3, 99 , "┌");
    mvprintw(LINES -1 , 99 , "└");
    mvprintw(LINES - 3 , COLS - 1 , "┐");
    mvprintw(LINES - 1 , COLS - 1 , "┘");
    mvprintw(LINES - 2 , 99 , "│");
    mvprintw(LINES -2  , COLS - 1 , "│");
    for(int i = 100 ; i<COLS - 1 ; i++){
        mvprintw(LINES - 1 , i , "─");
        mvprintw(LINES - 3 , i , "─");
    }
    attroff(COLOR_PAIR(120));
    char *taslihat[6]= {"Nothing" , "Gorz" , "Khanjar" , "Asaa" , "Shamshir" , "Tir"};
    char *unicodes[6]= {":(" , "\U00002692" , "\U0001F5E1", "\U0001FA84" , "\U00002694" , "\U000027B3"};
    mvprintw(LINES - 2 , 100 , "Current weapon: %s %s" , taslihat[g->current_selah + 1] , unicodes[g->current_selah + 1] );
    char * ability[4] = {"Nothing" , "Nothing" , "Speed" , "Damage"};
    mvprintw(LINES - 2 , 128 , "Current Ability: %s" , ability[g->current_telesm + 1]);

    mvprintw(LINES -2 , 2 , "Gold : %d" , g->golds);
    mvprintw(LINES -2 , 18 , "Point : %d" , g->point);
    draw_health(g);
}

void draw_player(Game *g){
    init_pair(11 , COLOR_WHITE , COLOR_BLACK);
    init_pair(12 , COLOR_RED , COLOR_BLACK);
    init_pair(13 , COLOR_GREEN , COLOR_BLACK);
    init_pair(14 , COLOR_YELLOW , COLOR_BLACK);
    init_pair(15 , COLOR_BLUE , COLOR_BLACK);
    if(g->color == 0)
        attron(COLOR_PAIR(11));
    if(g->color == 1)
        attron(COLOR_PAIR(12));
    if(g->color == 2)
        attron(COLOR_PAIR(13));
    if(g->color == 3)
        attron(COLOR_PAIR(14));
    if(g->color == 4)
        attron(COLOR_PAIR(15));
    mvprintw(g->player.y , g->player.x , "@");

    if(g->color == 0)
        attroff(COLOR_PAIR(11));
    if(g->color == 1)
        attroff(COLOR_PAIR(12));
    if(g->color == 2)
        attroff(COLOR_PAIR(13));
    if(g->color == 3)
        attroff(COLOR_PAIR(14));
    if(g->color == 4)
        attroff(COLOR_PAIR(15));
}

void draw_doshman(Game * g){
    for(int i = 0 ; i < g->enemy_num ; i++)
        if((g->show_map[loc_to_int(g->enemy[i].jash)] == '.') || (g->cheat == 1)){
            if(g->enemy[i].joon > 0)
                mvprintw(loc_to_int(g->enemy[i].jash)/COLS , loc_to_int(g->enemy[i].jash)%COLS , "%c" , g->enemy[i].model);
        }
}

void start_game_normal(Game *g){
    g->map = calloc(LINES * COLS + 10, sizeof(char));
    g->under_map = calloc(LINES * COLS + 10 , sizeof(char));
    g->show_map = calloc(LINES * COLS + 10 , sizeof(char));
    g->end = 0;
    g->cheat = 0;
    g->enemy_num = 0;
    g->payam = 0;
    g->enemy = calloc(30 , sizeof(doshman));
    clear();
    timeout(250);
    random_map(g , 0 , 0);
    while(1){
        handle_show(g , g->player);
        draw_map(g);
        draw_player(g);
        draw_doshman(g);
        printf_payam(g);
        handle_move(g);
        clear();
        draw_map(g);
        draw_player(g);
        if(g->end != 0)
            break;   
    }
    refresh();
}

void start_game_spell(Game *g){
    g->map = calloc(LINES * COLS + 10, sizeof(char));
    g->under_map = calloc(LINES * COLS + 10 , sizeof(char));
    g->show_map = calloc(LINES * COLS + 10 , sizeof(char));
    g->end = 0;
    g->cheat = 0;
    g->enemy_num = 0;
    g->payam = 0;
    g->enemy = calloc(30 , sizeof(doshman));
    clear();
    random_map(g , 1 , 0);
    while(1){
        handle_show(g , g->player);
        draw_map(g);
        draw_player(g);
        draw_doshman(g);
        printf_payam(g);
        handle_move(g);
        clear();
        draw_map(g);
        draw_player(g);
        if(g->end != 0)
            break;   
    }
    refresh();
}

void start_game_treasure(Game *g){
    g->map = calloc(LINES * COLS + 10, sizeof(char));
    g->under_map = calloc(LINES * COLS + 10 , sizeof(char));
    g->show_map = calloc(LINES * COLS + 10 , sizeof(char));
    g->end = 0;
    g->cheat = 0;
    g->enemy_num = 0;
    g->payam = 0;
    g->enemy = calloc(30 , sizeof(doshman));
    clear();
    random_map(g , 0 , 1);
    while(1){
        handle_show(g , g->player);
        draw_map(g);
        draw_player(g);
        draw_doshman(g);
        printf_payam(g);
        handle_move(g);
        clear();
        draw_map(g);
        draw_player(g);
        if(g->end != 0)
            break;   
    }
    refresh();
}

void easy_game(Game *g){
    if(g->tabaghe == 0){
        for(int i = 0 ; i < 6 ; i++)
            g->selah[i] = 0;
        g->selah[0] = 5;
        g->telesmha[0] = 0;
        g->telesmha[1] = 0;
        g->telesmha[2] = 0;
        g->mamooli = calloc(30 , sizeof(int));
        g->alaa = calloc(30 , sizeof(int));
        g->jadooyi = calloc(30 , sizeof(int));
        g->ghazaha[0] = 0;
        g->ghazaha[1] = 0;
        g->ghazaha[2] = 0;
        g->ghazaha[3] = 0;
        g->golds = 0;
        g->point = 0;
        g->health = 60;
        g->hunger = 10;
        g->sec_5 = time(NULL);
        g->sec_1 = time(NULL);
        g->start_telesm = time(NULL);
        g->current_selah = 0;
        g->current_telesm = -1;
        g->enemy_num = 0;
        g->enemy = calloc(30 , sizeof(doshman));
        g->payam = 0;
    }
    if((g->end != 1) && (g->tabaghe == 0))
        start_game_normal(g);
    if((g->end != 1) && (g->tabaghe == 1))
        start_game_spell(g);
    if((g->end != 1) && (g->tabaghe == 2))
        start_game_spell(g);
    if((g->end != 1) && (g->tabaghe == 3))
        start_game_treasure(g);
}

void medium_game(Game * g){
    if(g->tabaghe == 0){
        for(int i = 0 ; i < 6 ; i++)
            g->selah[i] = 0;
        g->selah[0] = 5;
        g->telesmha[0] = 0;
        g->telesmha[1] = 0;
        g->telesmha[2] = 0;
        g->mamooli = calloc(30 , sizeof(int));
        g->alaa = calloc(30 , sizeof(int));
        g->jadooyi = calloc(30 , sizeof(int));
        g->ghazaha[0] = 0;
        g->ghazaha[1] = 0;
        g->ghazaha[2] = 0;
        g->ghazaha[3] = 0;
        g->golds = 0;
        g->point = 0;
        g->sec_5 = time(NULL);
        g->sec_1 = time(NULL);
        g->start_telesm = time(NULL);
        g->health = 60;
        g->hunger = 10;
        g->current_selah = 0;
        g->current_telesm = -1;
        g->enemy_num = 0;
        g->enemy = calloc(30 , sizeof(doshman));
        g->payam = 0;
    }

    if((g->end != 1) && (g->tabaghe == 0))
        start_game_spell(g);
    if((g->end != 1) && (g->tabaghe == 1))
        start_game_spell(g);
    if((g->end != 1) && (g->tabaghe == 2))
        start_game_spell(g);
    if((g->end != 1) && (g->tabaghe == 3))
        start_game_spell(g);
    if((g->end != 1) && (g->tabaghe == 4))
        start_game_treasure(g);
}

void hard_game(Game *g){
    if(g->tabaghe == 0){
        for(int i = 0 ; i < 6 ; i++)
            g->selah[i] = 0;
        g->selah[0] = 5;
        g->telesmha[0] = 0;
        g->telesmha[1] = 0;
        g->telesmha[2] = 0;
        g->mamooli = calloc(30 , sizeof(int));
        g->alaa = calloc(30 , sizeof(int));
        g->jadooyi = calloc(30 , sizeof(int));
        g->ghazaha[0] = 0;
        g->ghazaha[1] = 0;
        g->ghazaha[2] = 0;
        g->ghazaha[3] = 0;
        g->golds = 0;
        g->point = 0;
        g->sec_5 = time(NULL);
        g->sec_1 = time(NULL);
        g->start_telesm = time(NULL);
        g->health = 30;
        g->hunger = 10;
        g->current_selah = 0;
        g->current_telesm = -1;
        g->enemy_num = 0;
        g->enemy = calloc(30 , sizeof(doshman));
        g->payam = 0;
    }

    if((g->end != 1) && (g->tabaghe == 0))
        start_game_normal(g);
    if((g->end != 1) && (g->tabaghe == 1))
        start_game_spell(g);
    if((g->end != 1) && (g->tabaghe == 2))
        start_game_spell(g);
    if((g->end != 1) && (g->tabaghe == 3))
        start_game_spell(g);
    if((g->end != 1) && (g->tabaghe == 4))
        start_game_treasure(g);
}

void start_game(Game *g){
    if(g->tabaghe == -1)
        g->tabaghe = 0;
    play_music(g);
    if(g->levels == 0)
        easy_game(g);
    if(g->levels == 1)
        medium_game(g);
    if(g->levels == 2)
        hard_game(g);
    
}

void win_page(Game *g){
    g->tabaghe = 0;
    g->point += 50;
    g->golds += 30;

    if(g->name[0] != 0){
        char *filename = calloc(60 , sizeof(char));
        for(int i = 0 ; i < strlen(g->name) ; i++)
            filename[i] = g->name[i];
        
        filename[strlen(g->name)] = '.';
        filename[strlen(g->name) + 1] = 't';
        filename[strlen(g->name) + 2] = 'x';
        filename[strlen(g->name) + 3] = 't';
        filename[strlen(g->name) + 4] = '\0';
        FILE * file = fopen(filename , "w");
        fprintf(file , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        fclose(file);
    }

    Mix_HaltMusic();
    clear();
    mvprintw(LINES/2 - 10 , COLS/2 - 34 ,"/==================================================================\\");
    mvprintw(LINES/2 - 9 , COLS/2 - 34 , "||        _  __   _____  _   _  __        _____ _   _   _         ||");
    mvprintw(LINES/2 - 8 , COLS/2 - 34 , "||__/\\__ | | \\ \\ / / _ \\| | | | \\ \\      / |_ _| \\ | | | | __/\\__ ||");
    mvprintw(LINES/2 - 7 , COLS/2 - 34 , "||\\    / | |  \\ V | | | | | | |  \\ \\ /\\ / / | ||  \\| | | | \\    / ||");
    mvprintw(LINES/2 - 6 , COLS/2 - 34 , "||/_  _\\ |_|   | || |_| | |_| |   \\ V  V /  | || |\\  | |_| /_  _\\ ||");
    mvprintw(LINES/2 - 5 , COLS/2 - 34 , "||  \\/   (_)   |_| \\___/ \\___/     \\_/\\_/  |___|_| \\_| (_)   \\/   ||");
    mvprintw(LINES/2 - 4 , COLS/2 - 34 , "\\==================================================================/");
    //mvprintw(LINES/2 -3 , COLS/2 - 3 , "YOU WIN");
    if(g->name[0] != '\0'){
        mvprintw(LINES/2 - 1 , COLS/2 - 13, "%s   Points:%d   Golds:%d" , g->name , g->point , g->golds);
        FILE *file = fopen("users.txt" , "r");
        char line[200];
        char user[56];
        char pass[56];
        char email[56];
        int emtiaz;
        int talaa;

        FILE * new_file = fopen("users.txt" , "a");

        fprintf(new_file , "%s %s %s %d %d\n" , g->name , "none" , "none" , g->point , g->golds);

        fclose(new_file);
    }

    else{
        mvprintw(LINES/2 - 1 , COLS/2 - 13, "Guest   Points:%d   Golds:%d" , g->point , g->golds);
    }




    while(1){
        char a = getch();
        if(a == '\n')
            break;  
    }
    second_menu(g);

}

void fight_room(Game *g){
    clear();
    mvprintw(LINES/2 - 3 , COLS/2 - 21 , "Fight room would be added in second phase");
    while(1){
        char a = getch();
        if(a == '\n')
            break;
    }
}

void draw_health(Game *g){    
    int timer_hunger = time(NULL) - g->sec_5;
    int timer_health = time(NULL) - g->sec_1;

    int check = time(NULL) - g->start_telesm;

    if(check >= 10){
        g->current_telesm = -1;
    }

    if(g->hunger > 10)
        g->hunger = 10;
    if(g->health > 60)
        g->health = 60;

    if((check < 10)&&(g->current_telesm == 0)){
        if(timer_health >= 1){
            if(g->health < 59)
                g->health += 2;
            if(g->health == 59)
                g->health = 60;
            g->sec_1 = time(NULL);
        }
    }

    else{
        if((timer_health >= 1)){
            if(g->hunger < 5){
                g->health -= 1;
            }
            else{
                if(g->health < 60)
                    g->health += 1;
            }
            g->sec_1 = time(NULL);
        }
    }
    
    if(timer_hunger >= 5){
        if(g->hunger > 0)
            g->hunger-= 1;
        else
            g->hunger = 0;
        g->sec_5 = time(NULL);
    }

    for(int i = 0 ; i < g->ghazaha[0] ; i++){
        int diffrence = time(NULL) - g->mamooli[i];
        if(diffrence > 30){
            g->mamooli[i] = 0;
            g->ghazaha[0] -= 1;
            g->ghazaha[1] += 1;
        }
    }

    for(int i = 0 ; i < g->ghazaha[2] ; i++){
        int diffrence = time(NULL) - g->alaa[i];
        if(diffrence > 30){
            g->alaa[i] = 0;
            g->ghazaha[2] -= 1;
            g->mamooli[g->ghazaha[0]] = time(NULL);
            g->ghazaha[0] += 1;
        }
    }

    for(int i = 0 ; i < g->ghazaha[3] ; i++){
        int diffrence = time(NULL) - g->jadooyi[i];
        if(diffrence > 30){
            g->jadooyi[i] = 0;
            g->ghazaha[3] -= 1;
            g->mamooli[g->ghazaha[0]] = time(NULL);
            g->ghazaha[0] += 1;
        }
    }

    init_color(52 , 124 , 255 , 218);
    init_pair(120 , 52 , COLOR_BLACK);
    //rgb(124, 255, 218)
    attron(COLOR_PAIR(120));
    int n = g->payam;
    mvprintw(LINES - 3, 36 , "┌");
    mvprintw(LINES -1 , 36 , "└");
    mvprintw(LINES - 3 , 98 , "┐");
    mvprintw(LINES - 1 , 98 , "┘");
    mvprintw(LINES - 2 , 36 , "│");
    mvprintw(LINES -2  , 98 , "│");
    for(int i = 37 ; i<98 ; i++){
        mvprintw(LINES - 1 , i , "─");
        mvprintw(LINES - 3 , i , "─");
    }
    attroff(COLOR_PAIR(120));


    mvprintw(LINES - 2 , 39 , "%s Health: " , "\U00002764");
   
    int percent = (g->health* 10 / 60);
    for(int i = 0 ; i < percent ; i++)
        mvprintw(LINES - 2 , 49 + i , "%s" , "\u258c");

    mvprintw(LINES -2 , 59 , "%d%%" , (g->health*100 /60));

    mvprintw(LINES - 2 , 68 , "%sHunger: " , "\U0001F354");
    for(int i = 0 ; i < g->hunger ; i++)
        mvprintw(LINES -2 , 78 + i , "%s" , "\u258c");
    mvprintw(LINES - 2 , 88 , "%d%%" , g->hunger*10);
    
    if(g->health <= 0){
        g->tabaghe = 0;
        lose_page(g);
    }
}

void selah_menu(Game *g){
    char *taslihat[5]= {"Gorz" , "Khanjar" , "Asaa" , "Shamshir" , "Tir"};
    init_pair(1 , COLOR_MAGENTA, COLOR_BLACK);
    noecho();
    curs_set(FALSE);
    init_pair(3 , COLOR_BLACK , COLOR_YELLOW);
    int choice = 0;
    while(1){
        clear();
        attron(COLOR_PAIR(1));
        mvprintw(LINES/2 - 3 , COLS/2 - 7, "Kootah bord:");
        mvprintw(LINES/2 - 2 , COLS/2 - 4, "Gorz %d" , g->selah[0]);
        mvprintw(LINES/2 - 1 , COLS/2 - 4, "Shamshir %d" , g->selah[3]);
        //mvprintw(LINES/2 , COLS/2 - 4, "Tir %d" , g->selah[4]);
        mvprintw(LINES/2 + 1 , COLS/2 - 7, "Boland bord:");
        mvprintw(LINES/2 +2, COLS/2 - 4, "Tir %d" , g->selah[4]);
        mvprintw(LINES/2 + 3, COLS/2 - 4, "Khanjar %d" , g->selah[1]);
        mvprintw(LINES/2 + 4, COLS/2 - 4, "Asaa %d" , g->selah[2]);


        init_pair(2 , COLOR_BLACK , COLOR_WHITE);
        attron(COLOR_PAIR(2));
        if(g->current_selah >= 0)
            mvprintw(LINES/2 - 5 , COLS/2 - 4 , "%s" , taslihat[g->current_selah]);
        else
            mvprintw(LINES/2 - 5 , COLS/2 - 4 , "None");
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));
        mvprintw(LINES/2 - 7 , COLS/2 - 13 , "*** Pess 'q' to exit ***");
        attroff(COLOR_PAIR(3));
        int a = getch();
        if(a == 'q'){
            break;
        }
        refresh();
    }
}

void telesm_menu(Game *g){
    clear();
    char *chemidoonam[3]= {"Health" , "Speed" , "Damage"};
    init_pair(1 , COLOR_MAGENTA, COLOR_BLACK);
    noecho();
    curs_set(FALSE);
    init_pair(3 , COLOR_BLACK , COLOR_YELLOW);
    int choice = 0;
    while(1){
        attron(COLOR_PAIR(1));
        for(int i = 0 ; i <3 ; i ++){
            if(i == choice)
                attron(A_REVERSE);
            mvprintw(LINES/2 -3 + i , COLS/2 -5 ,"%s %d" , chemidoonam[i] , g->telesmha[i]);
            if(i == choice)
                attroff(A_REVERSE);
        }
        init_pair(2 , COLOR_BLACK , COLOR_WHITE);
        attron(COLOR_PAIR(3));
        mvprintw(LINES/2 - 5 , COLS/2 - 13 , "*** Pess 'q' to exit ***");
        attroff(COLOR_PAIR(3));
        int a = getch();
        if(a == 'q'){
            break;
        }
        if(a == KEY_UP)
            choice = (choice + 2)%3;
        else if(a == KEY_DOWN)
            choice = (choice + 1)%3;
        else if(a == '\n'){
            if(g->telesmha[choice] > 0){
                g->current_telesm = choice;
                g->telesmha[choice] -= 1;
                g->start_telesm = time(NULL);
                break;
            }
            attron(A_BLINK);
            mvprintw(LINES/2 - 7 , COLS/2 - 20 , "!*!*! This choice is not available !*!*!");
            attroff(A_BLINK);
        }
        else if(a == 'h'){
            if(g->telesmha[0] > 0){
                g->current_telesm = 0;
                g->telesmha[0] -= 1;
                g->start_telesm = time(NULL);
                break;
            }
            attron(A_BLINK);
            mvprintw(LINES/2 - 7 , COLS/2 - 20 , "!*!*! This choice is not available !*!*!");
            attroff(A_BLINK);
        }

        else if(a == 'p'){
            if(g->telesmha[1] > 0){
                g->current_telesm = 1;
                g->telesmha[1] -= 1;
                g->start_telesm = time(NULL);
                break;
            }
            attron(A_BLINK);
            mvprintw(LINES/2 - 7 , COLS/2 - 20 , "!*!*! This choice is not available !*!*!");
            attroff(A_BLINK);
        }

        else if(a == 'd'){
            if(g->telesmha[2] > 0){
                g->current_telesm = 2;
                g->telesmha[2] -= 1;
                g->start_telesm = time(NULL);
                break;
            }
            attron(A_BLINK);
            mvprintw(LINES/2 - 7 , COLS/2 - 20 , "!*!*! This choice is not available !*!*!");
            attroff(A_BLINK);
        }
        
        refresh();
    }
}

void food_menu(Game *g){
    clear();
    char *chemidoonam[3]= {"Mamouli" , "Alaa" , "Jadoui"};
    init_pair(1 , COLOR_MAGENTA, COLOR_BLACK);
    noecho();
    curs_set(FALSE);
    init_pair(3 , COLOR_BLACK , COLOR_YELLOW);
    int choice = 0;
    while(1){
        attron(COLOR_PAIR(1));
        for(int i = 0 ; i <3 ; i ++){
            if(i == choice)
                attron(A_REVERSE);
            if(i == 0)
                mvprintw(LINES/2 -3 + i , COLS/2 -5 ,"%s %d" , chemidoonam[i] , g->ghazaha[0]  + g->ghazaha[1]);
            if(i != 0)
                mvprintw(LINES/2 -3 + i , COLS/2 -5 ,"%s %d" , chemidoonam[i] , g->ghazaha[i + 1] );
            if(i == choice)
                attroff(A_REVERSE);
        }
        init_pair(2 , COLOR_BLACK , COLOR_WHITE);
        attron(COLOR_PAIR(3));
        mvprintw(LINES/2 - 5 , COLS/2 - 13 , "*** Pess 'q' to exit ***");
        attroff(COLOR_PAIR(3));
        int a = getch();
        if(a == 'q'){
            break;
        }
        if(a == KEY_UP)
            choice = (choice + 2)%3;
        else if(a == KEY_DOWN)
            choice = (choice + 1)%3;
        else if(a == '\n'){
            if(choice > 0){
                if(g->ghazaha[choice + 1] > 0){
                    if(choice == 1)
                        g->current_telesm = 2;
                    if(choice == 2)
                        g->current_telesm = 1;

                    g->ghazaha[choice + 1] -= 1;
                    g->hunger += 5;
                    g->health += 5;
                    g->start_telesm = time(NULL);
                    break;
                }
            }
            if(choice == 0){
                if((g->ghazaha[0] == 0)&&(g->ghazaha[1] > 0)){
                    g->health -= 5;
                    g->hunger -= 1;
                    g->ghazaha[1] -= 1;
                    break;
                }
                else if((g->ghazaha[0] > 0)&&(g->ghazaha[1] == 0)){
                    g->hunger += 4;
                    g->health += 5;
                    g->ghazaha[0] -= 1;
                    break;
                }

                else if ((g->ghazaha[0] > 0)&&(g->ghazaha[1] > 0)){
                    int random_ghaza = rand()%2;
                    if(random_ghaza == 0){
                        g->hunger += 4;
                        g->health += 5;
                        g->ghazaha[0] -= 1;
                    }
                    else if(random_ghaza == 1){
                        g->health -= 5;
                        g->hunger -= 1;
                        g->ghazaha[1] -= 1;
                    }
                    break;
                }
            }

            attron(A_BLINK);
            mvprintw(LINES/2 - 7 , COLS/2 - 20 , "!*!*! This choice is not available !*!*!");
            attroff(A_BLINK);
        }
        // else if(a == 'h'){
        //     if(g->telesmha[0] > 0){
        //         g->current_telesm = 0;
        //         g->telesmha[0] -= 1;
        //         g->start_telesm = time(NULL);
        //         break;
        //     }
        //     attron(A_BLINK);
        //     mvprintw(LINES/2 - 7 , COLS/2 - 20 , "!*!*! This choice is not available !*!*!");
        //     attroff(A_BLINK);
        // }

        // else if(a == 'p'){
        //     if(g->telesmha[1] > 0){
        //         g->current_telesm = 1;
        //         g->telesmha[1] -= 1;
        //         g->start_telesm = time(NULL);
        //         break;
        //     }
        //     attron(A_BLINK);
        //     mvprintw(LINES/2 - 7 , COLS/2 - 20 , "!*!*! This choice is not available !*!*!");
        //     attroff(A_BLINK);
        // }

        // else if(a == 'd'){
        //     if(g->telesmha[2] > 0){
        //         g->current_telesm = 2;
        //         g->telesmha[2] -= 1;
        //         g->start_telesm = time(NULL);
        //         break;
        //     }
        //     attron(A_BLINK);
        //     mvprintw(LINES/2 - 7 , COLS/2 - 20 , "!*!*! This choice is not available !*!*!");
        //     attroff(A_BLINK);
        // }
        
        refresh();
    }
}

void saving(Game *g){
    char *filename = calloc(60 , sizeof(char));
    for(int i = 0 ; i < strlen(g->name) ; i++)
        filename[i] = g->name[i];
    
    filename[strlen(g->name)] = '.';
    filename[strlen(g->name) + 1] = 't';
    filename[strlen(g->name) + 2] = 'x';
    filename[strlen(g->name) + 3] = 't';
    filename[strlen(g->name) + 4] = '\0';

    FILE * file_avalie = fopen(filename , "w");
    fclose(file_avalie);

    FILE * file = fopen(filename , "w");

    fprintf(file , "%s\n" , g->name);

    fprintf(file , "%d %d\n" , g->player.x , g->player.y);

    fprintf(file , "%d\n" , g->enemy_num);

    for(int i = 0 ; i < g->enemy_num ; i++)
        fprintf(file , "%d %d %c %d %d\n" , g->enemy[i].jash.x , g->enemy[i].jash.y , g->enemy[i].model , g->enemy[i].joon , g->enemy[i].marhale);
    
    for(int i = 0 ; i < 6 ; i++){
        fprintf(file , "%d %d %d %d\n" , g->homes[i][0].x , g->homes[i][0].y , g->homes[i][1].x , g->homes[i][1].y);
    }



    for(int i = 0 ; i < COLS * LINES + 8 ; i++){
        // if(g->map[i] == '.')
        //     fprintf(file , ". ");
        // else
        //     fprintf(file , "0 ");
        fprintf(file , "%c" , g->map[i]);

    }
    fprintf(file , "\n");

    for(int i = 0 ; i < COLS * LINES + 8 ; i++){
        // if(g->map[i] == '.')
        //     fprintf(file , ". ");
        // else
        //     fprintf(file , "0 ");
        fprintf(file , "%c" , g->under_map[i]);

    }

    fprintf(file ,"\n");

    for(int i = 0 ; i < COLS * LINES + 8 ; i++){
        // if(g->map[i] == '.')
        //     fprintf(file , ". ");
        // else
        //     fprintf(file , "0 ");
        fprintf(file , "%c" , g->show_map[i]);

    }

    fprintf(file , "\n");

    fprintf(file , "%d %d %d\n" , g->telesmha[0] , g->telesmha[1] , g->telesmha[2]);

    fprintf(file , "%d %d %d %d %d %d\n" , g->selah[0] , g->selah[1] , g->selah[2] , g->selah[3] , g->selah[4] , g->selah[5]);

    fprintf(file , "%d %d %d %d\n" , g->ghazaha[0] , g->ghazaha[1] , g->ghazaha[2] , g->ghazaha[3]);

    fprintf(file , "%d\n" , g->current_selah);

    fprintf(file , "%d\n" , g->current_telesm);

    fprintf(file , "%d\n" , g->hunger);

    for(int i = 0 ; i < g->ghazaha[0] ; i++){
        fprintf(file , "%ld " , time(NULL) - g->mamooli[i]);
    }

    fprintf(file ,"\n");

    for(int i = 0 ; i < g->ghazaha[2] ; i++){
        fprintf(file ,"%ld " , time(NULL) - g->alaa[i]);
    }

    fprintf(file , "\n");

    for(int i = 0 ; i < g->ghazaha[3] ; i++){
        fprintf(file , "%ld " , time(NULL) - g->jadooyi[i]);
    }

    fprintf(file , "\n");

    fprintf(file , "%d\n" , g->health);

    fprintf(file , "%d\n" , g->point);

    fprintf(file , "%d\n" , g->golds);

    fprintf(file , "%d\n" , g->levels);

    fprintf(file , "%d\n" , g->color);

    fprintf(file , "%d\n" , g->song);

    fprintf(file , "%d\n" , g->tabaghe);

    fclose(file);
}

void resume_game(Game * g){

    char *filename = calloc(60 , sizeof(char));
    for(int i = 0 ; i < strlen(g->name) ; i++)
        filename[i] = g->name[i];
    
    filename[strlen(g->name)] = '.';
    filename[strlen(g->name) + 1] = 't';
    filename[strlen(g->name) + 2] = 'x';
    filename[strlen(g->name) + 3] = 't';
    filename[strlen(g->name) + 4] = '\0';


    g->enemy = calloc(30 , sizeof(doshman));

    g->map = calloc(LINES * COLS + 10, sizeof(char));
    g->under_map = calloc(LINES * COLS + 10 , sizeof(char));
    g->show_map = calloc(LINES * COLS + 10 , sizeof(char));

    g->mamooli = calloc(30 , sizeof(int));
    g->alaa = calloc(30 , sizeof(int));
    g->jadooyi = calloc(30 , sizeof(int));

    g->sec_5 = time(NULL);
    g->sec_1 = time(NULL);
    g->start_telesm = time(NULL);

    g->cheat = 0;
    g->start_payam = time(NULL);
    g->payam = 0;

    g->end = 0;



    FILE * file = fopen(filename , "r");

    char khat[COLS * LINES + 10];
    char test[100];
    fgets(khat , 100 , file);
    sscanf(khat, "%s" , test);

    if((strcmp(test , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") == 0) || (g->name[0] == '\0')){
        while(1){
            clear();
            mvprintw(LINES/2 -3 , COLS/2 - 23 , "!*!*! There's no previous game to resume !*!*!");
            mvprintw(LINES/2 -1 , COLS/2 - 25, "!*!*! To return to the menu , press q button !*!*!");
            char a = getch();
            if(a == 'q'){
                break;
            }
        }
        second_menu(g);
    }

    else{
        sscanf(khat , "%s" , g->name);
        fgets(khat , 100 , file);
        sscanf(khat , "%d %d" , &g->player.x , &g->player.y);

        fgets(khat , 10 , file);
        sscanf(khat , "%d" , &g->enemy_num);

        for(int i = 0 ; i < g->enemy_num ; i++){
            fgets(khat , 100 , file);
            sscanf(khat , "%d %d %c %d %d" , &g->enemy[i].jash.x , &g->enemy[i].jash.y , &g->enemy[i].model , &g->enemy[i].joon , &g->enemy[i].marhale);
        }

        for(int i = 0 ; i < 6 ; i++){
            fgets(khat , 100 , file);
            sscanf(khat , "%d %d %d %d" , &g->homes[i][0].x , &g->homes[i][0].y , &g->homes[i][1].x , &g->homes[i][1].y);
        }


        char ajab1[COLS * LINES + 10];
        fgets(ajab1 , COLS*LINES + 20 , file);
        for(int i = 0 ; i< COLS * LINES + 8 ; i++)
            g->map[i] = ajab1[i];
        
        char ajab2[COLS * LINES + 10];
        fgets(ajab2 , COLS*LINES + 20 , file);
        for(int i = 0 ; i< COLS * LINES + 8 ; i++)
            g->under_map[i] = ajab2[i];
        
        char ajab3[COLS * LINES + 10];

        fgets(ajab3 , COLS*LINES + 20 , file);
        for(int i = 0 ; i< COLS * LINES + 8 ; i++)
            g->show_map[i] = ajab3[i];

        // while(1){
        //     clear();
        //     for(int i = 0 ; i < COLS*LINES ; i++)
        //         mvprintw(i/COLS , i%COLS , "%c" , ajab1[i]);
        //     char a = getch();
        //     if(a == 'q')
        //         break;
        // }

        // while(1){
        //     clear();
        //     for(int i = 0 ; i < COLS*LINES ; i++)
        //         mvprintw(i/COLS , i%COLS , "%c" , ajab2[i]);
        //     char a = getch();
        //     if(a == 'q')
        //         break;
        // }

        // while(1){
        //     clear();
        //     for(int i = 0 ; i < COLS*LINES ; i++)
        //         mvprintw(i/COLS , i%COLS , "%c" , ajab3[i]);
        //     char a = getch();
        //     if(a == 'q')
        //         break;
        // }

        fgets(khat , 100 , file);
        sscanf(khat , "%d %d %d" , &g->telesmha[0] , &g->telesmha[1] , &g->telesmha[2]);

        fgets(khat , 100 , file);
        sscanf(khat , "%d %d %d %d %d %d" , &g->selah[0] , &g->selah[1] , &g->selah[2] , &g->selah[3] , &g->selah[4] , &g->selah[5]);

        fgets(khat , 100 , file);
        sscanf(khat , "%d %d %d %d" , &g->ghazaha[0] , &g->ghazaha[1] , &g->ghazaha[2] , &g->ghazaha[3]);

        fgets(khat , 100 , file);
        sscanf(khat , "%d" , &g->current_selah);

        fgets(khat , 100 , file);
        sscanf(khat , "%d" , &g->current_telesm);

        fgets(khat , 100 , file);
        sscanf(khat , "%d" , &g->hunger);

        for(int i = 0 ; i < g->ghazaha[0] ; i++){
            fgets(khat , 100 , file);
            sscanf(khat , "%d" , &g->mamooli[i]);
            g->mamooli[i] = time(NULL) - g->mamooli[i];
        }
        if(g->ghazaha[0] == 0)
            fgets(khat , 100 , file);

        for(int i = 0 ; i < g->ghazaha[2] ; i++){
            fgets(khat , 100 , file);
            sscanf(khat ,"%d" , &g->alaa[i]);
            g->alaa[i] = time(NULL) - g->alaa[i];
        }
        if(g->ghazaha[2] == 0)
            fgets(khat , 100 , file);


        for(int i = 0 ; i < g->ghazaha[3] ; i++){
            fgets(khat , 100 , file);
            sscanf(khat , "%d" , &g->jadooyi[i]);
            g->jadooyi[i] = time(NULL) - g->jadooyi[i];
        }
        if(g->ghazaha[3] == 0)
            fgets(khat , 100 , file);

        fgets(khat , 100 , file);
        sscanf(khat , "%d" , &g->health);

        // while(1){
        //     mvprintw(1 , 1 , "%d" , g->health);
        //     char a = getch();
        //     if(a == 'q')
        //         break;
        // }

        fgets(khat , 100 , file);
        sscanf(khat , "%d" , &g->point);

        fgets(khat , 100 , file);
        sscanf(khat , "%d" , &g->golds);

        fgets(khat , 100 , file);    
        sscanf(khat , "%d" , &g->levels);

        fgets(khat , 100 , file);
        sscanf(khat , "%d" , &g->color);

        fgets(khat , 100 , file);
        sscanf(khat , "%d" , &g->song);

        fgets(khat , 50 , file);
        sscanf(khat , "%d" , &g->tabaghe);

        fclose(file);
        
        clear();
        timeout(250);

        while(1){
            handle_show(g , g->player);
            draw_map(g);
            draw_player(g);
            draw_doshman(g);
            printf_payam(g);
            handle_move(g);
            clear();
            draw_map(g);
            draw_player(g);
            if(g->end != 0)
                break;   
        }
        start_game(g);
        refresh();
    }
}

void printf_payam(Game * g){
    int timer = time(NULL) - g->start_payam;
    init_color(40 , 0 , 254 , 131);
    init_pair(50 , 40 , COLOR_BLACK);
//rgb(0, 254, 131)
    // if(g->map[i] == '|')
    //             mvprintw(i/COLS , i%COLS , "%s" , "│");
    // if(g->map[i] == '_')
    //             mvprintw(i/COLS , i%COLS , "%s" , "─");
    //rgb(246, 148, 253)

    init_color(51 , 246 , 148 , 253);
    init_pair(110 , 51 , COLOR_BLACK);
    attron(COLOR_PAIR(110));
    int n = g->payam;
    mvprintw(0 , COLS/2 - 22 , "┌");
    mvprintw(2 , COLS/2 - 22 , "└");
    mvprintw(0 , COLS/2 + 22 , "┐");
    mvprintw(2 , COLS/2 + 22 , "┘");
    mvprintw(1 , COLS/2 - 22 , "│");
    mvprintw(1 , COLS/2 + 22 , "│");
    for(int i = COLS/2 - 21 ; i<COLS/2 + 22 ; i++){
        mvprintw(0 , i , "─");
        mvprintw(2 , i , "─");
    }
    attroff(COLOR_PAIR(110));
    attron(COLOR_PAIR(50));
    if(timer < 6){
        if(n == 1)
            mvprintw(1 , COLS/2 - 14 , "!*!*! YOU FOUND 1 GOLD !*!*!");
        if(n == 2)
            mvprintw(1 , COLS/2 - 16 , "!*!*! YOU FOUND BLACK GOLD !*!*!");
        if(n == 3)
            mvprintw(1 , COLS/2 - 17 , "!*!*! ENEMY IS ATTACKING YOU !*!*!");
        if(n == 4)
            mvprintw(1 , COLS/2 - 23 , "!*!*! YOU SUCCESSFULLY KILLED THE ENEMY !*!*!");
        if(n == 5)
            mvprintw(1 , COLS/2 - 18 , "!*!*! YOUR BLOW HIT THE ENEMY !*!*!");
        if(n == 6)
            mvprintw(1 , COLS/2 - 21 , "!*!*! YOU DON'T HAVE ENOUGH WEAPONS !*!*!");
        if(n == 7)
            mvprintw(1 , COLS/2 - 18 , "!*!*! FIRST CHOOSE YOUR WEAPON !*!*!");
        if(n == 8)
            mvprintw(1 , COLS/2 - 15, "!*!*! YOU FOUND A WEAPON !*!*!");
        if(n == 9)
            mvprintw(1 , COLS/2 - 16 , "!*!*! YOU KILLED AN ENEMY !*!*!");
        

    }
    attroff(COLOR_PAIR(50));

    //rgb(251, 255, 0)

    // init_color(41 , 251 , 255 , 0);
    // init_pair(51 , 41 , COLOR_BLACK);

    // attron(COLOR_PAIR(51));

    // //mvprintf(0 , 0 , "|");
    // mvprintw(1 , 0 , "|");
    // for(int i = 0 ; i<50 ; i++){
    //     mvprintw(0 , i , "_");
    //     mvprintw(2 , i , "_");
    // }
    // mvprintw(1 , 49 , "|");

    // attroff(COLOR_PAIR(51));
}

void lose_page(Game * g){
    g->tabaghe = 0;
    Mix_HaltMusic();
    if(g->name[0] != 0){
        char *filename = calloc(60 , sizeof(char));
        for(int i = 0 ; i < strlen(g->name) ; i++)
            filename[i] = g->name[i];
        
        filename[strlen(g->name)] = '.';
        filename[strlen(g->name) + 1] = 't';
        filename[strlen(g->name) + 2] = 'x';
        filename[strlen(g->name) + 3] = 't';
        filename[strlen(g->name) + 4] = '\0';
        FILE * file = fopen(filename , "w");
        fprintf(file , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        fclose(file);
    }


    while(1){
        clear();
    mvprintw(LINES/2 - 10 , COLS/2 - 34 ,"╔═════════════════════════════════════════════════════════════════╗");
    mvprintw(LINES/2 - 9 , COLS/2 - 34 , "║        _  __   _____  _   _   _     ___  ____  _____   _        ║");
    mvprintw(LINES/2 - 8 , COLS/2 - 34 , "║__/\\__ | | \\ \\ / / _ \\| | | | | |   / _ \\/ ___|| ____| | | __/\\__║");
    mvprintw(LINES/2 - 7 , COLS/2 - 34 , "║\\    / | |  \\ V / | | | | | | | |  | | | \\___ \\|  _|   | | \\    /║");
    mvprintw(LINES/2 - 6 , COLS/2 - 34 , "║/_  _\\ |_|   | || |_| | |_| | | |__| |_| |___) | |___  |_| /_  _\\║");
    mvprintw(LINES/2 - 5 , COLS/2 - 34 , "║  \\/   (_)   |_| \\___/ \\___/  |_____\\___/|____/|_____| (_)   \\/  ║");
    mvprintw(LINES/2 - 4 , COLS/2 - 34 , "╚═════════════════════════════════════════════════════════════════╝");
        mvprintw(LINES/2  , COLS/2 -6 , "%s YOU LOSE%s" , "\U0001F62D" , "\U0001F62D");
        mvprintw(LINES/2 + 2 , COLS/2 - 25, "!*!*! To return to the menu , press q button !*!*!");
            char a = getch();
            if(a == 'q'){
                break;
            }
    }
    second_menu(g);
}

void treasure_room(Game * g){
    g->map = calloc(LINES * COLS + 10, sizeof(char));
    g->under_map = calloc(LINES * COLS + 10 , sizeof(char));
    g->show_map = calloc(LINES * COLS + 10 , sizeof(char));
    g->end = 0;
    g->cheat = 0;
    g->enemy_num = 0;
    g->enemy = calloc(30 , sizeof(doshman));

    clear();
    timeout(250);
    for(int i = 0 ; i < 6 ; i++){
        g->homes[i][0].x = COLS/2 - 17;
        g->homes[i][0].y = LINES/2 - 10;
        g->homes[i][1].x = COLS/2 + 17;
        g->homes[i][1].y = LINES/2 + 10;
    }

    for(int i = LINES/2 - 9 ; i< LINES/2 + 10 ; i++){
        for(int j = COLS/2 - 16 ; j<COLS/2 + 17 ; j++){
            g->map[i*COLS + j] = '.';
            g->show_map[i*COLS + j] = '.';
        }
    }
    for(int i = LINES/2 - 10 ; i < LINES/2 + 11 ; i++){
        g->map[i*COLS + COLS/2 - 17] = '|';
        g->map[i*COLS + COLS/2 + 17] = '|';
        g->show_map[i*COLS + COLS/2 - 17] = '.';
        g->show_map[i*COLS + COLS/2 + 17] = '.';
    }

    for(int i = COLS/2 - 17 ; i < COLS/2 + 18 ; i++){
        g->map[COLS * (LINES/2 - 10) + i] = '_';
        g->map[COLS * (LINES/2 + 10) + i] = '_';
        g->show_map[COLS * (LINES/2 - 10) + i] = '.';
        g->show_map[COLS * (LINES/2 + 10) + i] = '.';
    }


    for(int i = LINES/2 -10 ; i < LINES/2 + 11 ; i++){
        for(int j = COLS/2 -17 ; j< COLS/2 + 18 ; j++)
            g->under_map[i*COLS + j] = 'T';
    }

    //random 
    int golds = 25;
    while(golds > 0){
        loc talaa;
        talaa.x = COLS/2 -16 + rand()%33;
        talaa.y = LINES/2 -9 + rand()%19;
        if(g->map[loc_to_int(talaa)] == '.'){
            int random_tala = rand()%2;
            if(random_tala == 0)
                g->map[loc_to_int(talaa)] = '*';
            else if(random_tala == 1)
                g->map[loc_to_int(talaa)] = 'G';
            golds -= 1;
        }
    }

    //random tale

    int tales = 10;
    while(tales > 0){
        loc tale;
        tale.x = COLS/2 -16 + rand()%33;
        tale.y = LINES/2 -9 + rand()%19;
        if((g->map[loc_to_int(tale)] == '.')&&(g->under_map[loc_to_int(tale)] == 'T')){
            g->under_map[loc_to_int(tale)] = 'x';
            tales -= 1;
        }
    }


    int doshmanha = 10;
    while(doshmanha > 0){
        char subset[5] = {'D', 'F' , 'G' , 'S' , 'U'};
        int random_dosh = rand()%5;
        loc dosh;
        dosh.x = COLS/2 -16 + rand()%33;
        dosh.y = LINES/2 -9 + rand()%19;
        if((g->map[loc_to_int(dosh)] == '.')&&(g->under_map[loc_to_int(dosh)] != 'X') && (dosh.x != g->player.x) && (dosh.y != g->player.y)){
            g->enemy[g->enemy_num].jash = dosh;
            g->enemy[g->enemy_num].model = subset[random_dosh];
            //g->enemy[g->enemy_num].marhale = 0;
            if(random_dosh == 0){
                g->enemy[g->enemy_num].joon = 5;
                g->enemy[g->enemy_num].marhale= 1;
            }
            if(random_dosh == 1){
                g->enemy[g->enemy_num].joon = 10;
                g->enemy[g->enemy_num].marhale = 1;
            }
            if(random_dosh == 2){
                g->enemy[g->enemy_num].joon = 15;
                g->enemy[g->enemy_num].marhale = 5;
            }
            if(random_dosh == 3){
                g->enemy[g->enemy_num].joon = 20;
                g->enemy[g->enemy_num].marhale = 1;
            }
            if(random_dosh == 4){
                g->enemy[g->enemy_num].joon = 30;
                g->enemy[g->enemy_num].marhale = 5;
            }

            g->enemy_num += 1;
            doshmanha -= 1;
        }
    }

    g->player.x = COLS/2 - 16;
    g->player.y = LINES/2 - 9;

    g->map[COLS*(LINES/2 + 9) + COLS/2 + 16] = 'Q';


    while(1){
        handle_show(g , g->player);
        draw_map(g);
        draw_player(g);
        draw_doshman(g);
        printf_payam(g);
        handle_move(g);
        clear();
        draw_map(g);
        draw_player(g);
        if(g->end != 0)
            break;   
    }
    refresh();
}

void profile(Game * g){
    clear();
    if(g->name[0] == '\0'){
        // init_pair(100 , COLOR_YELLOW , COLOR_BLACK);
        // attron(COLOR_PAIR(100));
        // mvprintw(1 , 1 , "%s" , "\U00002588");
        // mvprintw(1 , 2 , "%s" , "\U00002588");
        // mvprintw(2 , 1 , "%s" , "\U00002588");
        // mvprintw(2 , 2 , "%s" , "\U00002588");
        // attroff(COLOR_PAIR(100));
        mvprintw(LINES/2 - 11 , COLS/2 - 29  ,"       (**)                                         (**)");
        mvprintw(LINES/2 - 10 , COLS/2 - 29 , "       IIII                                         IIII");
        mvprintw(LINES/2 - 9 , COLS/2 - 29 , "       ####                                         ####");
        mvprintw(LINES/2 - 8 , COLS/2 - 29 , "       HHHH          Please sign in to show         HHHH");
        mvprintw(LINES/2 - 7 , COLS/2 - 29 , "       HHHH            your informations            HHHH");
        mvprintw(LINES/2 - 6 , COLS/2 - 29 , "       ####                                         ####");
        mvprintw(LINES/2 - 5 , COLS/2 - 29 , "    ___IIII___                                   ___IIII___");
        mvprintw(LINES/2 - 4 , COLS/2 - 29 , " .-`_._'**'_._`-.                             .-`_._'**'_._`-.");
        mvprintw(LINES/2 - 3 , COLS/2 - 29 , "|/``  .`\\/`.  ``\\|                           |/``  .`\\/`.  ``\\|");
        mvprintw(LINES/2 - 2 , COLS/2 - 29 , "`     }    {     '                           `     }    {  ");
        mvprintw(LINES/2 - 1 , COLS/2 - 29 , "      ) () (                                       ) () (");
        mvprintw(LINES/2  , COLS/2 - 29 , "      ( :: )                                       ( :: )");
        mvprintw(LINES/2 +1 , COLS/2 - 29 , "      | :: |                                       | :: |");
        mvprintw(LINES/2 +2 , COLS/2 - 29 , "      | )( |                                       | )( |");
        mvprintw(LINES/2 + 3 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 4 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 5 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 6 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 7 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 8 , COLS/2 - 29 , "      ( () )                                       ( () )");
        mvprintw(LINES/2 + 9 , COLS/2 - 29 , "       \\  /                                         \\  /");
        mvprintw(LINES/2 + 10 , COLS/2 - 29 , "        \\/                                           \\/");
    }
    else{
        int scores;
        int golds;
        char pass[55];
        char email[55];

        int line_counter = 0;

        FILE * file = fopen("users.txt" , "r");
        char line[200];
        int tala = 0;
        int emtiaz = 0;
        int tajrobe = -1;
        while(fgets(line , 200 , file)){
            char *esm = malloc(55 * sizeof(char));
            sscanf(line , "%s %s %s %d %d" , esm , pass , email , &scores , &golds);
            if(strcmp(g->name , esm) == 0){
                tajrobe += 1;
                tala += golds;
                emtiaz += scores;
            }
        }
        
        mvprintw(LINES/2 - 11 , COLS/2 - 29  ,"       (**)                                         (**)");
        mvprintw(LINES/2 - 10 , COLS/2 - 29 , "       IIII                                         IIII");
        mvprintw(LINES/2 - 9 , COLS/2 - 29 , "       ####                                         ####");
        mvprintw(LINES/2 - 8 , COLS/2 - 29 , "       HHHH                USERNAME:                HHHH");
        mvprintw(LINES/2 - 7 , COLS/2 - 29 , "       HHHH                                         HHHH");
        mvprintw(LINES/2 - 7 , COLS/2 - 29 , "       HHHH                 %s" , g->name);
        mvprintw(LINES/2 - 6 , COLS/2 - 29 , "       ####                                         ####");
        mvprintw(LINES/2 - 5 , COLS/2 - 29 , "    ___IIII___                                   ___IIII___");
        mvprintw(LINES/2 - 4 , COLS/2 - 29 , " .-`_._'**'_._`-.                             .-`_._'**'_._`-.");
        mvprintw(LINES/2 - 3 , COLS/2 - 29 , "|/``  .`\\/`.  ``\\|                           |/``  .`\\/`.  ``\\|");
        mvprintw(LINES/2 - 2 , COLS/2 - 29 , "`     }    {     '                           `     }    {  ");
        mvprintw(LINES/2 - 1 , COLS/2 - 29 , "      ) () (                                       ) () (");
        mvprintw(LINES/2 - 1 , COLS/2 - 29 , "      ) () (           Experience: %d" , tajrobe);
        mvprintw(LINES/2  , COLS/2 - 29 , "      ( :: )                                       ( :: )");
        mvprintw(LINES/2 +1 , COLS/2 - 29 , "      | :: |                                       | :: |");
        mvprintw(LINES/2 +1 , COLS/2 - 29 , "      | :: |           Total Golds: %d" , tala);
        mvprintw(LINES/2 +2 , COLS/2 - 29 , "      | )( |                                       | )( |");
        mvprintw(LINES/2 + 3 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 3 , COLS/2 - 29 , "      | || |          Total points: %d" , emtiaz);
        mvprintw(LINES/2 + 4 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 5 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 6 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 7 , COLS/2 - 29 , "      | || |                                       | || |");
        mvprintw(LINES/2 + 8 , COLS/2 - 29 , "      ( () )                                       ( () )");
        mvprintw(LINES/2 + 9 , COLS/2 - 29 , "       \\  /                                         \\  /");
        mvprintw(LINES/2 + 10 , COLS/2 - 29 , "        \\/                                           \\/");


    }
    while(1){
        char a = getch();
        if(a == 'q'){
            break;
        }
    }
    second_menu(g);
}

int main(){
    setlocale(LC_ALL,"");
    srand(time(0));
    initscr();
    start_color();
    noecho();
    curs_set(FALSE);
    Game g;
    g.map = calloc(LINES * COLS + 10, sizeof(char));
    g.under_map = calloc(LINES * COLS + 10 , sizeof(char));
    g.show_map = calloc(LINES * COLS + 10 , sizeof(char));
    g.end = 0;
    g.levels = 0;
    g.color = 0;
    g.song = 0;
    g.cheat = 0;
    g.start_payam = time(NULL);
    g.payam = -1;
    g.tabaghe = 0;
    g.playing = 0;
    keypad(stdscr, TRUE);
    first_menu(&g);
    keypad(stdscr , TRUE);
    curs_set(FALSE);
    second_menu(&g);
    clear();
    endwin();
    return 0;
}