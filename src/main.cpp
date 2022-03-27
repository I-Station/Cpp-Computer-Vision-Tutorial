#include "net.h"

#include <chrono>
#include <stdio.h>
#include <iostream>
#include <torch/torch.h>
#include <torch/script.h>
#include <opencv2/opencv.hpp>

auto ToTensor(cv::Mat img, bool show_output = false, bool unsqueeze=false, int unsqueeze_dim = 0)
{
    at::Tensor tensor_image = torch::from_blob(img.data, {img.rows, img.cols, 3}, at::kByte);

    if (unsqueeze)
    {
        tensor_image.unsqueeze_(unsqueeze_dim);
        std::cout << "tensors new shape: " << tensor_image.sizes() << std::endl;
    }
    
    if (show_output)
    {
        std::cout << tensor_image.slice(2, 0, 1) << std::endl;
    }

    return tensor_image;
}

int pytorch2onnx2ncnn_Example() {
  // Init Variables
  ncnn::Net net;
  ncnn::Mat in, feat;


  // Read Image
  cv::Mat image = cv::imread("../testImage.jpg", 1);

  // Load Network
  net.load_param("Models/R50-Optimized.param");
  net.load_model("Models/R50-Optimized.bin");

  ncnn::Extractor r50 = net.create_extractor();
  r50.set_light_mode(true);
  r50.set_num_threads(4);

  

  // Preprocess
  in = ncnn::Mat::from_pixels_resize(image.data, ncnn::Mat::PIXEL_RGB, image.cols, image.rows, 224, 224);
  float mean[1] = { 128.f };
  float norm[1] = { 1/128.f };
  in.substract_mean_normalize(mean, norm);

  r50.input("input.1", in);
  r50.extract("495", feat);

  std::cout << "Pytorch2Onnx2NCNN Worked" << std::endl;

  return 0;
}

int pytorch2script() {
  // Init Variables
  cv::Mat image2;
  at::Tensor output;
  torch::Tensor tensor;
  std::vector<torch::jit::IValue> inputs;

  // Read Image
  image2 = cv::imread("../testImage.jpg", 1);
  
  // Load Network
  torch::jit::script::Module network;

  network = torch::jit::load("Models/R50.pt");
  network.eval();

  // Resize
  resize(image2, image2, cv::Size(224, 224), cv::INTER_LINEAR);

  // Array2Tensor
  tensor = ToTensor(image2);

  tensor = tensor.toType(c10::kFloat);

  tensor = tensor.unsqueeze_(0);

  tensor = tensor.permute({0, 3, 1, 2});

  // Inference
  inputs.push_back(tensor);
  output = network.forward(inputs).toTensor();

  std::cout << "Pytorch2Script Worked" << std::endl;

  return 0;
}


int main() {
  pytorch2onnx2ncnn_Example();
  pytorch2script();
}