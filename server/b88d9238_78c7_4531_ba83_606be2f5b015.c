#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#define FLUSH while(getchar()!='\n')

#define N 100
#define STR 50
int cod=100;

struct ogg{
        char nome[STR], venditore[STR], offerente[STR];
        int base, durata, offerta, codice;
}oggetto[N];

void VisualizzaElementoVend(struct ogg x){
     printf("\n--Info oggetto--");
     printf("\nNome: %s", x.nome);
     printf("\nVenditore: %s", x.venditore);
     printf("\nBase d'Asta: %d", x.base);
     printf("\n\n--Info offerte--");
     printf("\nOfferta Max: %d", x.offerta);
     printf("\nOfferente: %s", x.offerente);
}

void VisualizzaElementoAcq(struct ogg x){
     printf("\n--Info oggetto--");
     printf("\nNome: %s", x.nome);
     printf("\n\n--Info offerte--");
     printf("\nOfferta Max: %d", x.offerta);
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



int main(){

printf("****** SERVER ******\n");

/*instaurazione collegamento*/

struct sockaddr_in server_addr; /* indirizzo del server */
struct sockaddr_in client_addr; /* indirizzo del client */
int sd_server, sd_client; /* i socket descriptor usati per identificare server e client */
// Creazione socket descriptor per il server.
// AF_INET + SOCK_STREAM --> TCP, utilizzo del protocollo TCP (IPPROTO_TCP)
if((sd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                printf("Errore nella creazione del server\n");
/* Inseriamo nella struttura alcune informazioni */
server_addr.sin_family = AF_INET; /* la famiglia dei protocolli */
server_addr.sin_port = htons(1745); /* porta in ascolto */
server_addr.sin_addr.s_addr = INADDR_ANY; /* dato che è un server
bisogna associargli l'indirizzo della macchina su cui sta girando*/
/* Assegnazione del processo alla socket tramite la funzione BIND */
if(bind(sd_server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
                printf("Errore di binding\n");
/* Si mette in ascolto con un massimo di 5 connessioni */
listen (sd_server, 5);
/* Quindi accetterà una sola connessione per volta */
int address_size = sizeof(client_addr); /* dimensione della struttura client_addr */

if((sd_client = accept(sd_server, (struct sockaddr *)&client_addr, & address_size)) < 0)
                printf("Errore nella chiamata accept\n");
printf("Connessione accettata!!!\n");
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* ricezione dati dal client */

int i=0, j, req, cod, flag;

while(1){
        recv(sd_client,&req,sizeof(int),0);

        switch(req){
                case 1:
                        printf("\n\nRichiesta 1 ricevuta!");
                        recv(sd_client,&oggetto[i],sizeof(oggetto[i]),0);
                        printf("\nNome: %s", oggetto[i].nome);
                        printf("\nCodice: %d", oggetto[i].codice);
                        printf("\nBase d'Asta: %d", oggetto[i].base);
                        i++;

                        break;

                case 2:
                        printf("\n\nRichiesta 2 ricevuta!");
                        recv(sd_client,&cod,sizeof(int),0);
                        printf("\nCerco l'oggetto con codice: %d",cod);
                        flag=0;
                        for(j=0; j<100 && flag==0; j++){
                                if(cod==oggetto[j].codice) {
                                        flag=1;
                                        send(sd_client,&oggetto[j],sizeof(oggetto[j]),0);
                                        }
                                }
                        if(j>=100){
                                oggetto[j].durata=0;
                                send(sd_client,&oggetto[j],sizeof(oggetto[j]),0);
                                }
                        break;

                case 3:
                        printf("\n\nRichiesta 3 ricevuta!");
                        int stop=0;
                        for(j=0; j<100; j++){
                                if(oggetto[j].durata==0){
                                        stop=1;
                                        send(sd_client,&stop,sizeof(int),0);
                                        }
                                    else{
                                        send(sd_client,&stop,sizeof(int),0);
                                        send(sd_client,&oggetto[j],sizeof(oggetto[j]),0);
                                        }
                                }

                        break;

                case 4:
                        printf("\n\nRichiesta 4 ricevuta!");
                        recv(sd_client,&cod,sizeof(int),0);
                        flag=0;
                        int pos=0;
                        for(j=0; j<100 && flag==0; j++){
                                if(cod==oggetto[j].codice){
                                        flag=1;
                                        pos=j;
                                        printf("\nNome: %s", oggetto[j].nome);
                                        printf("\nCodice: %d", oggetto[j].codice);
                                        printf("\nBase d'Asta: %d", oggetto[j].base);

                                        }
                                }

                                write(sd_client,&oggetto[pos],sizeof(oggetto[pos]));
                                        read(sd_client,&oggetto[pos],sizeof(oggetto[pos]));

                                        printf("\nNome: %s", oggetto[pos].nome);
                                        printf("\nCodice: %d", oggetto[pos].codice);
                                        printf("\nOfferta: %d", oggetto[pos].offerta);
                                        printf("\nBase d'Asta: %d", oggetto[pos].base);
                                        printf("\nVenditore: %s", oggetto[pos].venditore);

                       /* if(j>=100){
                                oggetto[j].durata=0;
                                send(sd_client,&oggetto[j],sizeof(oggetto[j]),0);
                                }
                         */
                }
        }

}
