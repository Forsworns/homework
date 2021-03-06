import numpy as np
import math
import matplotlib.pyplot as plt
import matplotlib
import os
import json
import random


def processData(file_name, show_figure=False, interpolation=False):
    # 处理读入
    f = open(file_name)
    four_tuple = [0, 0, 0, 0]
    tuple_in_line = []
    cnt = 0
    while True:
        line = f.readline()
        line = line.replace(" ", "")
        if not line:
            break
        elif ":" in line:
            [minute, second, micro] = [float(item) for item in line.split(':')]
            four_tuple[0] = minute*60 + second + micro*0.001
            cnt += 1
        elif "=" in line:
            left = line.split('=')[0]
            right = line.split('=')[1]
            if left[1] == 'x':
                cnt += 1
                four_tuple[1] = float(right)
            elif left[1] == 'y':
                cnt += 1
                four_tuple[2] = float(right)
            elif left[1] == 'z':
                cnt += 1
                four_tuple[3] = float(right)
        if cnt % 4 == 0:
            tuple_in_line.append(four_tuple.copy())
    f.close()
    tuple_in_line.sort(key=lambda x: x[0], reverse=False)

    data = file_name.split("\\")[-1]
    dataset = file_name.split("\\")[-2]

    time = []
    length = []
    for x in tuple_in_line:
        time.append(x[0])
        length.append(math.sqrt(x[1]**2 + x[2]**2 + x[3]**2))

    # 分划片段
    standardA = 9.7  # 静态加速度
    deviationA = 0.4
    standardG = 0  # 静态角加速度
    deviationG = 0.2
    tl_slice = []
    if('A' in data):
        tl_slice = split_time_length(
            length, standardA, deviationA, time, use_filter=True, threshold=3)
        # [print(str(tl[0])+' '+str(tl[1])) for tl in tl_slice]
    else:
        tl_slice = split_time_length(
            length, standardG, deviationG, time, use_filter=True, threshold=3)
        # [print(str(tl[0])+' '+str(tl[1])) for tl in tl_slice]

    # 可以与图像比较
    if(show_figure):
        plt.plot(time, length)
        plt.title(dataset + "\\" + data)
        plt.ylabel("length")
        plt.xlabel("time")
        plt.show()

    # 正式划分
    tl_index = 0
    tl_output = []
    if(tl_slice != []):
        tl_output.append([])
        for item in tuple_in_line:
            if(item[0] < tl_slice[tl_index][0]):
                continue
            elif(item[0] < tl_slice[tl_index][1]):
                tl_output[tl_index].append(item.copy())
            else:
                tl_index += 1
                if(tl_index >= len(tl_slice)):
                    break
                tl_output.append([])

        # 对G做插值
        if(interpolation and 'G' in data):
            for item in tl_output:
                times = len(item)
                for i in range(0, times-1):
                    item.append(
                        [(item[i][0]+item[i+1][0])/2,
                         (item[i][1]+item[i+1][1])/2,
                         (item[i][2]+item[i+1][2])/2,
                         (item[i][3] + item[i+1][3])/2
                         ])
                item.sort(key=lambda x: x[0])

        json_output = json.dumps(tl_output)
        os.chdir(dataset)
        if not os.path.exists('json'):
            os.mkdir('json')
        with open("json\{}.json".format(data), 'w') as f:
            f.write(json_output)
        os.chdir('..')

    # 与前面的图像结合起来看
    if(show_figure):
        for item in tl_output:
            time = []
            length = []
            for x in item:
                time.append(x[0])
                length.append(math.sqrt(x[1]**2 + x[2]**2 + x[3]**2))
            plt.plot(time, length)
            plt.title(x[0])
            plt.ylabel("length")
            plt.xlabel("time")
            plt.show()

    transform2np(dataset, data, tl_output)


def is_interval(standard, deviation, message, idx, window=1):
    # 向window内的点看是否接近标准值
    sum = 0
    for i in range(idx, idx + window):
        if i < len(message):
            if(abs(message[i]-standard) < deviation):
                sum += -1
            else:
                sum += 1
    return sum < 0


def split_time_length(length, standard, deviation, time=[], **filter_para):
    # tl_slice = [(start,end)]
    # set parameter
    if(not filter_para):
        filter_para = {'use_filter': False, 'threshold': 0}
    else:
        if('use_filter' not in filter_para):
            filter_para['use_filter'] = False
        elif('threshold' not in filter_para):
            filter_para['threshold'] = 0

    use_time = False
    if(time != []):
        use_time = True
    else:
        time.extend(length)

    interval_flag = True
    tl_slice = []
    start = 0
    end = 0
    for idx, (t, l) in enumerate(zip(time, length)):
        # print('{} {}'.format(idx, l))  # A差不多300十组,G差不多
        if(is_interval(standard, deviation, length, idx, window=2)):
            if(interval_flag):
                # 遇到了interval，但是本身在interval内，不断更新起始和终结
                start = idx
                end = idx
                continue
            else:
                # 遇到了interval，但是刚刚不是在interval内，更新终结
                end = idx
                interval_flag = True
                if(use_time):
                    if(not filter_para['use_filter'] or
                       (filter_para['use_filter'] and time[end]-time[start] > filter_para['threshold'])):
                        tl_slice.append((time[start], time[end]))
                else:
                    if(not filter_para['use_filter'] or
                       (filter_para['use_filter'] and end-start > filter_para['threshold'])):
                        tl_slice.append((start, end))
                start = idx
        else:
            # 没有遇到interval
            interval_flag = False
    return tl_slice


def transform2np(dir_name, file_name, data):
    os.chdir(dir_name)
    savednpx = np.asarray(data)
    if not os.path.exists("x"):
        os.mkdir("x")
    np.save('{}\{}.npy'.format("x", file_name), savednpx)

    savednpy = np.ones([len(data), 1], dtype=np.uint8) * \
        int(dir_name.split("_")[0], 10)
    if not os.path.exists("y"):
        os.mkdir("y")
    np.save('{}\{}.npy'.format("y", file_name), savednpy)
    os.chdir("..")


if __name__ == "__main__":
    np.set_printoptions(threshold=np.inf)
    output_dir = "output"
    dataset_dir = os.getcwd()+"\dataset"
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)
    os.chdir(output_dir)
    for item in os.listdir(dataset_dir)[0:-1]:
        # the -1 is for time-person.md
        if not os.path.exists(item):
            os.mkdir(item)
        dataset_path = os.path.join(dataset_dir, item)
        for file in os.listdir(dataset_path):
            file_name = os.path.join(dataset_path, file)
            processData(file_name, interpolation=False)

    # 整理,长度统一 os.getcwd() == /output
    output_staticsA = []
    output_staticsG = []
    lengthA = 100
    lengthG = 100
    for sub_dir in os.listdir(os.getcwd()):
        if 'json' in sub_dir:
            continue
        y = sub_dir[0]
        for file in os.listdir(os.path.join(sub_dir, "json")):
            type_id = file.split('.')[0][-1]
            with open(sub_dir+"\\json\\"+file, 'r') as f:
                file_content = f.read()
                json_content = json.loads(file_content)
                for item in json_content:
                    if(type_id == 'G'):
                        lengthG = min(lengthG, len(item))
                        output_staticsG.append([item, y])
                    else:
                        lengthA = min(lengthA, len(item))
                        output_staticsA.append([item, y])
    print("加速度的序列长度{}".format(lengthA))
    print("角加速度的序列长度{}".format(lengthG))
    for item in output_staticsA:
        item[0] = item[0][:lengthA]
    for item in output_staticsG:
        item[0] = item[0][:lengthG]

    json_output_staticsA = json.dumps(output_staticsA)
    json_output_staticsG = json.dumps(output_staticsG)
    with open('A.json', 'w') as f:
        f.write(json_output_staticsA)
    with open('G.json', 'w') as f:
        f.write(json_output_staticsG)
