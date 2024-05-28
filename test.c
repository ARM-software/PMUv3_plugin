#include "/home/ubuntu/ut_integration/PMUv3_plugin/pmuv3_plugin_bundle.h"
#include "/home/ubuntu/ut_integration/PMUv3_plugin/pmuv3_plugin_helper_bundle.h"

int main(int argc, char **argv){
  if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
	exit(1);
  }
    num_bundles = atoi(argv[1]);
    //int cur_bundle_no = 8;
    pmuv3_bundle_init(num_bundles);
    
        int k = 0;
        process_start_count(&count_data);
        while(k < 500000000) {
             k++;
        }
        process_end_count(&count_data);

        // Shutdown resources
        shutdown_resources();
        // for(int k = 0; k < num_events; ++k) {
        uint64_t diff = event_counts[0].end_cnt[0] - event_counts[0].start_cnt[0];
        //uint64_t diff = event_counts[0].end_cnt[k] - event_counts[0].start_cnt[k];
        //printf("End is %ld, Start is %ld, diff is %ld\n", event_counts[0].end_cnt[k], event_counts[0].start_cnt[k], diff);
        printf("End is %ld, Start is %ld, CPU_CYCLES is %ld\n", event_counts[0].end_cnt[0], event_counts[0].start_cnt[0], diff);
//}
}

