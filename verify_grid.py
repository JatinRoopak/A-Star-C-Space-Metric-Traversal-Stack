import numpy as np
import cv2 
import matplotlib.pyplot as plt

grid = np.loadtxt("binary_grid.txt", dtype=np.uint8)

visual_grid = grid*255
visula_grid = cv2.resize(visual_grid, (500, 500), interpolation=cv2.INTER_NEAREST)

plt.imshow(visula_grid, cmap='gray')
plt.title("Verified Binary Grid")
plt.axis('off')
plt.show()
