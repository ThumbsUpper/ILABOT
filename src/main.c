// NOTE: Code so far is only a test for the first step of the object detection algorithm 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
	int x;
	int y;
} Vector2;
typedef struct {
	int x;
	int y;
	int z;
} Vector3;

int readBmpPixels(FILE *file, uint32_t pixelOffset, Vector2 imgSize, uint8_t *buffer) {
	size_t rowBytes = (size_t)imgSize.x * 3;
	size_t rowStride = (rowBytes + 3) & ~(size_t)3;
	uint8_t *row = malloc(rowStride);
	if (row == NULL) { return -1; }
	if (fseek(file, (long)pixelOffset, SEEK_SET) != 0) {free(row); return -1; }
	
	for (int pixelRow = 0; pixelRow < imgSize.y; ++pixelRow) {
		if (fread(row, 1, rowStride, file) != rowStride) { free(row); return -1; }
		for (int pixelColumn = 0; pixelColumn < imgSize.x; ++pixelColumn) {
			size_t rowPixelIndex = pixelColumn * 3;
			size_t bufferIndex = ((imgSize.y - 1 - pixelRow) * imgSize.x + pixelColumn) * 3;
			buffer[bufferIndex + 0] = row[rowPixelIndex + 2];
			buffer[bufferIndex + 1] = row[rowPixelIndex + 1];
			buffer[bufferIndex + 2] = row[rowPixelIndex + 0];
		}
	}
	free(row);
	return 0;
}

void checkImages(uint8_t *img, uint8_t *imgcheck, uint8_t *difference, Vector2 imgSize) {
	size_t pixelTotal = (size_t)imgSize.x * (size_t)imgSize.y;
	for (size_t i = 0; i < pixelTotal; ++i) {
		size_t byteIndex = i * 3;
		difference[i] =
			img[byteIndex] != imgcheck[byteIndex] ||
			img[byteIndex + 1] != imgcheck[byteIndex + 1] ||
			img[byteIndex + 2] != imgcheck[byteIndex + 2]
			? 255 : 0;
	}
}

int main(void) {

	// open test files
	FILE *img = fopen("current.bmp", "rb");
	FILE *imgcheck = fopen("prev.bmp", "rb");
	FILE *differienceOutput = fopen("differienceOutput.pgm", "wb");

	if (img == NULL || imgcheck == NULL || differienceOutput == NULL) { printf("File not successfully opened\n"); return 1; }

	// Image Processing Code
	// Load Image into Buffer
	uint8_t imgHeaderBuffer[0x20];
	size_t bytesRead = fread(imgHeaderBuffer, 1, 0x20, img);
	if (bytesRead == 0) { printf("Error reading from file\n"); return 1; }

	if (imgHeaderBuffer[0] != 'B' || imgHeaderBuffer[1] != 'M') {
		printf("current.bmp is not a BMP file\n");
		return 1;
	}
	
	// Define img Specifications
	Vector2 imgSize;
	uint32_t startimg = *(uint32_t*)&imgHeaderBuffer[0x0A];
	imgSize.x = *(uint32_t*)&imgHeaderBuffer[0x12];
	imgSize.y = *(uint32_t*)&imgHeaderBuffer[0x16];

	// Send img to be put into a buffer
	uint8_t *imgBuffer;
	imgBuffer = malloc(sizeof(uint8_t) * imgSize.x * imgSize.y * 3);
	if (imgBuffer == NULL) { printf("Could not allocate space\n"); free(imgBuffer); return 1;}
	if (readBmpPixels(img, startimg, imgSize, imgBuffer)) { printf("Error reading bmp file\n"); return 1; }

	// Load imgcheck into Buffer
	bytesRead = fread(imgHeaderBuffer, 1, 0x20, imgcheck);
	if (bytesRead == 0) { printf("Error reading from file\n"); free(imgBuffer); return 1; }

	if (imgHeaderBuffer[0] != 'B' || imgHeaderBuffer[1] != 'M') {
		printf("prev is not a bitmap file\n");
		free(imgBuffer);
		return 1;
	}
	
	// Define imgcheck Specifications
	Vector2 imgcheckSize;
	uint32_t startimgcheck = *(uint32_t*)&imgHeaderBuffer[0x0A];
	imgcheckSize.x = *(uint32_t*)&imgHeaderBuffer[0x12];
	imgcheckSize.y = *(uint32_t*)&imgHeaderBuffer[0x16];

	// Send imgcheck to be put into a buffer
	uint8_t *imgcheckBuffer;
	imgcheckBuffer = malloc(sizeof(uint8_t) * imgcheckSize.x * imgcheckSize.y * 3);
	if (imgcheckBuffer == NULL) { printf("Could not allocate space\n"); free(imgcheckBuffer); return 1;}
	if (readBmpPixels(imgcheck, startimgcheck, imgcheckSize, imgcheckBuffer)) { printf("Error reading bmp file\n"); return 1; }

	if (imgSize.x != imgcheckSize.x || imgSize.y != imgcheckSize.y) {
		printf("bmp dimensions do not match\n");
		return 1;
	}

	uint8_t *difference = malloc((size_t)imgSize.x * imgSize.y);
	if (difference == NULL) { printf("Could not allocate space\n"); free(difference); return 1; }
	checkImages(imgBuffer, imgcheckBuffer, difference, imgSize);
	fprintf(differienceOutput, "P5\n%d %d\n255\n", imgSize.x, imgSize.y);
	if (fwrite(difference, 1, (size_t)imgSize.x * imgSize.y, differienceOutput) != (size_t)imgSize.x * imgSize.y) {
		printf("error writing pgm pixels");
		return 1;
	}

	// close the files
	free(imgBuffer);
	free(imgcheckBuffer);
	free(difference);
    fclose(imgcheck);
    fclose(img);
	fclose(differienceOutput);
    return 0;
}