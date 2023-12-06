#include "./webserver.hpp"



std::string Response::get_Content_type(std::string url)
{

    std::string contentTypes[25] = {
        "text/plain",
        "text/html",
        "text/css",
        "text/javascript",
        "text/xml",
        "application/json",
        "application/xml",
        "application/xhtml+xml",
        "application/pdf",
        "application/octet-stream",
        "image/jpeg",
        "image/png",
        "image/jpg",
        "image/gif",
        "image/svg+xml",
        "audio/mpeg",
        "audio/wav",
        "video/mp4",
        "video/webm",
        "multipart/form-data",
        "multipart/byteranges",
        "application/x-www-form-urlencoded",
        "application/graphql",
        "application/vnd.api+json",
    };
    std::string extension = url.substr(url.rfind(".") + 1);
    unsigned int i = 0;
    while (i < 25)
    {
        if (contentTypes[i].find(extension) != std::string::npos)
            return contentTypes[i];
        i++;
    }

    return "text/html;";
}


int Response::get_permission(std::string &file)
{
    if (access(file.c_str(), F_OK) == -1)
        return -1;
    if (access(file.c_str(), R_OK) == -1)
        return -2;
    return 0;
}


std::string Response::read_file(const std::string &filename)
{
    std::ifstream file(filename);
    std::stringstream ss;
    if (!file.is_open())
    {
        return "not found";
    }
    ss << file.rdbuf();
    file.close();
    return ss.str();
}


void Response::ft_default_pages(std::string status, std::string &message, std::string &path)
{
    struct stat buffer;
    std::string filename;
    if (stat(this->response_items.error_pages.find(status)->second.c_str(), &buffer) != -1)
    {
        // if (path[0] != '/')
        //     path = "/" + path;
        
        filename = path;
        message = this->read_file(filename);
    }
}