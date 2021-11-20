import data.fetch
from multiprocessing import cpu_count
import subprocess
import os
import shutil

def bench(name, file, format):
    for i in range(cpu_count()):
        output = "benchmark/" + name + f".{i + 1}.topdown.bench"
        cmd = ['./main', file, format, output, '-1', 'topdown', f'{i + 1}']
        subprocess.run(cmd).check_returncode()
    for i in range(cpu_count()):
        output = "benchmark/" + name + f".{i + 1}.hybrid.bench"
        cmd = ['./main', file, format, output, '-1', 'topdown', f'{i + 1}']
        subprocess.run(cmd).check_returncode()

if __name__ == '__main__':
    names = data.fetch.names + ["RMAT1", "RMAT2", "RMAT3"]
    files = list(map(lambda x: "data/" + x + ".txt", names))
    formats = ["SNAP", "SNAP", "SNAP", "SNAP", "RMAT", "RMAT", "RMAT"]

    if os.path.exists("benchmark"):
        shutil.rmtree("benchmark")
    
    os.mkdir("benchmark")

    for name, file, f in zip(names, files, formats):
        bench(name, file, f)