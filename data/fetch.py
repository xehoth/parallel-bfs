import subprocess
import os

urls = [
    "https://snap.stanford.edu/data/web-Stanford.txt.gz",
    "https://snap.stanford.edu/data/roadNet-CA.txt.gz",
]

names = list(map(lambda x: x.split("/")[-1][:-7], urls))

print(names)

def download(name, url):
    if os.path.exists(f'{name}.txt'):
        print(f'{name} already download, skip...')
        return
    subprocess.run(['wget', url]).check_returncode()
    subprocess.run(['gzip', '-dfk', name + '.txt.gz']).check_returncode()

    
for name, url in zip(names, urls):
    download(name, url)
    
