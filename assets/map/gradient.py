import cv2
import numpy as np
import skimage.exposure as exposure

img = cv2.imread('height.png',  cv2.IMREAD_UNCHANGED)

gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
cv2.imwrite('gray.png', img)
blur = cv2.GaussianBlur(gray, (7,7), 1.3, 1.3)
dx = cv2.Sobel(blur, cv2.CV_64F, 1, 0, ksize=3)
dy = cv2.Sobel(blur, cv2.CV_64F, 0, 1, ksize=3)

orientation = cv2.phase(dx, dy, False)
orientation_encoded = np.floor(255 * orientation / (2 * np.pi))

print(dx)
dx = cv2.convertScaleAbs(np.abs(dx)) / 255
dy = cv2.convertScaleAbs(np.abs(dy)) / 255

dx_2 = dx*dx
dy_2 = dy*dy
cv2.imwrite('dx.png', dx)
cv2.imwrite('dy.png', dy)
magnitude = (np.sqrt(dx_2 + dy_2) * 255).astype(np.uint8)

cv2.imwrite('magnitude.png', magnitude)


output_img = np.zeros((len(img[0]), len(img), 3), np.uint8)

for i in range(len(img[0])):
    for j in range(len(img)):
        output_img[j][i] = (magnitude[j][i], orientation_encoded[j][i], 0)


cv2.imwrite('gradient.png', output_img)
