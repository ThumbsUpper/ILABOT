#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct {
	int x;
	int y;
} Vector2;
typedef struct {
	int x;
	int y;
	int z;
} Vector3;

// Incomplete grid function
void CreateGriddedImageAvg(Vector3 *griddedImgAvg, uint8_t *img, Vector2 imgSize, Vector2 gridSize) {
	printf("Yeah, it's not good. (Live in Las Vegas 20 %i %i)\n", imgSize.x, imgSize.y);

	// Fix this later vvv
	Vector2 gridSquareSize = {round((double)imgSize.x / (double)gridSize.x), round((double)imgSize.y / (double)gridSize.y)};
	Vector2 pixelTracker = {0, 0};
	Vector2 gridTracker = {0, 0};
	printf("%i, %i", gridSize.x, gridSize.y);
	Vector3 gridSquareTotal[gridSize.x][gridSize.y];
	int rowSize = imgSize.x;
	for (int pixelRow = 0; pixelRow < imgSize.y; ++pixelRow) {
		for (int pixelColumn = 0; pixelColumn < imgSize.x;) {
			gridSquareTotal[gridTracker.x][gridTracker.y].x = img[pixelRow * rowSize + pixelColumn];
			gridSquareTotal[gridTracker.x][gridTracker.y].y = img[pixelRow * rowSize + pixelColumn + 1];
			gridSquareTotal[gridTracker.x][gridTracker.y].z = img[pixelRow * rowSize + pixelColumn + 2];
			if (pixelTracker.x < gridSquareSize.x) {++pixelTracker.x;} 
			else {pixelTracker.x = 0, ++gridTracker.x;}
			pixelColumn += 3;
		}
		gridTracker.x = 0;
		if (pixelTracker.y < gridSquareSize.y) {++pixelTracker.y;} 
		else {pixelTracker.y = 0, ++gridTracker.y;} 
	}
	long gridSquareSizeTotal = gridSquareSize.x * gridSquareSize.y;
	for (int gridRow = 0; gridRow < gridSize.x; ++gridRow) {
		for (int gridColumn = 0; gridColumn < imgSize.y; ++gridColumn) {
			griddedImgAvg[gridRow * gridSize.x + gridColumn].x = gridSquareTotal[gridRow][gridColumn].x / gridSquareSizeTotal;
			griddedImgAvg[gridRow * gridSize.x + gridColumn].y = gridSquareTotal[gridRow][gridColumn].y / gridSquareSizeTotal;
			griddedImgAvg[gridRow * gridSize.x + gridColumn].z = gridSquareTotal[gridRow][gridColumn].z / gridSquareSizeTotal;
		}
	}
}

Vector2 gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	Vector2 result = {a, b};
	return result;
}

int main(void) {

	// file opeining code
	FILE *img = fopen("current.bmp", "r");
	FILE *imgcheck = fopen("prev.bmp", "r");
	if (img == NULL || imgcheck == NULL) { printf("File not successfully opened\n"); return 1; }

	// Image Processing Code
	// Load Images into Buffers
	uint8_t imgHeaderBuffer[0x20];
	size_t bytesRead = fread(imgHeaderBuffer, 1, 0x20, img);
	if (bytesRead == 0) { printf("Error reading from file\n"); return 1; }
	
	// Define Image Specifications
	Vector2 imgSize;
	uint32_t startimg = *(uint32_t*)&imgHeaderBuffer[0x0A];
	imgSize.x = *(uint32_t*)&imgHeaderBuffer[0x12];
	imgSize.y = *(uint32_t*)&imgHeaderBuffer[0x16];
	uint8_t *imgBuffer = malloc(imgSize.x *imgSize.y * 3);
	if (imgBuffer == 0) { printf("Error reading from file\n"); return 1; }

	//Send img to be split up
	fseek(img, startimg, 0);
	Vector2 gridSize;
	gridSize = gcd(imgSize.x, imgSize.y);
	Vector3 *griddedImgAvg = malloc(gridSize.x * gridSize.y * 3 * sizeof(int));
	bytesRead = fread(imgBuffer, 1, imgSize.x * imgSize.y * 3, img);
	if (bytesRead == 0) { printf("Error reading from file\n"); return 1; }

	CreateGriddedImageAvg(griddedImgAvg, imgBuffer, imgSize, gridSize);	

	// close the files
	free(griddedImgAvg);
    fclose(imgcheck);
    fclose(img);
    return 0;
}
