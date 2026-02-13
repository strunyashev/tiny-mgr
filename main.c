#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CLS "\e[H\e[J"
#define H_CUR "\e[?25l"
#define RESET "\x1b[0m"
#define RED   "\x1b[31m"
#define GREEN "\x1b[32m"
#define CYAN  "\x1b[36m"

void draw_bar(char *name, float p) {
    int i, f = (int)(20 * (p / 100.0));
    printf("%-10s [", name);
    for (i = 0; i < 20; i++) {
        if (i < f) printf("#");
        else printf("-");
    }
    printf("] %.1f%%\n", p);
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--install") == 0) {
        char cmd[256];
        sprintf(cmd, "sudo cp %s /usr/local/bin/tiny-mgr && sudo chmod +x /usr/local/bin/tiny-mgr", argv[0]);
        if (system(cmd) == 0) printf(GREEN "Installed! Run 'tiny-mgr' to start." RESET "\n");
        return 0;
    }

    unsigned long long rx, tx, old_rx = 0, old_tx = 0;
    char buf[256];
    
    printf(H_CUR CLS);

    while (1) {
        printf("\e[H" CYAN "--- [ TINY-MGR ] ---\n" RESET);

        FILE *f = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
        if (f) {
            int t; fscanf(f, "%d", &t); fclose(f);
            float c = t / 1000.0;
            printf("CPU TEMP:  %s%.1f°C" RESET "\n", (c > 65 ? RED : GREEN), c);
        }

        f = popen("nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader,nounits 2>/dev/null", "r");
        if (f) {
            if (fgets(buf, 10, f)) {
                float gt = atof(buf);
                printf("GPU TEMP:  %s%.1f°C" RESET "\n", (gt > 70 ? RED : GREEN), gt);
            }
            pclose(f);
        }

        f = fopen("/proc/meminfo", "r");
        if (f) {
            long t, a;
            fscanf(f, "MemTotal: %ld kB\n", &t);
            while (fgets(buf, 256, f) && !strstr(buf, "MemAvailable"));
            sscanf(buf, "MemAvailable: %ld kB", &a);
            fclose(f);
            draw_bar("MEMORY:", 100.0 * (t - a) / t);
        }

        f = popen("df / --output=pcent | tail -1", "r");
        if (f) {
            if (fgets(buf, 10, f)) draw_bar("DISK /:", (float)atoi(buf));
            pclose(f);
        }

        f = fopen("/proc/net/dev", "r");
        if (f) {
            unsigned long long rb_now = 0, tb_now = 0, rb, tb;
            fgets(buf, 256, f); fgets(buf, 256, f);
            while (fgets(buf, 256, f)) {
                if (strstr(buf, "lo:")) continue;
                char *delim = strchr(buf, ':');
                if (delim) {
                    sscanf(delim + 1, "%llu %*u %*u %*u %*u %*u %*u %*u %llu", &rb, &tb);
                    rb_now += rb; tb_now += tb;
                }
            }
            fclose(f);

            if (old_rx > 0) {
                printf("DOWN:      " GREEN "%.2f KB/s" RESET "\n", (rb_now - old_rx) / 1024.0);
                printf("UP:        " CYAN "%.2f KB/s" RESET "\n", (tb_now - old_tx) / 1024.0);
            }
            old_rx = rb_now; old_tx = tb_now;
        }

        printf(CYAN "--------------------\n" RESET);
        fflush(stdout);
        sleep(1);
    }
    return 0;
}