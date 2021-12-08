import struct
import torch 
from torch import nn 
from torch.nn import functional as F

class Lenet5 (nn.Module):
    
    '''
    test for inference in the TensorRT and create the wts file
    '''
    def __init__(self) :
        super(Lenet5,self).__init__()
        self.conv1 = nn.Conv2d(1,6,kernel_size=5,padding=0,bias=True)
        self.pool1 = nn.AvgPool2d(2,2,0)
        self.pool2 = nn.MaxPool2d(2)
        self.linear1 = nn.Linear(294,10)
    def forward(self,x):
        x = self.conv1(x)
        x = self.pool1(x)
        x = self.pool2(x)
        x = x.view(x.size(0),-1)
        x = self.linear1(x)
        return x
def inference():
    net = torch.load("lenet5.pth")
    net = net.to("cuda:0")
    net.eval()
    print(net)
    temp = torch.ones(1,1,32,32).to("cuda:0")
    out = net(temp)
    print(out)
    # ready to write for the format 
    f = open('lenet5.wts','w')
    f.write("{}\n".format(len(net.state_dict().keys())))
    for k,v in net.state_dict().items():

        vr = v.reshape(-1).cpu().numpy()
        f.write("{} {}".format(k,len(vr)))
        for vv in vr:
            f.write(" ")
            f.write(struct.pack(">f",float(vv)).hex())
        f.write("\n")
    f.close()
    return 
def srcinfer():
    print('cuda device count: ', torch.cuda.device_count())
    net = torch.load('lenet5.pth')
    net = net.to('cuda:0')
    net.eval()
    print('model: ', net)
    #print('state dict: ', net.state_dict()['conv1.weight'])
    tmp = torch.ones(1, 1, 32, 32).to('cuda:0')
    #print('input: ', tmp)
    out = net(tmp)
    print('lenet out:', out)

    f = open("lenet5add.wts", 'w')
    f.write("{}\n".format(len(net.state_dict().keys())))
    for k,v in net.state_dict().items():
        #print('key: ', k)
        #print('value: ', v.shape)
        vr = v.reshape(-1).cpu().numpy()
        f.write("{} {}".format(k, len(vr)))
        for vv in vr:
            f.write(" ")
            f.write(struct.pack(">f", float(vv)).hex())
        f.write("\n")

def main():
    print("cuda device whether or not found in the device ",torch.cuda.device_count())
    torch.manual_seed(1234)
    net = Lenet5()
    # net.state_dict()
    # a dictionary containing a whole state of the module   
    net = net.to('cuda:0')
    net.eval()
    tmp = torch.ones(1,1,32,32).to('cuda:0')
    out = net(tmp)
    print('lenet out shape ',out.shape)
    print('lenet out',out)
    torch.save(net,"lenet5.pth")
    # m = torch.jit.script(net)
    # torch.jit.save(m,"scriptLenet.pt")
if __name__=="__main__":
    # main()
    inference()
    # srcinfer()