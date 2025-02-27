#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return -1;
    }

    // Привязка сокета к порту
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Ошибка установки опций сокета" << std::endl;
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        return -1;
    }

    // Прослушивание входящих соединений
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Ошибка прослушивания" << std::endl;
        return -1;
    }

    std::cout << "Ожидание подключения..." << std::endl;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Ошибка принятия соединения" << std::endl;
        return -1;
    }

    // Чтение и отправка сообщений
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread > 0) {
            std::cout << "Сообщение от клиента: " << buffer << std::endl;
            std::string response = "Сообщение получено: " + std::string(buffer);
            send(new_socket, response.c_str(), response.size(), 0);
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
