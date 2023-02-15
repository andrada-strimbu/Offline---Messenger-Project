
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#define SIZE 4096
using namespace  std;
int port;

int main(int argc, char *argv[])
{
    int listening;
    struct sockaddr_in server;
    char buf[SIZE];
    if(argc != 3)
    {
        cout<<"Sintaxa: "<<argv[0]<<" 127.0.0.1 <port>"<<endl;
        return 1;
    }
    port= atoi(argv[2]);
    listening= socket(AF_INET,SOCK_STREAM,0);
    if(listening==-1)
    {
        cerr<<"EROARE SOCKET"<<endl;
        return 2;
    }
    server.sin_family=AF_INET;
    server.sin_addr.s_addr= inet_addr(argv[1]);
    server.sin_port= htons(port);
    if(connect(listening,(struct sockaddr*) &server, sizeof(struct sockaddr))==-1)
    {
        cerr<<"[client]EROARE LA CONNECT()";
        return 3;
    }
    cout<<"Introduceti un username: "<<endl;
    fflush(stdout);
    char userInput[SIZE];
    char username[SIZE];
    char username2[SIZE];
    char buf_username[SIZE];
    char buf_mesaj[SIZE];
    char buf_mesaj_trimis[SIZE];
    char log_user2[SIZE];
    char log_user1[SIZE];
    do{
        cout<<">";
        memset(userInput,0,4096);
        cin.getline(userInput,SIZE);
        if (strcmp(userInput, "4") == 0)
            return 0;
        if (send(listening,userInput, sizeof(userInput),0)<=0)
        {
            cerr<<"eroare trimitere la server";
            continue;
        }
        if(strcmp(userInput,"1")==0||strcmp(userInput,"2")==0||strcmp(userInput,"3")==0)
        {
            if (strcmp(userInput, "3") == 0)
            {
                cout << "AI FOST DECONECTAT!" << endl;
                cout << "Introduceti un username: " << endl;
                memset(buf,0,4096);
            }
            else if (strcmp(userInput, "2") == 0)
            {char meniu[10];
                // memset(log_user2,0,4096);
                while (strcmp(log_user2, "da_user") != 0) {
                    cout << "Introdu username-ul persoanei" << endl;
                    cout << ">>";
                    cin.getline(username2, SIZE);
                    if (send(listening, username2, SIZE, 0) <= 0)
                        cerr << "eroare afisare mesaj username";
                    if (recv(listening, log_user2, SIZE, 0) <= 0)
                        cerr << "eroare afisare mesaj client";
                    else if(strcmp(log_user2,"nu_user")==0)
                        cout<<"USER GRESIT!INCEARCA IAR..."<<endl;

                }
                if(strcmp(log_user2, "da_user") == 0)
                    do { memset(log_user2, 0, 4096);
                        //memset(buf_mesaj,0,4096);
                        memset(buf_username, 0, 4096);
                        memset(buf_mesaj, 0, 4096);
                        if (recv(listening, buf_username, SIZE, 0) <= 0)
                            cerr << "eroare afisare mesaj username";
                        else  if (strcmp(buf_username, "127.0.0.1") == 0)
                            break;

                        if (recv(listening, buf_mesaj, SIZE, 0) <= 0)
                            cerr << "eroare afisare mesaj";

                        cout << "<" << buf_username << ">: "<<buf_mesaj << endl;
                    } while (true);
                cout<<endl;
                cout << "vrei sa te intorci in  meniu?da/nu" << endl;
                cin.getline(meniu, 10);
                if (strcmp(meniu, "da") == 0)
                {
                    cout << "Buna, " << username << "!" << endl;
                    cout << endl;
                    cout << "1.Send messege" << endl;
                    cout << "2.Read messege" << endl;
                    cout << "3.LOGOUT" << endl;
                    cout << "4.EXIT" << endl;
                    cout << "SCRIE NUMARUL CORESPUNZATOR COMENZII DIN MENIU:" << endl;
                } else strcpy(userInput, "4");
            }
            else if (strcmp(userInput, "1") == 0) {
                memset(log_user1, 0, 4096);

                while (strcmp(log_user1, "da_user") != 0) {
                    cout << "Introdu username-ul persoanei" << endl;
                    cout << ">";
                    cin.getline(username2, SIZE);
                    if (send(listening, username2, sizeof(username2), 0) <= 0)
                        cerr << "eroare afisare mesaj username";
                    if (recv(listening, log_user1, SIZE, 0) <= 0)
                        cerr << "eroare afisare mesaj client";
                    else if(strcmp(log_user1,"nu_user")==0)
                        cout<<"USER GRESIT!INCEARCA IAR..."<<endl;

                }
                cout << "CE MESAJ VREI SA II TRIMITI?" << endl;
                cin.getline(buf_mesaj_trimis, SIZE);
                if (send(listening, buf_mesaj_trimis, SIZE, 0) <= 0)
                    cerr << "eroare afisare mesaj username";
                else cout << "mesaj trimis" << endl;
                cout<<endl;
                cout << "Buna, " << username << "!" << endl;
                cout<<endl;
                cout << "1.Send messege" << endl;
                cout << "2.Read messege" << endl;
                cout << "3.LOGOUT" << endl;
                cout << "4.EXIT" << endl;
                cout << "SCRIE NUMARUL CORESPUNZATOR COMENZII DIN MENIU:" << endl;

            }
        }
        else if(strcmp(buf,"da")!=0)
        {
            memset(buf, 0, 4096);
            if(recv(listening, buf, sizeof(buf), 0)<=0)
                cerr << "eroare afisare mesaj client";
            if (strcmp(buf, "da") == 0)
            {
                strcpy(username, userInput);
                cout << "Buna, " << username << "!" << endl;
                cout<<endl;
                cout << "1.Send messege" << endl;
                cout << "2.Read messege" << endl;
                cout << "3.LOGOUT" << endl;
                cout << "4.EXIT" << endl;
                cout << "SCRIE NUMARUL CORESPUNZATOR COMENZII DIN MENIU:" << endl;
            } else if (strcmp(buf, "nu") == 0) {
                cout << "USERNAME GRESIT.INCEARCA IAR.." << endl;
                cout << "Introduceti un username: " << endl;
            }
        }
        else {cout<<"Nu ai ales una din comenzile de mai sus"<<endl;
            cout << "SCRIE NUMARUL CORESPUNZATOR COMENZII DIN MENIU:" << endl;}

    }while(strcmp (userInput,"4")!=0);

    cout<<"QUIT"<<endl;


    return 0;
}