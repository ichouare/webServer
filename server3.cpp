#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sock=addr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <iostream>     // For cout
#include <unistd.h>     // For read
#include <vector>
#include <poll.h>
#include <fcntl.h>
// 1.1.7.1.3.5.8.3.8



///multi serveurs whit multi clients 
//TO   DO 
///service_name     
///chankese request
///merege requset whit serveur 

int main(int ac, char **av)
{
    // how to serveu run the port serveur
    // create multi serveur
    //  innsilit port
    std::vector<int> port;
    std::vector<int> file;
    std::vector<struct sockaddr_in> addresses;
    std::vector<socklen_t> addresselent;
    std::vector<int> new_cont;
    port.push_back(800);
    port.push_back(9080);
    for (int i = 0; i < 2; i++)
    {
        struct sockaddr_in adrese;
        socklen_t addrlen = sizeof(adrese);
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0)
        {
            perror("socket:felaid ");
            exit(0);
        }
        fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);///non-blocking file descriptors 
        adrese.sin_addr.s_addr = INADDR_ANY;
        adrese.sin_family = AF_INET;
        adrese.sin_port = htons(port[i]);
        if (bind(fd, (struct sockaddr *)&adrese, sizeof(adrese)) < 0)
        {
            perror("bind Failed :");
            exit(0);
        }
        if (listen(fd, 3) < 0)
        {
            perror("listen Failed : ");
            exit(0);
        }
        file.push_back(fd);
        addresses.push_back(adrese);
        addresselent.push_back(addrlen);
    }
    std::vector<struct pollfd> fds;
    for (int i = 0; i< file.size(); i++) {
        struct pollfd poll;
        poll.fd = file[i];
        poll.events = POLLIN;
        fds.push_back(poll);
    }

    while (true)
    {
        // int co;
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1)
        {
            perror("POOLLL failed :");
            return (0);
        }

        for (int i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {

                if (std::find(file.begin(), file.end(), fds[i].fd) != file.end())
                {
                    std::cout << "hello new\n"
                              << std::endl;
                    int co = accept(fds[i].fd, NULL, NULL);
                    fcntl(co, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
                    if (co < 0)
                    {
                        perror("Faild ::  acceot");
                        continue;
                    }
                    struct pollfd fl;
                    fl.fd = co;
                    fl.events = POLLIN;
                    fds.push_back(fl);
                }
                else
                {
                    // client bye bye

                    char buf[1024];
                    bzero(buf, 1024);
                    int rec = recv(fds[i].fd, buf, 1024, 0);
                    if (rec < 0)
                    {
                        perror("recv");
                        exit(1);
                    }
                    if (rec == 0)
                    {
                        fds.erase(fds.begin() + i);
                        std::cout << "bybye" << std::endl;
                    }
                    else
                    {

                        std::cout << buf;
                    }
                    // messgae wssel
                    //add my response to the server
                    // buff = myresponse();
                    send(fds[i].fd, buf, rev + 1, 0);
                }
            }
            // close(co);
        }
    }

    return (0);
}