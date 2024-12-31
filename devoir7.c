#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define START_PORT 0
#define END_PORT 3000

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <adresse_ip>\n", argv[0]);
        return 1;
    }

    const char* target = argv[1];
    printf("Scan des ports TCP de %s (ports %d-%d)\n", target, START_PORT, END_PORT);

    int ports_trouves = 0;

    // Parcours de tous les ports de START_PORT à END_PORT
    for (int port = START_PORT; port <= END_PORT; port++) {
        int sock;
        struct sockaddr_in addr;
        
        // Création de la socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            printf("Erreur création socket pour port %d: %s\n", port, strerror(errno));
            continue;
        }

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(target);

        // Tentative de connexion
        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
            printf("Connexion réussie au port %d\n", port); // Affichage du message de succès
            ports_trouves++;
        }
        close(sock); // Fermeture de la socket
    }
    
    if (ports_trouves == 0) {
        printf("Aucun port ouvert trouvé\n");
    }

    return 0;
}
