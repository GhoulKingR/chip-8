#ifndef CONFIG_HPP_
#define CONFIG_HPP_

static const int DISPLAY_WIDTH  = 512;      // scale 8 : 1
static const int DISPLAY_HEIGHT = 256;      // scale 8 : 1
static const int RAMSIZE        = 4096;

#define PROGRAM_BEGIN 0x200

#define SEND_ERR(str, ...) \
    fprintf(stderr, "Error: " str "\n" __VA_OPT__(,) __VA_ARGS__);

#define SEND_FAILED(...) \
    SEND_ERR(__VA_ARGS__);\
    exit(EXIT_FAILURE);

#ifdef DEBUG
#define DEBUG_LOG(str, ...) \
    printf("Debug: " str "\n" __VA_OPT__(,) __VA_ARGS__)
#else
#define DEBUG_LOG(str, ...) 0
#endif

#endif
