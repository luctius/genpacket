#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include <sndfile.h>

#include "cmdline.h"

static float mavg = 0.f;
static float filter = 0.01f;
static struct gengetopt_args_info args_info;

static inline bool above_threshold(void) {
    if (mavg > args_info.threshold_arg) return true;
    return false;
}

static inline void add_sample(float sample) {
    float log_sample = (log10(fabs(sample) ) * 20);
    if (sample == 0) log_sample = 0;

    if (mavg == 0.f) mavg = log_sample;
    mavg = (filter * (log_sample) ) + ((1.0f - filter) * mavg);
    if (args_info.debug_given) printf("sample: %.8g \tlog: %.8g  \t[mavg: %.8g]\t[trigger: %s]\n", sample, log_sample, mavg, (above_threshold() ? "true" : "false") );
}

int main(int argc, char **argv) {
    if (cmdline_parser (argc, argv, &args_info) != 0) exit(EXIT_FAILURE);
    /*squeue = queue_init_simple(args_info->lpf_sz_arg);*/

    SF_INFO sfinfo;
    SNDFILE *sf = sf_open(args_info.file_arg, SFM_READ, &sfinfo);
    if (sf == NULL) {
        fprintf(stderr,"could not open file %s\n", args_info.file_arg);
        return (EXIT_FAILURE);
    }

    bool running = true;
    bool abv_threshold = false;
    int retval = 0;

    uint32_t sr_length = args_info.length_arg * sfinfo.samplerate;
    uint32_t sr_ctr = 0;
    
    float samples[sfinfo.channels];
    while (running && ( (retval = sf_readf_float(sf, samples, 1) ) > 0) ) {
        //for (int i = 0; i < sfinfo.channels; i++) add_sample(samples[i]);
        add_sample(samples[0]);
        sr_ctr++;

        if (above_threshold() ) {
            if (abv_threshold == false) {
                sr_ctr = 0;
                abv_threshold = true;
            }
            else {
                if (args_info.debug_given) printf("timespent: %u [length: %u]\n", sr_ctr , sr_length);

                if (sr_ctr >= sr_length) {
                    if (args_info.action_given) {

                    }
                    else running = false;
                }
            }
        }
        else abv_threshold = false;
    }

    if (retval == -1) fprintf(stderr, "error: %s\n", strerror(errno) );

    /*queue_exit(squeue);*/

    sf_close(sf);

    return (EXIT_SUCCESS);
}
