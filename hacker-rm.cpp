#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

void handleError(const std::string &message) {
    std::cerr << message << std::endl;
    exit(EXIT_FAILURE);
}

void overwriteFile(const std::string &filePath) {
    int fd = open(filePath.c_str(), O_RDWR);
    if (fd == -1)
        handleError("Error opening file: " + std::string(strerror(errno)));

    struct stat fileStats;
    if (fstat(fd, &fileStats) == -1) {
        close(fd);
        handleError("Error getting file stats: " + std::string(strerror(errno)));
    }

    off_t fileSize = fileStats.st_size;
    char nullByte = '\0';  // The byte used for overwriting the file

    for (off_t i = 0; i < fileSize; ++i) {
        if (write(fd, &nullByte, 1) != 1) {
            close(fd);
            handleError("Error writing to file: " + std::string(strerror(errno)));
        }
    }

    close(fd);
}

void deleteFile(const std::string &filePath) {
    if (unlink(filePath.c_str()) == -1)
        handleError("Error deleting file: " + std::string(strerror(errno)));

    std::cout << "File " << filePath << " is successfully deleted." << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2)
        handleError("Usage: hacker-rm <filePath>");

    std::string filePath = argv[1];

    struct stat fileStats;
    if (stat(filePath.c_str(), &fileStats) == -1)
        handleError("File does not exist or is unavailable: " + std::string(strerror(errno)));

    overwriteFile(filePath);
    deleteFile(filePath);

    return 0;
}
