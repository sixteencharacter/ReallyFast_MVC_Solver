import os
import glob
paths = glob.glob("data/input/*")

path_in = [p.replace("data/","/") for p in paths]
path_out = [p.replace("/input/","/output/") for p in path_in]

for (pin,pout) in zip(path_in,path_out) :
    print("Running {} -> {}".format(pin,pout))
    os.system("./run_local_test.sh {} {}".format(pin,pout))
    print("Done!")