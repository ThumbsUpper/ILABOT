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
void CreateGriddedImageAvg(long *griddedImgAvg, uint8_t *img, Vector2 imgSize, Vector2 gridSize) {
	Vector2 gridSquareSize = {imgSize.x / gridSize.x, imgSize.y / gridSize.y};
	Vector2 pixelTracker = {0, 0};
	Vector2 gridTracker = {0, 0};
	for (int pixelRow = 0; pixelRow < imgSize.x; ++pixelRow) {
		for (int pixelColumn = 0; pixelColumn < imgSize.y; ++pixelColumn) {
			// Do check stuff ig (I'll add later)
			(pixelTracker.x < gridSquareSize.x) ? ++pixelTracker.x : pixelTracker.x = 0, ++gridTracker.x; 
		}
	}
}

int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

int main(int argc, char **argv) {

	// file opeining code
	FILE *img = fopen("current.bmp", "r");
	FILE *imgcheck = fopen("prev.bmp", "r");
	if (imgcheck == NULL || img == NULL) {
        printf("File not successfully opened\n");
        return 1;
    }

	// Image Processing Code
	// Load Images into Buffers
	uint8_t imgHeaderBuffer[0x20];
	size_t bytesRead = fread(imgHeaderBuffer, 1, 0x20, img);
	if (bytesRead == NULL) {
		printf("Error reading from file\n");
	}
	
	// Define Image Specifications
	Vector2 imgSize;
	uint32_t startimg = *(uint32_t*)&imgHeaderBuffer[0x0A];
	imgSize.x = *(uint32_t*)&imgHeaderBuffer[0x12];
	imgSize.y = *(uint32_t*)&imgHeaderBuffer[0x16];
	uint8_t *imgBuffer = malloc(imgSize.x *imgSize.y * 3);

	//Send img to be split up
	fseek(img, startimg, 0);
	Vector2 gridSize;
	gridSize.x = gcd(imgSize.x, imgSize.y) * 4;
	gridSize.y = 4;
	long *griddedImg = malloc(gridSize.x * gridSize.y * 3 * sizeof(int));
	bytesRead = fread(imgBuffer, 1, imgSize.x * imgSize.y * 3, img);
	
	CreateGriddedImageAvg(griddedImg, imgBuffer, imgSize, gridSize);	

	// Load imgCheck into a buffer
	uint8_t imgCheckHeaderBuffer[0x20];
	bytesRead = fread(imgCheckHeaderBuffer, 1, 0x20, imgcheck);
	if (bytesRead == NULL) {
		printf("Error reading from file\n");
	} 

	Vector2 imgCheckSize;
	uint32_t startimgCheck = *(uint32_t*)&imgCheckHeaderBuffer[0x0A];
	imgCheckSize.x = *(uint32_t*)&imgCheckHeaderBuffer[0x12];
	imgCheckSize.y = *(uint32_t*)&imgCheckHeaderBuffer[0x16];
	uint8_t *imgCheckBuffer = malloc(imgSize.x *imgSize.y * 3);

	// close the files
	free(griddedImg);
    fclose(imgcheck);
    fclose(img);
    return 0;
}
