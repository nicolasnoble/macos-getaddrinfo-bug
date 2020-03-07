#ifdef _WIN32

#pragma comment (lib, "Ws2_32.lib")
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#undef UNICODE

#include <winsock2.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>

#else

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int test_getaddrinfo(char *hostname, int havehints, int family, int flags) {
#ifdef _WIN32
  const unsigned size = sizeof(ADDRINFOA);
  ADDRINFOA hints;
  PADDRINFOA result;
  PADDRINFOA res;

  WSADATA wsaData;
  int status = WSAStartup(MAKEWORD(2, 0), &wsaData);

  if (status != 0) return 1;
#else
  const unsigned size = sizeof(struct addrinfo);
  struct addrinfo hints;
  struct addrinfo *result;
  struct addrinfo *res;
#endif
  int error;

  memset(&hints, 0, size);
  hints.ai_family = family;
  hints.ai_flags = flags;
  /* resolve the domain name into a list of addresses */
  error = getaddrinfo(hostname, NULL, havehints ? &hints : NULL, &result);
#ifdef _WIN32
  if (error != 0) {
    LPTSTR message;
    DWORD status = FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, (DWORD)WSAGetLastError(), MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
      (LPTSTR)(&message), 0, NULL);
      fprintf(stderr, "error in getaddrinfo: %s\n", message);
      LocalFree(message);
    }
#else
  if (error != 0) {
    if (error == EAI_SYSTEM) {
      perror("getaddrinfo");
    } else {
      fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
    }
    return 0;
  }
#endif

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
