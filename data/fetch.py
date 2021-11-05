import subprocess
import os

urls = [
    "https://snap.stanford.edu/data/web-Stanford.txt.gz",
    "https://snap.stanford.edu/data/roadNet-CA.txt.gz",
    # "https://snap.stanford.edu/data/soc-LiveJournal1.txt.gz",
    "https://snap.stanford.edu/data/bigdata/communities/com-orkut.ungraph.txt.gz",
]

names = list(map(lambda x: x.split("/")[-1][:-7], urls))

print(names)

def download(name, url):
    if os.path.exists(f'{name}.txt'):
        print(f'{name} already download, skip...')
        return
    subprocess.run(['wget', url]).check_returncode()
    subprocess.run(['gzip', '-dfk', name + '.txt.gz']).check_returncode()


def gen_rmat(file, n, m, a, b, c, nt = 16):
    if not os.path.exists("PaRMAT"):
        subprocess.run(["git", "clone", "https://github.com/farkhor/PaRMAT"]).check_returncode()
        subprocess.run(["sh", "build_PaRMAT.sh"])
    else:
        print("PaRMAT exists, skip...")
    if os.path.exists(file):
        print(f'{file} already generate, skip...')
        return
    subprocess.run(["PaRMAT", "-nVertices", str(n), "-nEdges", str(m), "-a", str(a), "-b", str(b), "-c", str(c), "-threads", str(nt), "-output", file]).check_returncode()


for name, url in zip(names, urls):
    download(name, url)

gen_rmat("RMAT1.txt", 100000000, 1000000000, 0.3, 0.25, 0.25)