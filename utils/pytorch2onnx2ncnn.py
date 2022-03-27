import torch
import torch.onnx
import torchvision

def pytorch2onnx(network:str, inputShape:int, outputName:str):
    """
    This method convert networks from PyTorch (.pth) to Onnx (.onnx) format 

    Args:
        network (str): Predefined network names for initialize such as "r18", "r50", "r101", etc.
        inputShape (int): Dummy input's input size as an integer
        outputName (str): Onnx network saved file name

    Raises:
        ValueError: Other networks can be implemented but resnet50 and resnet18 implemented for example
    """
    if network == "r18":
        network = torchvision.models.resnet18(pretrained=True)
    elif network == "r50":
        network = torchvision.models.resnet50(pretrained=True)
    else:
        raise ValueError("Network not defined!")
    
    dummyInput = torch.randn(1, 3, inputShape, inputShape)
    
    torch.onnx._export(network, dummyInput, "%s.onnx" % outputName, export_params=True)
    
if __name__ == "__main__":
    pytorch2onnx("r50", 224, "R50")