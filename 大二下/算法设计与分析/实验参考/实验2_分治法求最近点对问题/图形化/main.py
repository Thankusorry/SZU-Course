import random
import matplotlib.pyplot as plt
import imageio
import numpy


# 生成随机坐标点
def rand_points(n):
    return [(random.uniform(-1, 1), random.uniform(-1, 1)) for i in range(n)]


# 蛮力法求解最近点对
def divide(points):
    points=sorted(points,key=lambda point:point[0])
    number = len(points)
    fig, ax = plt.subplots()
    images = []

    for i in range(1, 4):
        plt.clf()
        plt.title("divideConque")
        plt.scatter([p[0] for p in points], [p[1] for p in points])
        for step in range(number // (2 ** i), number, number // (2 ** i)):
            midLine = points[step][0]
            plt.axvline(x=midLine, color="red", linestyle="--")
            fig.canvas.draw()
        img = numpy.frombuffer(fig.canvas.tostring_rgb(), dtype='uint8')
        img = img.reshape(fig.canvas.get_width_height()[::-1] + (3,))
        images.append(img)
    plt.close()
    imageio.mimsave('divideConque.gif', images, fps=1)


# 测试
points = rand_points(100)
divide(points)
