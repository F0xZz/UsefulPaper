#include <chrono>
#include <iostream>
#include <NvInfer.h>
#include <cuda_runtime.h>
#include <NvInferRuntimeCommon.h>
#include <map>
#include <assert.h>
#include <fstream>
#include <ios>
#include <glogger.h>
#include <NvOnnxParser.h>
#ifdef DEBUG
#define ASSERT(f) assert(f)
#else
#define ASSERT(f) ((void)0)
#endif
#define CHECK(status)\
    do\
{\
    auto ret = (status);\
    if(ret!=0)\
{\
    std::cerr<<"Cuda failure"<<ret<<std::endl;\
    abort();\
    }\
    }while(0)
static Logger glogger;
using namespace std;
using namespace nvinfer1;
static const int INPUT_H = 32;
static const int INPUT_W = 32;
static const int OUTPUT_SIZE = 10;
//static const int OUTPUT_SIZE = 1176;
const char* INPUT_BLOB_NAME = "data";
const char* OUTPUT_BLOB_NAME = "prob";
using namespace nvonnxparser;
IParser* parser;
std::map<std::string ,Weights> loadWeights(const std::string file){
    std::cout<<"Loading weights"<<file<<std::endl;
    std::map<std::string ,Weights> weightMap;
    std::ifstream input(file);
    assert(input.is_open()&&"could not open the file");
    int32_t count;
    input >> count;
    std::cout<<"test :"<<count<<std::endl;
    assert(count>0&&"Invaid weight map file");
    while(count--){
        Weights wt{DataType::kFLOAT,nullptr,0};
        uint32_t size;
        std::string name ;
        // read the name and set the type of blob
        input >> name >>std::dec>>size;
        wt.type = DataType::kFLOAT;
        //load the blob
        uint32_t *val = reinterpret_cast<uint32_t*>(malloc(sizeof(val)*size));
        for(uint32_t x = 0,y=size;x<y;++x){
            input>>std::hex>>val[x];
        }
        wt.values = val;
        wt.count = size;
        weightMap[name] = wt;
    }
    return weightMap;
}
ICudaEngine *createTestEngine(unsigned int maxbatchsize,IBuilder* builder,IBuilderConfig* config,DataType dt){
    INetworkDefinition* network = builder->createNetworkV2(0U);//is equal to createNetwork();
    //    dim3
    ITensor *data = network->addInput(INPUT_BLOB_NAME,dt,Dims3{1,INPUT_H,INPUT_W});

    ASSERT(data);
    std::cout<<"dd2"<<std::endl;
    std::map<std::string ,Weights> weightmap = loadWeights("./lenet5addLinear.wts");
    //weightmap
    for(auto &mem:weightmap){
        std::cout<<mem.first<<std::endl;}
    IConvolutionLayer* conv1 = network->addConvolutionNd(*data,6,DimsHW{5,5},weightmap["conv1.weight"],weightmap["conv"
                                                                                                                 ".bias"]);
    ASSERT(conv1);
    std::cout<<"dd"<<std::endl;
    conv1->setStrideNd(DimsHW{1,1});
    //    IPoolingLayer* pool1 = network->addActivation(*conv1->getOutput(0),PoolingType::kAVERAGE,DimsHW{2,2});
    IPoolingLayer* pool1 = network->addPooling(*conv1->getOutput(0),PoolingType::kAVERAGE,DimsHW{2,2});

    pool1->setStride(DimsHW{2,2});
    IPoolingLayer* pool2 = network->addPooling(*pool1->getOutput(0),PoolingType::kMAX,DimsHW{2,2});
    pool2->setStride(DimsHW{2,2});
    assert(pool2);
    IFullyConnectedLayer* fc1 = network->addFullyConnected(*pool2->getOutput(0),OUTPUT_SIZE,weightmap["linear1.weight"],weightmap["linear1.bias"]);
    assert(fc1);

    fc1->getOutput(0)->setName(OUTPUT_BLOB_NAME);
    //mark a tensor network as a output
    network->markOutput(*fc1->getOutput(0));
    //build engine
    builder->setMaxBatchSize(1);
    config->setMaxWorkspaceSize(1<<20);

    ICudaEngine* engine = builder->buildEngineWithConfig(*network,*config);
    network->destroy();
    for(auto &mem:weightmap){
        //free all the weightmap
        free((void*)(mem.second.values));
    }
    return engine;
}
void APItoModel(unsigned int maxbatchsize,IHostMemory** modelstream){

    //    ILogger logger;//ILogger is a virtual class

    IBuilder* builder = createInferBuilder(glogger);
    IBuilderConfig* config = builder->createBuilderConfig();
    ICudaEngine* engine = createTestEngine(1,builder,config,DataType::kFLOAT);

    ASSERT(engine! = nullptr);

    (*modelstream) = engine->serialize();

    engine->destroy();

    builder->destroy();
}
//use engine to detect or classifer
void doInference(IExecutionContext& context,float* input,float* output,int batchSize){

    const ICudaEngine& engine = context.getEngine();
    // pointer to input and output, device pass to engine
    // Engine requires exactly IEngine::getNbBindings() number of buffers
    // input and output
    int conbuffer = engine.getNbBindings();
    std::cout<<"this network engine num: "<<conbuffer<<std::endl;
    void* buffers[2];

    // in order to bind the buffers , we need to know the name of the input and output tensor
    // Note that dices are guaranteed to be less than
    const int inputIndex = engine.getBindingIndex(INPUT_BLOB_NAME);
    const int outputIndex = engine.getBindingIndex(OUTPUT_BLOB_NAME);
//    std::cout<<"inputIndex get from the blob name"<<inputIndex<<std::endl;
//    std::cout<<"outputIndex get from the blob name"<<outputIndex<<std::endl;


    CHECK(cudaMalloc(&buffers[inputIndex],batchSize*INPUT_H*INPUT_W*sizeof(float)));
    CHECK(cudaMalloc(&buffers[outputIndex],batchSize*OUTPUT_SIZE*sizeof(float)));

    cudaStream_t stream;
    CHECK(cudaStreamCreate(&stream));
    //    CHECK(cudaMemcpyAsync(buffers[inputIndex],
    //                          input,batchSize*INPUT_H*INPUT_W*sizeof(float),
    //                          cudaMemcpyHostToDevice,stream));
    //    auto ret = cudaMemcpyAsync(buffers[inputIndex],
    //                               input,batchSize*INPUT_H*INPUT_W*sizeof(float),
    //                               cudaMemcpyHostToDevice,stream);
    CHECK(cudaMemcpyAsync(buffers[inputIndex],
                          input, batchSize * INPUT_H * INPUT_W * sizeof(float),
                          cudaMemcpyHostToDevice, stream));

    std::cout<<"end the infer"<<std::endl;
    //context IExecutionContext
    context.enqueue(batchSize,buffers,stream,nullptr);
    // this return the output
    CHECK(cudaMemcpyAsync(output,buffers[outputIndex],batchSize*OUTPUT_SIZE*sizeof(float),
                          cudaMemcpyDeviceToHost,stream));

    cudaStreamSynchronize(stream);

    cudaStreamDestroy(stream);
    CHECK(cudaFree(buffers[inputIndex]));
    CHECK(cudaFree(buffers[outputIndex]));
}
void doInferencesrc(IExecutionContext& context, float* input, float* output, int batchSize)
{
    const ICudaEngine& engine = context.getEngine();

    // Pointers to input and output device buffers to pass to engine.
    // Engine requires exactly IEngine::getNbBindings() number of buffers.
    assert(engine.getNbBindings() == 2);
    void* buffers[2];

    // In order to bind the buffers, we need to know the names of the input and output tensors.
    // Note that indices are guaranteed to be less than IEngine::getNbBindings()
    const int inputIndex = engine.getBindingIndex(INPUT_BLOB_NAME);
    const int outputIndex = engine.getBindingIndex(OUTPUT_BLOB_NAME);

    // Create GPU buffers on device
    CHECK(cudaMalloc(&buffers[inputIndex], batchSize * INPUT_H * INPUT_W * sizeof(float)));
    CHECK(cudaMalloc(&buffers[outputIndex], batchSize * OUTPUT_SIZE * sizeof(float)));

    // Create stream
    cudaStream_t stream;
    CHECK(cudaStreamCreate(&stream));

    // DMA input batch data to device, infer on the batch asynchronously, and DMA output back to host
    CHECK(cudaMemcpyAsync(buffers[inputIndex], input, batchSize * INPUT_H * INPUT_W * sizeof(float), cudaMemcpyHostToDevice, stream));
    context.enqueue(batchSize, buffers, stream, nullptr);
    CHECK(cudaMemcpyAsync(output, buffers[outputIndex], batchSize * OUTPUT_SIZE * sizeof(float), cudaMemcpyDeviceToHost, stream));
    cudaStreamSynchronize(stream);

    // Release stream and buffers
    cudaStreamDestroy(stream);
    CHECK(cudaFree(buffers[inputIndex]));
    CHECK(cudaFree(buffers[outputIndex]));
}
int main()
{
    char* trtModelStream(nullptr);
    size_t size{0};
//        IHostMemory* modelStream{nullptr};

//        APItoModel(1,&modelStream);

//        assert(modelStream != nullptr);
//        std::ofstream p("test.engine",std::ios::binary);//create now has p pointer to the
//        // CREATE THE ENGINE
//        assert(modelStream!=nullptr);
//        if(!p){
//            std::cout<<"could not open the file"<<std::endl;
//            return -1;
//        }
//        p.write(reinterpret_cast<const char*>(modelStream->data()),modelStream->size());
//        modelStream->destroy();// has already write to the engine
//        return -1;
    std::cout<<"runtime in the inference "<<std::endl;
    std::ifstream file("test.engine",std::ios::binary);

    if(file.good()){
        //ios::beg begin pointer of file
        //ios::cur currently pointer of file
        //ios::end end pointer of file
        file.seekg(0,file.end);//set the file pointer
        size = file.tellg();// store the current pointer location
        file.seekg(0,file.beg);
        trtModelStream = new char[size];
        assert(trtModelStream);
        file.read(trtModelStream,size);
        file.close();
    }else{
        return -1;
    }
    //    set the store the input blob
    float data[INPUT_H*INPUT_W];
    for(int i = 0;i<INPUT_H*INPUT_W;++i){
        data[i]=1.0;
    }
    IRuntime* runtime = createInferRuntime(glogger);
    assert(runtime!=nullptr);
    ICudaEngine* engine = runtime->deserializeCudaEngine(trtModelStream,size,nullptr);//Deserialize an engine from a stream.
    assert(engine!=nullptr);
    IExecutionContext* context = engine->createExecutionContext();
    assert(context!=nullptr);

    float prob[OUTPUT_SIZE];
    for(int i = 0;i<1000;++i){
        auto start = std::chrono::system_clock::now();
        doInference(*context,data,prob,1);
        //        doInferencesrc(*context,data,prob,1);
        auto end = std::chrono::system_clock::now();
    }
    context->destroy();
    engine->destroy();
    runtime->destroy();
    std::cout<<"\n output prob"<<std::endl;
    for(unsigned int i = 0;i<OUTPUT_SIZE;++i){
        std::cout<<prob[i];
    }
    std::cout<<std::endl;
    //    INetworkDefinition *network;
    //    const string filePath = "./lenet5.wts";
    //    std::map<std::string ,Weights> weightmap = loadWeights(filePath);
    //   std::cout<<
    //    network->addConvolution();
    cout << "Hello World!" << endl;
    return 0;
}
