// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.cpp                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: iedderqi <iedderqi@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/10/26 14:23:56 by iedderqi          #+#    #+#             */
/*   Updated: 2023/11/04 16:27:46 by iedderqi         ###   ########.fr          */
// /*                                                                            */
// /* ************************************************************************** */

// 1.1.7.1.3.5.8.3.8

#include "servers.hpp"

///multi serveurs whit multi clients 
//TODO 
///service_name     
///chankese request
///merege requset whit serveur 

void feedRequest(unsigned int index, std::map<unsigned int , server> &serv, std::string content)
{
    std::map<unsigned int , server>::iterator it = serv.begin();
    while (it != serv.end())
    {
        if (it->first == index)
        {
            it->second.request_content = content;
            break;
        }
        it++;
    }
    
}

void ports(std::vector<int> &port, std::map<unsigned int , server> &data_serv)
{
    std::map<unsigned int , server>::iterator itb = data_serv.begin();
    std::map<unsigned int , server>::iterator ite = data_serv.end();
    while (itb != ite)
    {
        port.push_back((itb->second.port));
        std::cout << (itb->second.port) << std::endl;
        itb++;
    }
}
int main(int ac,const char **av)
{
    // how to serveu run the port serveur
    // create multi serveur
    //  innsilit port
    (void)ac;
    try
    {
    /////////////////////////////
    ConfigParser data_conf(av);//
    data_conf.readConfigFile();//
    data_conf.checkBrackets();///
    /////////////////////////////
    std::vector<int> port;
    std::vector<int> file;
    std::vector<struct sockaddr_in> addresses;
    std::vector<socklen_t> addresselent;
    std::vector<int> new_cont;
    ports(port, data_conf.m_servers);
    for (unsigned int i = 0; i < data_conf.getNumber_ofServers(); i++)
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
    for (size_t i = 0; i < file.size(); i++) {
        struct pollfd poll;
        poll.fd = file[i];
        poll.events = POLLIN;
        fds.push_back(poll);
    }

    while (true)
    {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1)
        {
            perror("POOLLL failed :");
            return (0);
        }

        for (size_t i = 0; i < fds.size(); i++)
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
                        puts("this is the server");
                        std::cout << buf;
                        //TODO feed Request using feedRequest(); 

                    }
                    // messgae wssel
                }
            }
            // close(co);
        }
    }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
