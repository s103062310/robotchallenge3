import os
import numpy
import scipy
from scipy import ndimage

im = scipy.misc.imread('../cifar10_image/adjust/adjust_1.jpg')
im = im.astype('int32')
dx = ndimage.sobel(im, 1) # horizontal derivative
dy = ndimage.sobel(im, 0) # vertical derivative
mag = numpy.hypot(dx, dy) # magnitude
mag *= 255.0 / numpy.max(mag) # normalize (Q&D)
ga = ndimage.filters.uniform_filter1d(mag, 1000, 0)
ga *= 255.0 / numpy.max(ga)
scipy.misc.imsave('../cifar10_image/sobel/sobel_1.jpg', mag);

os.system("./challenge3 ../cifar10_image/adjust/adjust_1.jpg")
