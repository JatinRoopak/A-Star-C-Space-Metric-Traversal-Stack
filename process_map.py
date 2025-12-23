import cv2
import numpy as np


warehouse_image = cv2.imread("warehouse.png", 0)
if warehouse_image is None:
    raise FileNotFoundError("The image 'warehouse.png' could not be found.")

_, binary_grid = cv2.threshold(warehouse_image, 200, 1, cv2.THRESH_BINARY_INV)

kernel = np.ones((3, 3), np.uint8)

robust_grid = cv2.dilate(binary_grid, kernel, iterations=1)

final_grid = cv2.resize(robust_grid, (100, 100), interpolation=cv2.INTER_NEAREST)

np.savetxt("binary_grid.txt", final_grid, fmt='%d')
print("Binary grid saved to 'binary_grid.txt'")