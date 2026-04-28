import numpy as np

# 生成所有可能的 positive uint16 位模式（从 0 到 32767）
all_bits = np.arange(0, 2**15, dtype=np.uint16)  # 0,1,2,...,32767

# 将位模式解释为 float16
all_f16 = all_bits.view(np.float16)

# 输出文件名
output_file = "float16_positive_values_numpy.txt"

with open(output_file, "w") as f:
    for idx, value in enumerate(all_f16, start=0):  # idx 从 1 开始
        # 写入序号和数值，用空格分开；数值使用 repr 以便区分 NaN 的多种形式
        f.write(f"{idx} {str(value)}\n")

print(f"已写入 {len(all_f16)} 行数据到文件 {output_file}")