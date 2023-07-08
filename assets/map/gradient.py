import cv2
import numpy as np

img = cv2.imread('height.png')

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
dx = cv2.Sobel(gray, cv2.CV_64F, 1, 0)
dy = cv2.Sobel(gray, cv2.CV_64F, 0, 1)

orientation = cv2.phase(dx, dy, False)
orientation_encoded = np.floor(255 * orientation / (2 * np.pi))

magnitude_encoded = np.floor(cv2.magnitude(dx, dy))

print(magnitude_encoded, orientation_encoded)
