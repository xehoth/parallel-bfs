import glob
import re
import matplotlib.pyplot as plt

class BenchData:
    def __init__(self, core, mteps, stddev, time):
        self.core = core
        self.mteps = mteps
        self.stddev = stddev
        self.time = time
    
    def __repr__(self) -> str:
        return f"[{self.core}, {self.mteps}, {self.stddev}, {self.time}]"

def get_core_in_file_name(str):
    return int(re.findall(r'\.\d+\.', str)[0][1:-1])

def load(prefix, method):
    files = glob.glob(f"benchmark/{prefix}.*.{method}.bench")
    files.sort(key=lambda x: get_core_in_file_name(x))
    ret = []
    for i, v in enumerate(files):
        with open(v) as f:
            mteps = float(f.readline())
            stddev = float(f.readline())
            time = float(f.readline())
            ret.append(BenchData(i + 1, mteps, stddev, time))
    return ret

def calc_speedup(p, s):
    return p.mteps / s.mteps

def get_max_performace(l):
    return max(l, key=lambda x: x.mteps)

def get_max_speedup(l, base):
    return max(l, key=lambda x: calc_speedup(x, base))

def draw_speedup(data, name, format, color, show=True, **args):
    l = data[name]
    a = l[0]
    b = l[1]
    base = min(a[0], b[0], key=lambda x: x.mteps)
    x = [i for i in range(1, 64)]
    y = [max(calc_speedup(a[i], base), calc_speedup(b[i], base)) for i in range(len(x))]
    m = max(get_max_speedup(a, base), get_max_speedup(b, base), key=lambda x: calc_speedup(x, base))
    plt.plot(x, y, format, label=f'{name}', color=color, **args)
    if show:
        offset = 1
        if name == "roadNet-CA":
            offset = -3
        plt.text(m.core + offset, calc_speedup(m, base), f'{calc_speedup(m, base):.2f}', ha='center', va='bottom', fontsize=10.5)

def draw_mteps(data, name, format, color, show=True, **argvs):
    l = data[name]
    a = l[0]
    b = l[1]
    x = [i for i in range(1, 65)]
    y = [max(u, v, key=lambda x: x.mteps).mteps for u, v in zip(a, b)]
    m = max([max(u, v, key=lambda x: x.mteps) for u, v in zip(a, b)], key=lambda x: x.mteps)
    plt.plot(x, y, format, label=f'{name}', color=f'{color}', **argvs)
    plt.text(m.core, m.mteps, f'{m.mteps:.2f}', ha='center', va='bottom', fontsize=10.5)
    plt.title(f'{name} performance')

def gen_table(l, name):
    print(r"""\begin{table}[h]
\renewcommand\arraystretch{0.8}
\centering
\begin{tabular}{@{}c|ccc@{}}
\toprule
Threads          & MTEPS  & StdDev     & Time (s)      \\ \midrule""")
    for i in l:
        print(f'{i.core} & {i.mteps:.3f} & {i.stddev:.3f} & {i.time:.6f} \\\\')
    print(r"""\bottomrule
\end{tabular}
\caption{""" + name + "}\n" + r"\end{table}")


data = {
    "web-Stanford": [load("web-Stanford", "topdown"), load("web-Stanford", "hybrid")],
    "roadNet-CA": [load("roadNet-CA", "topdown"), load("roadNet-CA", "hybrid")],
    "com-Orkut": [load("com-orkut.ungraph", "topdown"), load("com-orkut.ungraph", "hybrid")],
    "soc-LiveJournal1": [load("soc-LiveJournal1", "topdown"), load("soc-LiveJournal1", "hybrid")],
    "RMAT1": [load("RMAT1", "topdown"), load("RMAT1", "hybrid")],
    "RMAT2": [load("RMAT2", "topdown"), load("RMAT2", "hybrid")],
    "RMAT3": [load("RMAT3", "topdown"), load("RMAT3", "hybrid")],
}

# print(data)
# gen_table(data["web-Stanford"][0], "web-Stanford top-down")
# gen_table(data["web-Stanford"][1], "web-Stanford hybrid")
# gen_table(data["roadNet-CA"][0], "roadNet-CA top-down")
# gen_table(data["roadNet-CA"][1], "roadNet-CA hybrid")
# gen_table(data["com-Orkut"][0], "com-Orkut top-down")
# gen_table(data["com-Orkut"][1], "com-Orkut hybrid")
# gen_table(data["soc-LiveJournal1"][0], "soc-LiveJournal1 top-down")
# gen_table(data["soc-LiveJournal1"][1], "soc-LiveJournal1 hybrid")
# gen_table(data["RMAT1"][0], "RMAT1 top-down")
# gen_table(data["RMAT1"][1], "RMAT1 hybrid")
# gen_table(data["RMAT2"][0], "RMAT2 top-down")
# gen_table(data["RMAT2"][1], "RMAT2 hybrid")
# gen_table(data["RMAT3"][0], "RMAT3 top-down")
# gen_table(data["RMAT3"][1], "RMAT3 hybrid")

# l = data["RMAT3"]
# s = 138.81
# p = get_max_performace(l[1])
# print(p, p.mteps / s)


plt.clf()
plt.plot(list(range(1, 64)), list(range(1, 64)), "r--", label='ideal linear')

draw_speedup(data, "web-Stanford", "-o", "dimgray", False, markersize=3.0)
draw_speedup(data, "roadNet-CA", "-o", "thistle", markersize=3.0)
draw_speedup(data, "com-Orkut", "-o" ,"red", markersize=3.0)
draw_speedup(data, "soc-LiveJournal1", "-o", "green", markersize=3.0)
draw_speedup(data, "RMAT1", "-o", "blue", markersize=3.0)
draw_speedup(data, "RMAT2", "-o", "purple", markersize=3.0)
draw_speedup(data, "RMAT3", "-o", "orange", markersize=3.0)

plt.xlabel("threads")
plt.ylabel("speedup")
plt.title(f"speedup graph")
plt.legend()
# plt.show()

mteps_draws = ["web-Stanford", "roadNet-CA", "com-Orkut", "soc-LiveJournal1", "RMAT1", "RMAT2", "RMAT3"]

for i in mteps_draws:
    plt.clf()
    draw_mteps(data, i, "-o", "skyblue", markersize=4.0)
    plt.xlabel("threads")
    plt.ylabel("MTEPS")
    plt.legend()
    plt.savefig(f"report/figures/{i}.png")


