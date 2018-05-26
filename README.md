# Simple-Parallel-Self-Organizing-Migrating-Algorithm-MLSOMA-
MLSOMA version 1.0.2 :<br />
 -m    : Method (0|1)<br />
 [default = 0] -pop  : Pop size [default = 16]<br />
 -step : Migration step (float) [default = 1073741824]<br />
 -plen : Path length (float) [default = 5.000000]<br />
 -range: Range (bound) (float) [default = 512.000000]<br />
 -d    : Dimensions [default = 32]<br />
 -grid : Grid count per dimension [default = 2]<br />
 -w    : Set write the migrations [default = b]<br />
 -ng   : Number SOMA groups for each node<br />
 [default = 2] -type : SOMA type(0:Allto1;1:AlltoAll) [default = 1]<br />
 -op0  : Optimization method 0 [default = b]<br />
 -op1  : Optimization method 1 [default = b]<br />
        -op1thresh  : Config the threashold [default = 0.010000]<br />
 -op2  : Fix number of leaders for each individuals [default = b]<br />
        -op2n  : Config number of leader [default = 2]<br />
 -tf  : testfunction [default = 0]<br />
 -help : Help!<br />
 <br />
 Examples:<br />
 <br />
Run MLSOMA on single processor (512 dimensions, 16 pop-size, range from -512 to 512):<br />
	./MLSOMA -m 0 -d 512 -pop 16 -range 512<br />
Run MLSOMA on multiple processors with mpiexec (32 cpu cpu-processors, 512 dimensions, 16 pop-size, range from -512 to 512):<br />
    mpiexec -n 32 ./MLSOMA -m 1 -d 512 -pop 16 -range 512<br />


<br />
<br />
Zelinka, Ivan. "SOMA—self-organizing migrating algorithm." New optimization techniques in engineering. Springer, Berlin, Heidelberg, 2004. 167-217.
