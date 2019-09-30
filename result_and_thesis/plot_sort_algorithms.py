from matplotlib import pyplot as plt
import matplotlib
import os
import sys
import pandas as pd
import numpy as np

if __name__ == '__main__':
    file_name = [x for x in os.listdir('.') if x.endswith('.tsv')]
    tables = [pd.read_csv(x, sep='\t') for x in file_name]

    cv = ['r','y','g','b','r','y','g','b','r']

    plt.figure(figsize=(8, 8))
    for i in range(len(tables)):
        n = np.array(tables[i]['N'])
        t = np.array(tables[i]['Time(ms)'])
        plt.plot(n, t, c=cv[i], label=file_name[i].split('.')[0])
    
    plt.xlabel('Scale(N)')
    plt.ylabel('Time(ms)')
    plt.title('Sorting performace comaprision between qsort and linked mergesort')
    plt.legend()
    plt.savefig('sorting_comparison.png')