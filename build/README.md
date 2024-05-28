#test.c is a file to test the working functionality of PMUV3_plugin. On N1 machine, it gives 3000000000 as CPU CYCLES. 
#NOTE: This test can be applicable only to N1 ampere. However, the plugin works on all ARM servers. 
#Run the binary test with any argument between 0 and 14 since there are 15 bundles that are supported. 
#Examples

./test 0

./test 14

NOTE:If it works on N1, this plugin can be used on G2,G3,N2,G4 as well. 

#OUTPUT ON N1 looks like below,
End is 3007928490, Start is 136649, CPU_CYCLES is 3007791841
