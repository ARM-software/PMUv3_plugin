cd /home/ubuntu/ut_integration
pushd linux/tools/lib/perf
make
popd; pushd linux/tools/lib/api
make
popd

cd /home/ubuntu/ut_integration/PMUv3_plugin

cp /home/ubuntu/ut_integration/linux/tools/lib/perf/libperf.a .
cp /home/ubuntu/ut_integration/linux/tools/lib/api/libapi.a .

gcc -c pmuv3_plugin.c -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o pmuv3_plugin.o
gcc -c pmuv3_plugin_bundle.c -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o pmuv3_plugin_bundle.o
gcc -c processing.cpp -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o processing.o

ar rcs libpmuv3_plugin.a pmuv3_plugin.o
ar rcs libpmuv3_plugin_bundle.a pmuv3_plugin_bundle.o processing.o

