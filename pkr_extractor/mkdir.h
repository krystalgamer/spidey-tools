#include <sys/stat.h>
#include <sys/types.h>

#define mkdir(path) mkdir((path), 0777)
