import numpy as np
import math
import matplotlib.pyplot as plt
import matplotlib
import os


def processData(file_name, show_figure=False):
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
			subfour_tuple = four_tuple
			tuple_in_line.append(subfour_tuple.copy())

	data = file_name.split("\\")[-1]
	dataset = file_name.split("\\")[-2]

	if(show_figure):
		length = [0, 0]
		time_length = []
		a = []
		b = []
		for x in tuple_in_line:
			a.append(x[0])
			b.append(math.sqrt(x[1]**2 + x[2]**2 + x[3]**2))
			time_length.append(length.copy())
		plt.plot(a, b)
		plt.title(dataset+ "\\" +data)
		plt.ylabel("length")
		plt.xlabel("time")
		plt.show()

	os.chdir(dataset)
	savednpx = np.asarray(tuple_in_line)
	if not os.path.exists("x"):
		os.mkdir("x")
	np.save('{}\{}.npy'.format("x", data), savednpx)

	savednpy = np.ones([10,1],dtype=np.uint8) * int(dataset.split("_")[0],10)
	if not os.path.exists("y"):
    		os.mkdir("y")
	np.save('{}\{}.npy'.format("y", data), savednpy)
	os.chdir("..")
	f.close()


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
			processData(file_name,True)