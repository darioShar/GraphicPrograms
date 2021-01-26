// ImageProcessing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ImageProcessor.h"
#include <iostream>
#include <cxxopts.hpp>
#include <filesystem>

using namespace sf;
using namespace std;
using std::cout;
namespace fs = std::experimental::filesystem;

// Program main parametres
bool ShowAllProcesses(false);

bool Blur(false);
unsigned int BlurKernelSize(0);
float BlurSigma = 1.4f;

bool Edge(false);
unsigned int EdgeBlurKernelSize(0);
float EdgeBlurSigma = 1.4f;
float EdgeThreshold(100.0f);


vector<string> SourcesPaths;
string DestinationFolderPath("");


// Image Processing object
ImageProcessor imPro;


int main(int argc, char** argv)
{
	/** Manage command line arguments */
	if (argc < 2) {
		printf("\nNo arguments. Exiting program...\n");
		return 0;
	}

	cxxopts::Options options(argv[0], "Apply a filter to image(s)");
	options.positional_help("[optional args] SOURCE(s)");
	options.add_options()
		("h,help", "Show help message")
		("s,show", "Show all intermdiary processes", cxxopts::value<bool>(ShowAllProcesses))

		("b,blur", "Apply a gaussian blur filter", cxxopts::value<bool>(Blur))
		("bksize", "Blur kernel size", cxxopts::value<unsigned int>(BlurKernelSize)->default_value("5"), "UINT")
		("bksigma", "Blur kernel sigma (gaussian blur)", cxxopts::value<float>(BlurSigma)->default_value("1.4"), "FLOAT")

		("e,edge", "Apply edge filter (Sobel)", cxxopts::value<bool>(Edge))
		("edbksize", "Edge filter blur kernel size", cxxopts::value<unsigned int>(EdgeBlurKernelSize)->default_value("5"), "UINT")
		("edbksigma", "Edge filter blur kernel sigma (gaussian blur)", cxxopts::value<float>(EdgeBlurSigma)->default_value("1.4"), "FLOAT")
		("edth", "Threshold value above which pixel is considered edge", cxxopts::value<float>(EdgeThreshold)->default_value("100.0f"), "FLOAT(0<=x<255)")

		("d,destination", "Save image to destination folder", cxxopts::value<string>(DestinationFolderPath)->default_value("Test/"), "FOLDER")
		("sources", "Path(s) to source image(s)", cxxopts::value<std::vector<std::string>>(), "FILE(S)");

	options.parse_positional("sources");
	options.parse(argc, argv);

	if (options.count("help") || options.count("h")) {
		cout << options.help() << "\n";
		return 1;
	}

	if (options.count("sources")) {
		for (auto& source : options["sources"].as<vector<string>>()) {
			if ((source.find('*') != string::npos) || (source.find('?') != string::npos)) {
				// Then we look in the directory for all possibly corresponding files
				for (auto& p : fs::directory_iterator(SourceDirectory(source))) {
					if (DoStringsMatch(source.c_str(), p.path().generic_string().c_str())) {
						SourcesPaths.push_back(p.path().generic_string());
					}
				}
			}
			else
				SourcesPaths.push_back(source);
		}
	}
	else {
		cerr << "No source image to process. Exiting program...\n";
		return 1;
	}

	cout << SourcesPaths.size() << " images to be processed : \n";
	for (auto& source : SourcesPaths)
		cout << "\t" << source << "\n";

	cout << endl;

	/** Process images */
	int count = 1;
	// If blurring and detecting edge and blur kernels similar
	if (Blur && Edge && (std::abs(BlurSigma - EdgeBlurSigma) < 0.05f) && (BlurKernelSize == EdgeBlurKernelSize)) { // That means user has asked to change.
		imPro.setBlurMatrix(IPMatrix::createGaussianKernel(BlurKernelSize, BlurSigma));
		for (auto& sourcePath : SourcesPaths) {
			sourcePath;
			string processedImagePath = DestinationFolderPath + RemovePathVerbose(sourcePath);
			if (!imPro.loadImageToProcess(sourcePath))
				continue;
			cout << "[" << count++ << "/" << SourcesPaths.size() << "]\tBlurring and edge detecting : " << sourcePath << "\n";
			imPro.applyBrightnessFilter();
			imPro.applyBlurFilter(ShowAllProcesses);
			imPro.saveProcessedImage(processedImagePath + "_blurred.png");
			imPro.applyEdgeFilter(ShowAllProcesses);
			imPro.saveProcessedImage(processedImagePath + "_edge.png");
		}
	}
	else {
		if (Blur) {
			cout << "\n\nBlurring " << SourcesPaths.size() << " images\n\n";
			imPro.setBlurMatrix(IPMatrix::createGaussianKernel(BlurKernelSize, BlurSigma));
			for (auto& sourcePath : SourcesPaths) {
				sourcePath;
				string processedImagePath = DestinationFolderPath + RemovePathVerbose(sourcePath);
				if (!imPro.loadImageToProcess(sourcePath))
					continue;
				cout << "[" << count++ << "/" << SourcesPaths.size() << "]\tBlurring : " << sourcePath << "\n";
				imPro.applyBrightnessFilter();
				imPro.applyBlurFilter(ShowAllProcesses);
				imPro.saveProcessedImage(processedImagePath + "_blurred.png");
			}
		}

		if (Edge) {
			cout << "\n\n Performing edge detection on " << SourcesPaths.size() << " images\n\n";
			imPro.setBlurMatrix(IPMatrix::createGaussianKernel(EdgeBlurKernelSize, EdgeBlurSigma));
			if (EdgeThreshold >= 0.0f && EdgeThreshold < 255.0f)
				imPro.setEdgeDetetctionThreshold(EdgeThreshold);
			for (auto& sourcePath : SourcesPaths) {
				sourcePath;
				string processedImagePath = DestinationFolderPath + RemovePathVerbose(sourcePath);
				if (!imPro.loadImageToProcess(sourcePath))
					continue;
				cout << "[" << count++ << "/" << SourcesPaths.size() << "]\tEdge detecting : " << sourcePath << "\n";
				imPro.applyBrightnessFilter();
				imPro.applyBlurFilter(ShowAllProcesses);
				imPro.applyEdgeFilter(ShowAllProcesses);
				imPro.saveProcessedImage(processedImagePath + "_edge.png");
			}
		}
	}

	printf("Press enter to exit...\n");
	getchar();
    return 0;
}
