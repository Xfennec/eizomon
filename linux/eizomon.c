#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "hidapi.h"
#include "eizo_ctrl.h"

#define EIZOMON_VERSION         "0.1"

eizo_ctrl_monitor *monitor;

signed char flag_quiet = 0;
signed char flag_debug = 0;
char *arg_screen_mode = NULL;
char *arg_backlight_mode = NULL;
int arg_volume = -1;

void parse_options(int argc, char *argv[])
{
static struct option long_options[] = {
    {"version",     no_argument,       0, 'v'},
    {"quiet",       no_argument,       0, 'q'},
    {"debug",       no_argument,       0, 'd'},
    {"help",        no_argument,       0, 'h'},
    {"mode",        required_argument, 0, 'm'},
    {"backlight",   required_argument, 0, 'b'},
    {"volume",      required_argument, 0, 'V'},
    {0, 0, 0, 0}
};

static char *options_string = "vqdhm:b:V:";
int c,i;
int option_index = 0;

while(1) {
    c = getopt_long (argc, argv, options_string, long_options, &option_index);

    // no more options
    if (c == -1)
        break;

    switch(c) {
        case 'v':
            printf("eizomon version %s\n",EIZOMON_VERSION);
            exit(EXIT_SUCCESS);
            break;

        case 'h':
            printf("eizomon - Eizo monitors control tool\n");
            printf("------------------------------------\n");
            printf("Usage: %s [-vqdhb] [-m mode]\n",argv[0]);
            printf("  -v --version            show version\n");
            printf("  -q --quiet              hides all messages\n");
            printf("  -d --debug              shows all warning/error messages\n");
            printf("  -h --help               this message\n");
            printf("  -m --mode mode          change mode (user1, user2, user3, fps1, fps2, rts, web)\n");
            printf("  -b --backlight on|off   turn the backlight on / off\n");
            printf("  -V --volume 0..30       change volume\n");

            exit(EXIT_SUCCESS);
            break;

        case 'q':
            flag_quiet = 1;
            break;

        case 'd':
            flag_debug = 1;
            break;

        case 'm':
            arg_screen_mode = strdup(optarg);
            break;

        case 'b':
            arg_backlight_mode = strdup(optarg);
            break;

        case 'V':
            arg_volume = strtol(optarg, NULL, 0);
            arg_volume = (arg_volume < 0  ? 0  : arg_volume);
            arg_volume = (arg_volume > 30 ? 30 : arg_volume);
            break;

        case '?':
        default:
            exit(EXIT_FAILURE);
    }
}

if (optind < argc) {
    fprintf(stderr,"Invalid arguments\n");
    exit(EXIT_FAILURE);
}

if (arg_screen_mode &&
    strcmp("user1", arg_screen_mode) &&
    strcmp("user2", arg_screen_mode) &&
    strcmp("user3", arg_screen_mode) &&
    strcmp("fps1", arg_screen_mode) &&
    strcmp("fps2", arg_screen_mode) &&
    strcmp("rts", arg_screen_mode) &&
    strcmp("web", arg_screen_mode)) {
        fprintf(stderr,"Invalid mode \"%s\"\n", arg_screen_mode);
        exit(EXIT_FAILURE);
    }

if (arg_backlight_mode &&
    strcmp("on", arg_backlight_mode) &&
    strcmp("off", arg_backlight_mode)) {
        fprintf(stderr,"Invalid mode \"%s\"\n", arg_backlight_mode);
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[])
{
int detected;

parse_options(argc,argv);

// enumerate
detected = eizo_ctrl_detect(EM_EIZO_FG_2421_VENDORID, EM_EIZO_FG_2421_PRODUCTID, NULL);

if (!detected) {
    fprintf(stderr, "Unable to find any monitor\n");
    return 1;
}

if (!flag_quiet)
    printf("Found %d monitor(s)\n", detected);

monitor = eizo_ctrl_open(EM_EIZO_FG_2421_VENDORID, EM_EIZO_FG_2421_PRODUCTID, NULL);
if (!monitor) {
    fprintf(stderr, "Unable to open monitor, check /dev/hidraw* access rights (or udev rules.d)\n");
    return 2;
}

// Screen mode
if (arg_screen_mode) {
    if (!flag_quiet)
        printf("Switching to %s mode\n", arg_screen_mode);

    if (!strcmp("user1", arg_screen_mode))
        eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_USER1, 0);

    if (!strcmp("user2", arg_screen_mode))
        eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_USER2, 0);

    if (!strcmp("user3", arg_screen_mode))
        eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_USER3, 0);

    if (!strcmp("fps1", arg_screen_mode))
        eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_FPS1, 0);

    if (!strcmp("fps2", arg_screen_mode))
        eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_FPS2, 0);

    if (!strcmp("rts", arg_screen_mode))
        eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_RTS, 0);

    if (!strcmp("web", arg_screen_mode))
        eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_WEB, 0);
}

// Backlight
if (arg_backlight_mode) {
    if (!flag_quiet)
        printf("Switching backlight %s \n", arg_backlight_mode);

    if (!strcmp("on", arg_backlight_mode))
        eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_BACKLIGHT, 1, 0);

    if (!strcmp("off", arg_backlight_mode))
        eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_BACKLIGHT, 0, 0);
}

// Volume
if (arg_volume >= 0) {
    if (!flag_quiet)
        printf("Setting volume to %d\n", arg_volume);
    eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_VOLUME, arg_volume, 0);
}

return 0;
}
