#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <logging.h>


void usage()
{
    std::cout <<
                R"(
./cp src dest
src - source file to be copied from.
dest - destination file to be copied into.
    )";
}
// cp src dest
int main(int argc, char** argv)
{
    if(argc != 3)
    {
        usage();
        return EXIT_FAILURE;
    }
    // open read write close.
    const int copyFromFD = open(argv[1], O_RDONLY);
    if(copyFromFD < 0)
    {
        log_fatal(errno);
    }

    constexpr int standardMode =
            S_IRUSR | S_IWUSR |
            S_IRGRP | S_IWGRP |
            S_IROTH;
    constexpr int copyFlags = O_WRONLY | O_CREAT;
    const int copyToFD = open(argv[2], copyFlags, standardMode);
    if(copyToFD < 0)
    {
        log_fatal(errno);
    }

    std::vector<char> buffer(4096);
    int bytesRead = read(copyFromFD, buffer.data(),buffer.size());
    if(bytesRead < 0)
    {
        log_fatal(errno);
    }
    while(bytesRead != 0)
    {
        int bytesWritten = write(copyToFD, buffer.data(), bytesRead);
        if(bytesWritten < 0)
        {
            log_fatal(errno);
        }
        bytesRead = read(copyFromFD, buffer.data(),buffer.size());
        if(bytesRead < 0)
        {
            log_fatal(errno);
        }
    }

    if(close(copyToFD) < 0)
    {
        log_fatal(errno);
    }
}