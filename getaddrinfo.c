#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

static int test_getaddrinfo(char *hostname, int type) {
  struct addrinfo hints;
  struct addrinfo *result;
  struct addrinfo *res;
  int error;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = type;
  /* resolve the domain name into a list of addresses */
  error = getaddrinfo(hostname, NULL, &hints, &result);
  if (error != 0) {
    if (error == EAI_SYSTEM) {
      perror("getaddrinfo");
    } else {
      fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
    }
    return 0;
  }

  /* loop over all returned results and print them */
  for (res = result; res != NULL; res = res->ai_next) {
    char ip[256];
    switch (res->ai_addr->sa_family) {
      case AF_INET:
        inet_ntop(AF_INET, &(((struct sockaddr_in *) res->ai_addr)->sin_addr), ip, 255);
        break;
      case AF_INET6:
        inet_ntop(AF_INET6, &(((struct sockaddr_in6 *) res->ai_addr)->sin6_addr), ip, 255);
        break;
      default:
        strcpy(ip, "???");
        break;
    }
    fprintf(stderr, "%s\n", ip);
  }

  freeaddrinfo(result);
  return 1;
}

int main() {
  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", AF_UNSPEC);
  fprintf(stderr, "Testing IPv6 only with AF_INET\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", AF_INET);
  fprintf(stderr, "Testing IPv6 only with AF_INET6\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", AF_INET6);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", AF_UNSPEC);
  fprintf(stderr, "Testing IPv4/6 with AF_INET\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", AF_INET);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", AF_INET6);
  return 0;
}
