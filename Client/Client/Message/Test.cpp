#include "Message/Auth.h"
#include "Message/ClientText.h"
#include "Message/ServerText.h"
#include <stdio.h>
#include <iostream>

using namespace std;

void testAuth() {
    MessageAuth auth("username", "motdepasse");
   
    char buffer[30];

    int size = auth.serialize((uint8_t*) buffer, 30);
    for(int i = 0; i < size; i++)
        if(isprint(buffer[i]))
            printf("%c ", buffer[i]);
        else
            printf("%d ", buffer[i]);
    printf("\n");

    MessageAuth* auth2 = MessageAuth::deserialize((uint8_t*) buffer, 30);
    cout << auth2->getName() << "\t" << auth2->getPass() << endl;

    delete auth2;
}

void testClientText() {
    MessageClientText text("ceci est un message");
   
    char buffer[30];

    int size = text.serialize((uint8_t*) buffer, 30);
    for(int i = 0; i < size; i++)
        if(isprint(buffer[i]))
            printf("%c ", buffer[i]);
        else
            printf("%d ", buffer[i]);
    printf("\n");

    MessageClientText* text2 = MessageClientText::deserialize((uint8_t*) buffer, 30);
    cout << text2->getMessage() << endl;

    delete text2;
}

void testServerText() {
    MessageServerText text("username", 1000, 423442000, 5050, "ceci est un message");
   
    char buffer[60];

    int size = text.serialize((uint8_t*) buffer, 100);
    for(int i = 0; i < size; i++)
        if(isprint(buffer[i]))
            printf("%c ", buffer[i]);
        else
            printf("%d ", buffer[i]);
    printf("\n");

    MessageServerText* text2 = MessageServerText::deserialize((uint8_t*) buffer, 30);
    cout << text2->getName() << "\t"
         << text2->getMessage() << "\t"
         << text2->getAddress() << "\t"
         << text2->getPort() << "\t"
         << text2->getTime() << "\t"
         << endl;

    delete text2;
}

int main() {
    testAuth();
    testClientText();
    testServerText();

    return 0;
}
