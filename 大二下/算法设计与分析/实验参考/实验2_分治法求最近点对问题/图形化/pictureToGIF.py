from PIL import Image
import os


def make_gif(image_folder, gif_file_name, duration=500):
    images = []
    for filename in os.listdir(image_folder):
        if filename.endswith('.jpg') or filename.endswith('.png') or filename.endswith('.jpeg'):
            img_path = os.path.join(image_folder, filename)
            img = Image.open(img_path)
            images.append(img)
    images[0].save(gif_file_name, append_images=images[1:], save_all=True, duration=duration, loop=0)


if __name__ == '__main__':
    make_gif('./images', 'animated.gif', duration=300)