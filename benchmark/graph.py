from collections import defaultdict
from pprint import pprint
import matplotlib.pyplot as plt

def create_graph(file):
    seq = defaultdict(lambda: {"time": [], "average": 0})
    parfiles = defaultdict(lambda: defaultdict(lambda: {"time": [], "average": 0, "speedup": 1}))
    parslices = defaultdict(lambda: defaultdict(lambda: {"time": [], "average": 0, "speedup": 1}))
    # Open file
    with open(file, "r") as f:
        # Iterate through each line 
        for line in f:
            line = line.strip()
            line_arr = line.split(",")
            run_type = line_arr[0]
            file_size = line_arr[1]
            # Sequential run
            if run_type == 's':
                time = float(line_arr[2])
                # Append time
                seq[file_size]["time"].append(time)
                # Calculate average time
                if len(seq[file_size]["time"]) == 5:
                    seq[file_size]["average"] = round(sum(seq[file_size]["time"]) / 5, 2)
            # Parallel files
            elif run_type == 'parfiles':
                threads = int(line_arr[2])
                time = float(line_arr[3])
                # Append time 
                parfiles[file_size][threads]["time"].append(time)
                # Calculate average time 
                if len(parfiles[file_size][threads]["time"]) == 5:
                    parfiles[file_size][threads]["average"] = round(sum(parfiles[file_size][threads]["time"]) / 5, 2)
            # Parallel slices
            else:
                threads = int(line_arr[2])
                time = float(line_arr[3])
                # Append time 
                parslices[file_size][threads]["time"].append(time)
                # Calculate average time 
                if len(parslices[file_size][threads]["time"]) == 5:
                    parslices[file_size][threads]["average"] = round(sum(parslices[file_size][threads]["time"]) / 5, 2)
                        
    # Run this to update speedup in a seperate loop to ensure the all averages 
    # are calculated first
    for file_size in seq:
        for threads in parfiles[file_size]:
            parfiles[file_size][threads]["speedup"] = round(seq[file_size]["average"] / parfiles[file_size][threads]["average"], 2)
            parslices[file_size][threads]["speedup"] = round(seq[file_size]["average"] / parslices[file_size][threads]["average"], 2)
    
    # Commend out below to check data
    # pprint(seq)
    # pprint(par)
    
    # Create speed up graph
    plt.figure(figsize=(10, 7))
    # Plot a line on each file size with varying number of threads
    for file_size in parfiles:
        threads = []
        speedup = []
        for thread_count in parfiles[file_size]:
            threads.append(int(thread_count))
            speedup.append(parfiles[file_size][thread_count]["speedup"])
        # Plot the line for this file size
        plt.plot(threads, speedup, marker='o', label=file_size + " (Parfiles)")
    for file_size in parslices:
        threads = []
        speedup = []
        for thread_count in parslices[file_size]:
            threads.append(int(thread_count))
            speedup.append(parslices[file_size][thread_count]["speedup"])
        # Plot the line for this file size
        plt.plot(threads, speedup, marker='x', label=file_size + " (Parslices)")
    plt.title('Speedup Graph')
    plt.xlabel('Threads')
    plt.ylabel('Speedup')
    plt.grid(True)
    plt.legend(title='File Size')
    plt.savefig('../benchmark/speedup.png')

# Call the function to create the graph
create_graph("../benchmark/slurm/out/measurements.txt")