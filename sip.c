#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int get_decimal_from_ip(char *addr);
int get_linecount();
int intcmp(const void *aa, const void *bb);
const char *get_dotted_quad(int);

int main() {
    FILE *fp = fopen("ips.txt", "r");
    char ipv4_address[16];
    int num_ips = get_linecount(); 
    int ip_addresses[num_ips];
    uint32_t ip32_addrs[num_ips];
    if ( fp == NULL ) {
        fprintf(stderr, "couldn't open file ips.txt\n");
        exit(EXIT_FAILURE);
    }

    printf("\n\nunsorted:\n\n");
    int i = 0;
    while (fscanf(fp, "%s", ipv4_address) != EOF) {
        ip_addresses[i] = htonl(get_decimal_from_ip(ipv4_address));
        printf("%d\n", ip_addresses[i]);
        i++;
    }

    printf("\n\nsorted:\n\n");
    qsort(ip_addresses, num_ips, sizeof(int), intcmp);
    for (i=0; i<num_ips; i++ ) {
        printf("%d\n", ip_addresses[i]);
    }

    printf("\n\nsorted, IPv4 dotted-quad notation:\n\n");
    for (i=0; i<num_ips; i++) {
        ip32_addrs[i] = htonl(ip_addresses[i]);
        printf("%s\n", get_dotted_quad(ip32_addrs[i]));
    }
    fclose(fp);
}

int get_linecount() {
    FILE *f = fopen("ips.txt", "r");
    char c;
    int nl = 0;
    if (f == NULL) {
        fprintf(stderr, "couldn't open file ips.txt\n");
        exit(EXIT_FAILURE);
    }

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') nl++;
    }
    fclose(f);
    return nl;
}

int get_decimal_from_ip(char *addr) {
    struct sockaddr_in sin;
    inet_pton(AF_INET, addr, &(sin.sin_addr));
    int decimal_ip = inet_addr(addr);
    return decimal_ip;
}

const char *get_dotted_quad(int decimal_ip) {
    const char *dotted_quad;
    struct in_addr addr;
    memcpy(&addr, &decimal_ip, sizeof(decimal_ip));
    dotted_quad = inet_ntoa(addr);
    return dotted_quad;
}

int intcmp( const void *aa, const void *bb ) {
    const int *a = aa;
    const int *b = bb;
    return (*a < *b) ? -1 : (*a > *b);
}
