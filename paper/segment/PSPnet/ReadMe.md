# PSPNet - Pyramid Scene Parsing Network

官方Paper:https://arxiv.org/abs/1612.01105

该文主要为语义分割内容，该文采用金字塔多尺度特征融合

官方code:https://github.com/hszhao/PSPNet

train code:https://github.com/hszhao/semseg

翻版 train code: https://github.com/mehtanihar/pspnet

![Screenshot from 2021-11-17 11-00-03](./img/Screenshot%20from%202021-11-17%2011-00-03.png)

论文整体：

采用多尺度的方案，使用ppm模块配合特征利用拓展和提取出的feature模块进行拼接后，直接进行网络分类。

整体流程读取类的代码位于该datasets中，将Img,label以list的形式存储，存储过程中进入网络forward传播的时候，直接进入网络的model(input,output)直接进行计算loss。

数据读取类:位于util/dataset内中

```
IMG_EXTENSIONS = ['.jpg', '.jpeg', '.png', '.ppm', '.bmp', '.pgm']

def is_image_file(filename):# 判定是否读取数据为正确图片
    filename_lower = filename.lower()
    return any(filename_lower.endswith(extension) for extension in IMG_EXTENSIONS)

def make_dataset(split='train', data_root=None, data_list=None): #生成数据list类
    assert split in ['train', 'val', 'test']
    if not os.path.isfile(data_list):
        raise (RuntimeError("Image list file do not exist: " + data_list + "\n"))
    image_label_list = []
    list_read = open(data_list).readlines()
    print("Totally {} samples in {} set.".format(len(list_read), split))
    print("Starting Checking image&label pair {} list...".format(split))
    for line in list_read:
        line = line.strip()
        line_split = line.split(' ')
        if split == 'test':
            if len(line_split) != 1:
                raise (RuntimeError("Image list file read line error : " + line + "\n"))
            image_name = os.path.join(data_root, line_split[0])
            label_name = image_name  # just set place holder for label_name, not for use
        else:
            if len(line_split) != 2:
                raise (RuntimeError("Image list file read line error : " + line + "\n"))
            image_name = os.path.join(data_root, line_split[0])
            label_name = os.path.join(data_root, line_split[1])
        '''
        following check costs some time
        if is_image_file(image_name) and is_image_file(label_name) and os.path.isfile(image_name) and os.path.isfile(label_name):
            item = (image_name, label_name)
            image_label_list.append(item)
        else:
            raise (RuntimeError("Image list file line error : " + line + "\n"))
        '''
        item = (image_name, label_name)
        image_label_list.append(item)
    print("Checking image&label pair {} list done!".format(split))
    return image_label_list


class SemData(Dataset): #数据读取用于 Dataloader 载入
    def __init__(self, split='train', data_root=None, data_list=None, transform=None):
        self.split = split
        self.data_list = make_dataset(split, data_root, data_list)
        self.transform = transform

    def __len__(self):
        return len(self.data_list)

    def __getitem__(self, index):
        image_path, label_path = self.data_list[index]
        image = cv2.imread(image_path, cv2.IMREAD_COLOR)  # BGR 3 channel ndarray wiht shape H * W * 3
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)  # convert cv2 read image from BGR order to RGB order
        image = np.float32(image) 生成numpy.ndarray(float32)
        label = cv2.imread(label_path, cv2.IMREAD_GRAYSCALE)  # GRAY 1 channel ndarray with shape H * W # one channel numpy.ndarray(int)
        if image.shape[0] != label.shape[0] or image.shape[1] != label.shape[1]:
            raise (RuntimeError("Image & label shape mismatch: " + image_path + " " + label_path + "\n"))#做了异常排除
        if self.transform is not None:
            image, label = self.transform(image, label)
        return image, label
```

主要关心训练网络:semseg 仓库中的model结构和论文中结构进行解析

![Screenshot from 2021-11-17 10-57-36](./img/Screenshot%20from%202021-11-17%2010-57-36.png)

```
#在model.py中参考
# 整个网络PSPnet 对应网络中的feature提取模块，拼接的方式采用adaptivepool的方式进行放缩	

class PPM(nn.Module):
    def __init__(self, in_dim, reduction_dim, bins):#offical bins 1,2,3,6 indims: 2048 reduction_dim: indims/len(bins) 512
        super(PPM, self).__init__()
        self.features = []
        for bin in bins:
            self.features.append(nn.Sequential(
                nn.AdaptiveAvgPool2d(bin),#bin: 1 2 3 6 针对ppm 官方paper上的1x1 2x2 3x3 6x6 的特征
                nn.Conv2d(in_dim, reduction_dim, kernel_size=1, bias=False), # 2048 上述的特征 512 1 0  
                nn.BatchNorm2d(reduction_dim), # 512
                nn.ReLU(inplace=True)#
            ))
        self.features = nn.ModuleList(self.features) # ModuleLists is convert the features to Net module

    def forward(self, x):
        x_size = x.size()
        out = [x]
        for f in self.features:
            out.append(F.interpolate(f(x), x_size[2:], mode='bilinear', align_corners=True))
        return torch.cat(out, 1)

class PSPNet(nn.Module):
    def __init__(self, layers=50, bins=(1, 2, 3, 6), dropout=0.1, classes=2, zoom_factor=8, use_ppm=True, criterion=nn.CrossEntropyLoss(ignore_index=255), pretrained=True):
        super(PSPNet, self).__init__()
        assert layers in [50, 101, 152]
        assert 2048 % len(bins) == 0  #
        assert classes > 1
        assert zoom_factor in [1, 2, 4, 8]
        self.zoom_factor = zoom_factor
        self.use_ppm = use_ppm
        self.criterion = criterion

        if layers == 50:
            resnet = models.resnet50(pretrained=pretrained)
        elif layers == 101:
            resnet = models.resnet101(pretrained=pretrained)
        else:
            resnet = models.resnet152(pretrained=pretrained)
        self.layer0 = nn.Sequential(resnet.conv1, resnet.bn1, resnet.relu, resnet.conv2, resnet.bn2, resnet.relu, resnet.conv3, resnet.bn3, resnet.relu, resnet.maxpool) # select the conv1 bn1 relu
        self.layer1, self.layer2, self.layer3, self.layer4 = resnet.layer1, resnet.layer2, resnet.layer3, resnet.layer4

        for n, m in self.layer3.named_modules():
            if 'conv2' in n:
                m.dilation, m.padding, m.stride = (2, 2), (2, 2), (1, 1)
            elif 'downsample.0' in n:
                m.stride = (1, 1)
        for n, m in self.layer4.named_modules():
            if 'conv2' in n:
                m.dilation, m.padding, m.stride = (4, 4), (4, 4), (1, 1)
            elif 'downsample.0' in n:
                m.stride = (1, 1)

        fea_dim = 2048
        if use_ppm: #Pyramid pooling module
            self.ppm = PPM(fea_dim, int(fea_dim/len(bins)), bins)
            fea_dim *= 2
        self.cls = nn.Sequential(
            nn.Conv2d(fea_dim, 512, kernel_size=3, padding=1, bias=False),
            nn.BatchNorm2d(512),
            nn.ReLU(inplace=True),
            nn.Dropout2d(p=dropout),
            nn.Conv2d(512, classes, kernel_size=1)
        )
        if self.training:
            self.aux = nn.Sequential(
                nn.Conv2d(1024, 256, kernel_size=3, padding=1, bias=False),
                nn.BatchNorm2d(256),
                nn.ReLU(inplace=True),
                nn.Dropout2d(p=dropout),
                nn.Conv2d(256, classes, kernel_size=1)
            )

    def forward(self, x, y=None):
        x_size = x.size()
        assert (x_size[2]-1) % 8 == 0 and (x_size[3]-1) % 8 == 0 
        h = int((x_size[2] - 1) / 8 * self.zoom_factor + 1)
        w = int((x_size[3] - 1) / 8 * self.zoom_factor + 1)

        x = self.layer0(x)
        x = self.layer1(x)
        x = self.layer2(x)
        x_tmp = self.layer3(x)
        x = self.layer4(x_tmp)# CNN feature map has include the resnet serial weights
        if self.use_ppm:
            x = self.ppm(x) # #Pyramid pooling module this is the 
        x = self.cls(x) #prepare the extend of the map
        if self.zoom_factor != 1:
            x = F.interpolate(x, size=(h, w), mode='bilinear', align_corners=True)# resize 双线性插值返回hw,这里直接返回target segment img

        if self.training: # 如果训练模式采用如下形式
            aux = self.aux(x_tmp) # aux module extend the kernal to classes
            if self.zoom_factor != 1:
                aux = F.interpolate(aux, size=(h, w), mode='bilinear', align_corners=True) # resize 成align_corners
            main_loss = self.criterion(x, y)  # y is target label use to caculate the loss 
            aux_loss = self.criterion(aux, y)
            return x.max(1)[1], main_loss, aux_loss #caculate the losses
        else:
            return x
```

Loss函数的设置，对于分割模型，论文中没具体阐述Loss的构成，从Train入口中进入查看target和output之间的Loss，表示上述网络结构中。

![Screenshot from 2021-11-17 10-59-38](./img/Screenshot%20from%202021-11-17%2010-59-38.png)

test

```
def test(model, image_path, classes, mean, std, base_size, crop_h, crop_w, scales, colors):
    image = cv2.imread(image_path, cv2.IMREAD_COLOR)  # BGR 3 channel ndarray wiht shape H * W * 3
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)  # convert cv2 read image from BGR order to RGB order
    h, w, _ = image.shape
    prediction = np.zeros((h, w, classes), dtype=float)
    for scale in scales:
        long_size = round(scale * base_size)
        new_h = long_size
        new_w = long_size
        if h > w:
            new_w = round(long_size/float(h)*w)
        else:
            new_h = round(long_size/float(w)*h)
        image_scale = cv2.resize(image, (new_w, new_h), interpolation=cv2.INTER_LINEAR)
        prediction += scale_process(model, image_scale, classes, crop_h, crop_w, h, w, mean, std)
    prediction = scale_process(model, image_scale, classes, crop_h, crop_w, h, w, mean, std)
    prediction = np.argmax(prediction, axis=2)
    gray = np.uint8(prediction)
    color = colorize(gray, colors)
    image_name = image_path.split('/')[-1].split('.')[0]
    gray_path = os.path.join('./figure/demo/', image_name + '_gray.png')
    color_path = os.path.join('./figure/demo/', image_name + '_color.png')
    cv2.imwrite(gray_path, gray)
    color.save(color_path)
    logger.info("=> Prediction saved in {}".format(color_path))
```



