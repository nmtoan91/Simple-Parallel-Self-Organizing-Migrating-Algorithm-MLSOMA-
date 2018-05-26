# Simple-Parallel-Self-Organizing-Migrating-Algorithm-MLSOMA-
SOMA Algorithm: Prof. Ivan Zelinka
MLSOMA version 1.0.2 :
 -m    : Method (0|1)
 [default = 0] -pop  : Pop size [default = 16]
 -step : Migration step (float) [default = 1073741824]
 -plen : Path length (float) [default = 5.000000]
 -range: Range (bound) (float) [default = 512.000000]
 -d    : Dimensions [default = 32]
 -grid : Grid count per dimension [default = 2]
 -w    : Set write the migrations [default = b]
 -ng   : Number SOMA groups for each node
 [default = 2] -type : SOMA type(0:Allto1;1:AlltoAll) [default = 1]
 -op0  : Optimization method 0 [default = b]
 -op1  : Optimization method 1 [default = b]
        -op1thresh  : Config the threashold [default = 0.010000]
 -op2  : Fix number of leaders for each individuals [default = b]
        -op2n  : Config number of leader [default = 2]
 -tf  : testfunction [default = 0]
 -help : Help!
 
 Examples:
 
Run MLSOMA on single processor (512 dimensions, 16 pop-size, range from -512 to 512):
	./MLSOMA -m 0 -d 512 -pop 16 -range 512
Run MLSOMA on multiple processors with mpiexec (32 cpu cpu-processors, 512 dimensions, 16 pop-size, range from -512 to 512):
    mpiexec -n 32 ./MLSOMA -m 1 -d 512 -pop 16 -range 512
