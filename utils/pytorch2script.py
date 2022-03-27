import torch
import torchvision

def pytorch2script(network:str, inputShape:int, outputName:str):
    if network == "r18":
        network = torchvision.models.resnet18(pretrained=True)
    elif network == "r50":
        network = torchvision.models.resnet50(pretrained=True)
    else:
        raise ValueError("Network not defined!")
    
    dummyInput = torch.randn(1, 3, inputShape, inputShape)
    
    traced_script_module = torch.jit.trace(network, dummyInput)
    traced_script_module.save("%s.pt" % outputName)
    
if __name__ == "__main__":
    pytorch2script("r50", 224, "R50")