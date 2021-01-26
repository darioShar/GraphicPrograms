#include <cstdio>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "Plane.h"
#include <SFML\Graphics.hpp>


#define NUM_THREAD 1024

cudaError_t computeMandelbrot(int, int, ComplexMandelbrot*, int*);

__global__ void mandelbrotKernel(int size, int iterations, ComplexMandelbrot* c_initial, ComplexMandelbrot* z_current, int* screen)
{
    int i = threadIdx.x + blockIdx.x*blockDim.x;
	if (i < size) {
		ComplexMandelbrot z = z_current[i];
		DATA_TYPE x, y;
		int k;
		for (k = 0; k <iterations; k++) {
			x = (z.real * z.real - z.imaginary * z.imaginary) + c_initial[i].real;
			y = (z.imaginary * z.real + z.real * z.imaginary) + c_initial[i].imaginary;

			if ((x * x + y * y) > 4.0) break;
			z.real = x;
			z.imaginary = y;
		}
		screen[i] += k;
	}
}

__global__ void resetScreenKernel(int size, int* screen) {
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	if (i < size) {
		screen[i] = 0;
	}
}

__global__ void shaderScreenKernel(int size, int iterations, int* screen) {
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	if (i < size) {
		if (screen[i] == iterations) screen[i] = 0;
		float t, r, g, b;
		if (screen[i] != 0) {
			//screen[i] = (Color)( 0xFF0000FF + (complexNumbers[i].counter << 13));
			t = (float)screen[i] / (float)iterations;
			r = 9 * (1 - t)*t*t*t;
			g = 15 * (1 - t)*(1 - t)*t*t;
			b = 8.5*(1 - t)*(1 - t)*(1 - t)*t;
			screen[i] = (static_cast<int>(r * 255)) + (static_cast<int>(g * 255) << 8) + (static_cast<int>(b * 255) << 16) + (static_cast<int>(255) << 24);
		}
	}
}


constexpr DATA_TYPE ZOOM_FACTOR = 1.3;

int main()
{
	DATA_TYPE MOVE_FACTOR = 0.2;

	ofstream parametersFile;
	parametersFile.open("Parameters.txt");
	if (!parametersFile.is_open()) {
		cout << "Couldn't open Paramters.txt\n";
	}

	Vue view(ASPECT_RATIO);
	/*
	view.xmin = -0.759130752537225395;
	view.xmax = -0.759130739373624519;
	view.ymin = -0.070381827302726005;
	view.ymax = -0.070381819903563639;

	MOVE_FACTOR = 0.000000000739984117;
	*/

	Plane plane(view);

	Event evnt;
	while (plane.isOpen()) {
		while (plane.pollEvent(evnt)) {
			if (evnt.type == Event::Closed) {
				plane.close();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Z)) {
			view.move(0.0f, MOVE_FACTOR);
			plane.setWindow(view);
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			view.move(0.0f, -MOVE_FACTOR);
			plane.setWindow(view);
		}
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			view.move(ASPECT_RATIO * (-MOVE_FACTOR), 0.0f);
			plane.setWindow(view);
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			view.move(ASPECT_RATIO * MOVE_FACTOR, 0.0f);
			plane.setWindow(view);
		}
		if (Keyboard::isKeyPressed(Keyboard::E)) {
			view.zoom(ZOOM_FACTOR);
			plane.setWindow(view);
			MOVE_FACTOR /= ZOOM_FACTOR;
		}
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			view.zoom(1.0f / ZOOM_FACTOR);
			plane.setWindow(view);
			MOVE_FACTOR *= ZOOM_FACTOR;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			int ITERATIONS = plane.getNumIterations();
			ITERATIONS < 100 ? ITERATIONS += 1 : ITERATIONS *= 1.3;
			cout << "ITERATIONS : " << ITERATIONS << "\n";
			plane.setNumIterations(ITERATIONS);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			int ITERATIONS = plane.getNumIterations();
			ITERATIONS < 100 ? ITERATIONS -= 1 : ITERATIONS /= 1.3;
			cout << "ITERATIONS : " << ITERATIONS << "\n";
			plane.setNumIterations(ITERATIONS);
		}
		if (Keyboard::isKeyPressed(Keyboard::R)) {
			plane.setNumIterations(1500);
			cout << "ITERATIONS : " << 1500 << "\n";
		}
		if (Keyboard::isKeyPressed(Keyboard::M)) {
			plane.setNumIterations(100000);
			cout << "ITERATIONS : " << 100000 << "\n";
		}
		if (Keyboard::isKeyPressed(Keyboard::O)) {
			plane.setNumIterations(1000000);
			cout << "ITERATIONS : " << 1000000 << "\n";
		}
		if (Mouse::isButtonPressed(Mouse::Button::Left)) {
			printf("Parameters :\n\
				Xmin = %.18lf,\n\
				Xmax = %.18lf,\n\
				Ymin = %.18lf,\n\
				Ymax = %.18lf\n\n\
				Mouse Cursor Position : %.18lf , %.18lf\n\n\
				Move factor : %.18lf\n\n\n",
				view.xmin,
				view.xmax,
				view.ymin,
				view.ymax,
				(DATA_TYPE)Mouse::getPosition().x * (view.xmax - view.xmin) / (DATA_TYPE)WIDTH,
				(DATA_TYPE)Mouse::getPosition().y * (view.ymax - view.ymin) / (DATA_TYPE)HEIGHT,
				MOVE_FACTOR);

			parametersFile << fixed << setprecision(numeric_limits<DATA_TYPE>::digits10 + 1) << "Parameters :\nview.xmin = " << view.xmin <<
				"\nview.xmax = " << view.xmax << "\nview.ymin = " << view.ymin << "\nview.ymax = " << view.ymax << "\n\nMouse Cursor : " <<
				(DATA_TYPE)Mouse::getPosition().x * (view.xmax - view.xmin) / (DATA_TYPE)WIDTH << " , " <<
				(DATA_TYPE)Mouse::getPosition().y * (view.ymax - view.ymin) / (DATA_TYPE)HEIGHT << "\n\n" <<
				"MOVE_FACTOR = " << MOVE_FACTOR << "\n\n\n";
		}

		plane.clear();
		computeMandelbrot(NUM_PIXEL, plane.getNumIterations(), plane.getComplexNumbers(), plane.getScreenPointer());
		plane.drawPlane();
		plane.display();
	}
	return 0;
}






// Helper function for using CUDA to add vectors in parallel.
cudaError_t computeMandelbrot(int size, int iterations, ComplexMandelbrot* c, int* screen)
{
    ComplexMandelbrot *dev_c;
	ComplexMandelbrot *dev_z;
	int* dev_screen;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(ComplexMandelbrot));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

	cudaStatus = cudaMalloc((void**)&dev_z, size * sizeof(ComplexMandelbrot));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_screen, size * sizeof(int));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_c, c, size * sizeof(ComplexMandelbrot), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

	cudaStatus = cudaMemcpy(dev_z, dev_c, size * sizeof(ComplexMandelbrot), cudaMemcpyDeviceToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}

	// Reset dev_screen
	resetScreenKernel <<<(size + NUM_THREAD - 1) / NUM_THREAD, NUM_THREAD >>>(size, dev_screen);

	unsigned long long maxIterationsPerKernel = 10000/10;
	unsigned long long kernelCalls = iterations / maxIterationsPerKernel;

	for (int i = 0; i < kernelCalls; i++) {
		mandelbrotKernel <<<(size + NUM_THREAD - 1) / NUM_THREAD, NUM_THREAD >>>(size, maxIterationsPerKernel, dev_c, dev_z, dev_screen);
		// Check for any errors launching the kernel
		cudaStatus = cudaGetLastError();
		if (cudaStatus != cudaSuccess) {
			fprintf(stderr, "mandelbrotKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
			goto Error;
		}
		// cudaDeviceSynchronize waits for the kernel to finish, and returns
		// any errors encountered during the launch.
		cudaStatus = cudaDeviceSynchronize();
		if (cudaStatus != cudaSuccess) {
			fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching mandelbrotKernel!\n", cudaStatus);
			goto Error;
		}
	}

	mandelbrotKernel <<<(size + NUM_THREAD - 1) / NUM_THREAD, NUM_THREAD >>>(size, iterations % maxIterationsPerKernel, dev_c, dev_z, dev_screen);
	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "mandelbrotKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}
	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching mandelbrotKernel!\n", cudaStatus);
		goto Error;
	}


	// Apply shader to dev_screen
	shaderScreenKernel << <(size + NUM_THREAD - 1) / NUM_THREAD, NUM_THREAD >> >(size, iterations, dev_screen);

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(screen, dev_screen, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
	cudaFree(dev_z);
	cudaFree(dev_screen);
    
    return cudaStatus;
}
