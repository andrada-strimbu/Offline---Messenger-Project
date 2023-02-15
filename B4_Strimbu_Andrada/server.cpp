#include "json.hpp"
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include "jsoncpp/json/json.h"
#define PORT 2908
#define SIZE 4096
using namespace  std;
using json = nlohmann::json;
ofstream fout("../jsondetalii.json");

typedef struct thData{
    int idThread;
    int cl;
    int status;
}thData;
int login(char *buf)
{ FILE *login;
    char name[SIZE];
    int find=0;
    login=fopen("/home/andrada/Desktop/Retele/proiect_retele1/username.txt","r");
    if(login== nullptr)
        cerr<<"nu se deschide fisierul";
    while(!feof(login))
    {
        fscanf(login,"%s",name);
        if(strcmp(name,buf)==0)
        {
            find=1;
            break;
        }
    }
    fclose(login);
    if(find==1)
        return 1;
    else return 0;

}

void send_messege(char *username,void*arg){
    struct thData client;
    char username2[SIZE];
    char mesaj_primit[SIZE];
    client = *((struct thData *) arg);
   if (recv(client.cl, username2, 4096, 0)<=0)
       cerr<<"eroare afisare mesaj";
   else {
       cout << "am primit..." << username2 << endl;
       if (login(username2) ==1) {
           if( send(client.cl, "da_user", sizeof("da_user"), 0)<=0)
               cerr<<"eroare afisare mesaj username";
           char a[SIZE];
           if (recv(client.cl, mesaj_primit, SIZE, 0) <= 0)
               cerr << "eroare afisare mesaj";
           cout << "<" << "mesaj primit" << ">: " << mesaj_primit << endl;

           ifstream conversatie("../json_conversatii.json");
           Json::Value root;
           if (conversatie.is_open()) {
               conversatie>> root;
           }
           conversatie.close();
           ofstream fout("../json_conversatii.json");
           Json::Value mesaj;
           mesaj["username2"] = username2;
           mesaj["username1"] = username;
           mesaj["mesaj"] = mesaj_primit;
           Json::Value trimitere = root.get("conversatii", Json::Value());
           trimitere.append(mesaj);
           root["conversatii"] = trimitere;
           Json::StyledWriter styledWriter;
           fout << styledWriter.write(root);
       }else{ if( send(client.cl, "nu_user", sizeof("nu_user"), 0)<=0)
           cerr<<"eroare afisare mesaj username";
              void(send_messege( username,arg));}

   }
}
void read_messege(char *username, void *arg)
{
    Json::Reader reader;
    Json::Value root;
    int i;
    struct thData client;
    char username2[SIZE];
    client= *((struct thData *) arg);
    if (recv(client.cl, username2, 4096, 0)<=0)
        cerr<<"eroare afisare mesaj";
    else {
        cout << "am primit..." << username2 << endl;
        if (login(username2) ==1) {
            if( send(client.cl, "da_user", sizeof("da_user"), 0)<=0)
                cerr<<"eroare afisare mesaj username";

    ifstream conversatii("../json_conversatii.json");
    reader.parse(conversatii, root, false);
    auto  mesaj_brut = root["conversatii"];
    for(i=0;i<root["conversatii"].size() ;i++)
    {   auto mesaje = mesaj_brut[i];
        string sender = mesaje["username1"].asString();
        string recever = mesaje["username2"].asString();
        string mesaj = mesaje["mesaj"].asString();
        const char* sender_char = sender.c_str();
        const char* recever_char = recever.c_str();
        const char* mesaj_char = mesaj.c_str();
        if((strcmp(sender_char,username)==0 && strcmp(recever_char,username2)==0)||(strcmp(sender_char,username2)==0 && strcmp(recever_char,username)==0))
        {

            if (send(client.cl,sender_char, SIZE,0) <= 0) {
                cout << "[Thread %d]" << client.idThread << endl;
                cerr << "eroare scriere catre client " << endl;
                //   memset(sender_, 0, 4096);
            } else
                cout << "[Thread " << client.idThread << "]Mesajul a fost trimis..." <<sender_char<<endl;

            if ( send(client.cl,mesaj_char, SIZE,0) <= 0) {
                cout << "[Thread %d]" << client.idThread << endl;
                cerr << "eroare scriere catre client " << endl;
                // memset(, 0, 4096);
               } else
                   cout << "[Thread " << client.idThread << "]Mesajul a fost trimis..." <<mesaj_char<<endl;
            cout<<sender_char<<": "<<mesaj_char<<endl;
        }
       }
            if (send(client.cl, "127.0.0.1", SIZE, 0) <= 0)
                cerr << "eroare afisare mesaj username";
            else cout<<"127.0.0.1"<<endl;
        }else{ if( send(client.cl, "nu_user", sizeof("nu_user"), 0)<=0)
                cerr<<"eroare afisare mesaj username";
            void(read_messege( username,arg));
             }
    }

}

void raspunde(void *arg)
{
    char buf[SIZE], username[SIZE];
    struct thData client;
    struct  thData *verificare_conectare;
    client = *((struct thData *) arg);
    verificare_conectare = ((struct thData *)arg);
    do {
        memset(buf, 0, 4096);
        int read_client = recv(client.cl, buf, 4096, 0);
        if (read_client == -1) {
            cout << "[Thread %d]aici" << client.idThread << client.cl << endl;
            cerr << "eroare citire de la client " << endl;
        }
        if(verificare_conectare->status==0)
            if (login(buf) == 1)
            {
                strcpy(username,buf);
                strcpy(buf, "da");
                verificare_conectare->status=1;
                cout << "[Thread " << client.idThread << "]trimitem mesajul inapoi..." << buf << endl;
                if (send(client.cl, buf, sizeof(buf), 0) <= 0) {
                    cout << "[Thread %d]" << client.idThread << endl;
                    cerr << "eroare scriere catre client " << endl;
                    memset(buf, 0, 4096);
                } else
                    cout << "[Thread " << client.idThread << "]Mesajul a fost trimis..." << endl;
            }
            else {
                // memset(buf, 0, 4096);
                strcpy(buf, "nu");
                cout << "[Thread " << client.idThread << "]trimitem mesajul inapoi..." << buf << endl;
                if (send(client.cl, buf, sizeof(buf), 0) <= 0) {
                    cout << "[Thread %d]" << client.idThread << endl;
                    cerr << "eroare scriere catre client " << endl;
                    memset(buf, 0, 4096);
                } else
                    cout << "[Thread " << client.idThread << "]Mesajul a fost trimis..." << endl;
            }
        if(strcmp(buf,"3")==0)
            verificare_conectare->status=0;
        else if(strcmp(buf,"1")==0)
        {send_messege(username,(struct thData *) arg);
            memset(buf,0,4096);
        }
        else if(strcmp(buf,"2")==0)
        { read_messege(username,(struct thData *) arg );
            memset(buf,0,4096);
        }
    }while(true);

}

static void *treat(void* arg)
{
    struct thData tdL;
    struct thData *verificare_conectare;
    tdL=*((struct thData*)arg);
    verificare_conectare = ((struct thData *)arg);
    verificare_conectare->status = 0;
    cout<<"[thread]-"<<tdL.idThread<<"-Asteptam mesajul..."<<endl;
    fflush(stdout);
    pthread_detach(pthread_self());
    raspunde((struct thData *) arg);
    return(nullptr);
}
int main() {

    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        cerr << "[server]Eroare la socket!";
        return -1;
    }
    struct sockaddr_in server;
    struct sockaddr_in from;
    pthread_t th[100];
    int i=0;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr= htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "0.0.0.0", &server.sin_addr);
    if (bind(listening, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        cerr << "[server]nu pot face bind la ip/port";
        return -2;
    }
    if (listen(listening, 2) == -1) {
        cerr << "[server]EROARE LA LISTEN()"<<endl;
        return -3;
    }
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);
    while(true)
    {   int client;
        thData * td;
        socklen_t length = sizeof (from);
        cout<<"[server]Asteptam la portul "<<PORT<<" ..."<<endl;
        fflush(stdout);
        if ((client = accept (listening, (struct sockaddr *) &from, &length)) == -1)
        {
            cerr << "[server]Eroare la accept()";
            continue;
        }
        td=(struct thData*)malloc(sizeof(struct thData));
        td->idThread=i++;
        td->cl=client;
        pthread_create(&th[i], nullptr, &treat,td);


    }
}