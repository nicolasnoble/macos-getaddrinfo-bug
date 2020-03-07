#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

static int test_getaddrinfo(char *hostname, int havehints, int family, int flags) {
  struct addrinfo hints;
  struct addrinfo *result;
  struct addrinfo *res;
  int error;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = family;
  hints.ai_flags = flags;
  /* resolve the domain name into a list of addresses */
  error = getaddrinfo(hostname, NULL, havehints ? &hints : NULL, &result);
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
  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC, with no hints:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 0, AF_UNSPEC, 0);
  fprintf(stderr, "Testing IPv6 only with AF_INET, with no hints:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 0, AF_INET, 0);
  fprintf(stderr, "Testing IPv6 only with AF_INET6, with no hints:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 0, AF_INET6, 0);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC, with no hints:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 0, AF_UNSPEC, 0);
  fprintf(stderr, "Testing IPv4/6 with AF_INET, with no hints:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 0, AF_INET, 0);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6, with no hints:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 0, AF_INET6, 0);

  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC, with hints and default flags:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_UNSPEC, 0);
  fprintf(stderr, "Testing IPv6 only with AF_INET, with hints and default flags:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET, 0);
  fprintf(stderr, "Testing IPv6 only with AF_INET6, with hints and default flags:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET6, 0);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC, with hints and default flags:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_UNSPEC, 0);
  fprintf(stderr, "Testing IPv4/6 with AF_INET, with hints and default flags:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET, 0);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6, with hints and default flags:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET6, 0);

  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC, with hints and AI_ALL:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_UNSPEC, AI_ALL);
  fprintf(stderr, "Testing IPv6 only with AF_INET, with hints and AI_ALL:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET, AI_ALL);
  fprintf(stderr, "Testing IPv6 only with AF_INET6, with hints and AI_ALL:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET6, AI_ALL);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC, with hints and AI_ALL:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_UNSPEC, AI_ALL);
  fprintf(stderr, "Testing IPv4/6 with AF_INET, with hints and AI_ALL:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET, AI_ALL);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6, with hints and AI_ALL:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET6, AI_ALL);

  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC, with hints and AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_UNSPEC, AI_V4MAPPED);
  fprintf(stderr, "Testing IPv6 only with AF_INET, with hints and AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET, AI_V4MAPPED);
  fprintf(stderr, "Testing IPv6 only with AF_INET6, with hints and AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET6, AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC, with hints and AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_UNSPEC, AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_INET, with hints and AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET, AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6, with hints and AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET6, AI_V4MAPPED);

  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC, with hints and AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_UNSPEC, AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv6 only with AF_INET, with hints and AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET, AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv6 only with AF_INET6, with hints and AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET6, AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC, with hints and AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_UNSPEC, AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_INET, with hints and AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET, AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6, with hints and AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET6, AI_ALL | AI_V4MAPPED);

  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC, with hints and AI_ADDRCONFIG:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_UNSPEC, AI_ADDRCONFIG);
  fprintf(stderr, "Testing IPv6 only with AF_INET, with hints and AI_ADDRCONFIG flags:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET, AI_ADDRCONFIG);
  fprintf(stderr, "Testing IPv6 only with AF_INET6, with hints and AI_ADDRCONFIG flags:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET6, AI_ADDRCONFIG);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC, with hints and AI_ADDRCONFIG flags:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_UNSPEC, AI_ADDRCONFIG);
  fprintf(stderr, "Testing IPv4/6 with AF_INET, with hints and AI_ADDRCONFIG flags:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET, AI_ADDRCONFIG);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6, with hints and AI_ADDRCONFIG flags:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET6, AI_ADDRCONFIG);

  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC, with hints and AI_ADDRCONFIG | AI_ALL:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_UNSPEC, AI_ADDRCONFIG | AI_ALL);
  fprintf(stderr, "Testing IPv6 only with AF_INET, with hints and AI_ADDRCONFIG | AI_ALL:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET, AI_ADDRCONFIG | AI_ALL);
  fprintf(stderr, "Testing IPv6 only with AF_INET6, with hints and AI_ADDRCONFIG | AI_ALL:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET6, AI_ADDRCONFIG | AI_ALL);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC, with hints and AI_ADDRCONFIG | AI_ALL:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_UNSPEC, AI_ADDRCONFIG | AI_ALL);
  fprintf(stderr, "Testing IPv4/6 with AF_INET, with hints and AI_ADDRCONFIG | AI_ALL:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET, AI_ADDRCONFIG | AI_ALL);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6, with hints and AI_ADDRCONFIG | AI_ALL:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET6, AI_ADDRCONFIG | AI_ALL);

  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC, with hints and AI_ADDRCONFIG | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_UNSPEC, AI_ADDRCONFIG | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv6 only with AF_INET, with hints and AI_ADDRCONFIG | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET, AI_ADDRCONFIG | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv6 only with AF_INET6, with hints and AI_ADDRCONFIG | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET6, AI_ADDRCONFIG | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC, with hints and AI_ADDRCONFIG | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_UNSPEC, AI_ADDRCONFIG | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_INET, with hints and AI_ADDRCONFIG | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET, AI_ADDRCONFIG | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6, with hints and AI_ADDRCONFIG | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET6, AI_ADDRCONFIG | AI_V4MAPPED);

  fprintf(stderr, "Testing IPv6 only with AF_UNSPEC, with hints and AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_UNSPEC, AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv6 only with AF_INET, with hints and AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET, AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv6 only with AF_INET6, with hints and AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback6.unittest.grpc.io", 1, AF_INET6, AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_UNSPEC, with hints and AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_UNSPEC, AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_INET, with hints and AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET, AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED);
  fprintf(stderr, "Testing IPv4/6 with AF_INET6, with hints and AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED:\n");
  test_getaddrinfo("loopback46.unittest.grpc.io", 1, AF_INET6, AI_ADDRCONFIG | AI_ALL | AI_V4MAPPED);

  return 0;
}
