
#ifndef _LSIN_H
#define _LSIN_H

int username_check(char *username );

void username(char name[55]);

int password_check(char *password);

void password(char password[55]);

int username_exist(char* username);

void sign_username(char name[55]);

int password_exist(char* username , char* password);

void sign_password(char* username , char* password);

void borderdesign();

#endif /*lsin.h*/