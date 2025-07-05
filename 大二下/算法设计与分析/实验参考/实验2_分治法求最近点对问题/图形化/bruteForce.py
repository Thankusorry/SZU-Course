import random
import math
import matplotlib.pyplot as plt
import imageio
import numpy


# 生成随机坐标点
def rand_points(n):
    return [(random.uniform(-1, 1), random.uniform(-1, 1)) for i in range(n)]
# 蛮力法求解最近点对
def brute_force(points):
    fig, ax = plt.subplots()
    images = []
    for i in range(len(points)):
        for j in range(i + 1, len(points)):
            plt.clf()
            plt.scatter([p[0] for p in points], [p[1] for p in points])
            plt.plot([points[j][0],points[i][0]],[points[j][1],points[i][1]], 'red')
            plt.title("bruteForceFromYeMaolin")
            fig.canvas.draw()
            img = numpy.frombuffer(fig.canvas.tostring_rgb(), dtype='uint8')
            img = img.reshape(fig.canvas.get_width_height()[::-1] + (3,))
            images.append(img)
    plt.close()
    imageio.mimsave('brute_force.gif', images, fps=5)

# 测试
points = rand_points(50)
brute_force(points)