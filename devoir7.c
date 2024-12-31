include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define START_PORT 0
#define END_PORT 3000

int try_connect(const char* target, int port) {
    int sock;
    struct sockaddr_in addr;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Erreur création socket pour port %d: %s\n", port, strerror(errno));
        return 0;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(target);

    // Tentative de connexion avec affichage des erreurs
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        printf("Connexion réussie au port %d\n", port);
        close(sock);
        return 1;
    } else {
        // printf("Erreur connexion port %d: %s\n", port, strerror(errno));
    }

    close(sock);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <adresse_ip>\n", argv[0]);
        return 1;
    }

    const char* target = argv[1];
    printf("Scan des ports TCP de %s (ports %d-%d)\n", target, START_PORT, END_PORT);
    printf("Ports ouverts:\n");

    int ports_trouves = 0;
    for (int port = START_PORT; port <= END_PORT; port++) {
        if (try_connect(target, port)) {
            printf("%d\n", port);
            ports_trouves++;
            fflush(stdout);
        }
    }
    
    if (ports_trouves == 0) {
        printf("Aucun port ouvert trouvé\n");
    }

    return 0;
}





