import cv2
import numpy as np
import skimage.exposure as exposure

img = cv2.imread('height.png',  cv2.IMREAD_UNCHANGED)


gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
blur = cv2.GaussianBlur(gray, (0,0), 1.3, 1.3)
dx = cv2.Sobel(blur, cv2.CV_64F, 1, 0, ksize=3)
dy = cv2.Sobel(blur, cv2.CV_64F, 0, 1, ksize=3)

dx_2 = cv2.multiply(dx_norm, dx_norm)
dy_2 = cv2.multiply(dy_norm, dy_norm)

sobel_magnitude = cv2.sqrt(dx2 + dy_2)

sobel_magnitude = exposure.rescale_intensity(sobel_magnitude, in_range='image', out_range=(0,255)).clip(0, 255).astype(np.uint8)
cv2.imwrite('magnitude', sobel_magnitude)

orientation = cv2.phase(dx, dy, False)
orientation_encoded = np.floor(255 * orientation / (2 * np.pi))

magnitude_encoded = np.floor(cv2.magnitude(dx, dy))

output_img = np.zeros((len(img[0]), len(img), 3), np.uint8)

for i in range(len(img[0])):
    for j in range(len(img)):
        output_img[j][i] = (magnitude_encoded[j][i], orientation_encoded[j][i], 0)


print(output_img)
cv2.imwrite('gradient.png', output_img)
