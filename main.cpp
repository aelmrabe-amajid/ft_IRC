// #include <iostream>
// #include <cstring>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>

// int main() {
//     int sockfd;
//     struct sockaddr_in addr;
//     char ip[INET_ADDRSTRLEN];

//     // Create socket
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0) {
//         perror("socket");
//         return 1;
//     }

//     // Connect to an external address (Google's public DNS server)
//     struct sockaddr_in remote_addr;
//     memset(&remote_addr, 0, sizeof(remote_addr));
//     remote_addr.sin_family = AF_INET;
//     remote_addr.sin_port = htons(53); // DNS port
//     remote_addr.sin_addr.s_addr = inet_addr("8.8.8.8");

//     if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) < 0) {
//         perror("connect");
//         close(sockfd);
//         return 1;
//     }

//     // Get the socket name to retrieve the local IP address
//     socklen_t addr_len = sizeof(addr);
//     if (getsockname(sockfd, (struct sockaddr *)&addr, &addr_len) < 0) {
//         perror("getsockname");
//         close(sockfd);
//         return 1;
//     }

//     // Convert the IP address to a string
//     const char* ip_str = inet_ntoa(addr.sin_addr);
//     if (ip_str == nullptr) {
//         perror("inet_ntoa");
//         close(sockfd);
//         return 1;
//     }
//     strncpy(ip, ip_str, INET_ADDRSTRLEN);

//     std::cout << "IP Address: " << ip << std::endl;

//     close(sockfd);
//     return 0;
// }