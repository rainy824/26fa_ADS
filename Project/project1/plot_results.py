import csv
import matplotlib.pyplot as plt
import numpy as np

# 读取CSV数据
data = {}
with open('result.csv', 'r', encoding='utf-8') as f:
    reader = csv.DictReader(f)
    for row in reader:
        n = int(row['N'])
        scene = row['场景']
        if scene not in data:
            data[scene] = {'N': [], 'BST_ins': [], 'BST_del': [], 'AVL_ins': [], 'AVL_del': [], 'Splay_ins': [], 'Splay_del': []}
        data[scene]['N'].append(n)
        data[scene]['BST_ins'].append(float(row['BST_insert_ms']))
        data[scene]['BST_del'].append(float(row['BST_delete_ms']))
        data[scene]['AVL_ins'].append(float(row['AVL_insert_ms']))
        data[scene]['AVL_del'].append(float(row['AVL_delete_ms']))
        data[scene]['Splay_ins'].append(float(row['Splay_insert_ms']))
        data[scene]['Splay_del'].append(float(row['Splay_delete_ms']))

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'Arial Unicode MS']
plt.rcParams['axes.unicode_minus'] = False

scene_titles = {
    '升序插入升序删除': 'Scenario 1: Insert Increasing, Delete Increasing',
    '升序插入降序删除': 'Scenario 2: Insert Increasing, Delete Decreasing',
    '随机插入随机删除': 'Scenario 3: Insert Random, Delete Random',
}

for scene, d in data.items():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
    fig.suptitle(scene_titles.get(scene, scene), fontsize=14, fontweight='bold')

    N = np.array(d['N'])

    # 左图：插入时间
    ax1.plot(N, d['BST_ins'], 'o-', label='Unbalanced BST', linewidth=2, markersize=5)
    ax1.plot(N, d['AVL_ins'], 's-', label='AVL Tree', linewidth=2, markersize=5)
    ax1.plot(N, d['Splay_ins'], '^-', label='Splay Tree', linewidth=2, markersize=5)
    ax1.set_xlabel('Input Size N', fontsize=12)
    ax1.set_ylabel('Running Time (ms)', fontsize=12)
    ax1.set_title('Insertion Time', fontsize=12)
    ax1.legend(fontsize=10)
    ax1.grid(True, alpha=0.3)
    ax1.set_yscale('log')

    # 右图：删除时间
    ax2.plot(N, d['BST_del'], 'o-', label='Unbalanced BST', linewidth=2, markersize=5)
    ax2.plot(N, d['AVL_del'], 's-', label='AVL Tree', linewidth=2, markersize=5)
    ax2.plot(N, d['Splay_del'], '^-', label='Splay Tree', linewidth=2, markersize=5)
    ax2.set_xlabel('Input Size N', fontsize=12)
    ax2.set_ylabel('Running Time (ms)', fontsize=12)
    ax2.set_title('Deletion Time', fontsize=12)
    ax2.legend(fontsize=10)
    ax2.grid(True, alpha=0.3)
    ax2.set_yscale('log')

    plt.tight_layout()
    filename = f'plot_{scene}.png'
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    print(f'Saved: {filename}')
    plt.close()

print('All plots generated successfully.')
