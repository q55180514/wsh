#include <chrono>
#include <thread>

// 引入FFmpeg库的头文件
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

// 服务器类，用于发送H.264帧
class RtpUdpServer {
public:
    // 构造函数，初始化RTP/RTCP和UDP流
    RtpUdpServer(const char* udpAddress, int udpPort) : udpAddress(udpAddress), udpPort(udpPort) {
        // 注册FFmpeg库的所有组件
        av_register_all();
        // 初始化网络模块
        avformat_network_init();
        // 创建输出格式上下文并设置为RTP/RTCP和UDP
        avformat_alloc_output_context2(&formatContext, nullptr, "rtp", udpAddress);
        // 初始化RTP流
        InitializeRtpStream();
        // 初始化RTCP流
        InitializeRtcpStream();
    }

    // 析构函数，释放资源
    ~RtpUdpServer() {
        // 写入尾部信息
        av_write_trailer(formatContext);
        // 释放上下文
        avformat_free_context(formatContext);
    }

    // 发送H.264帧
    void SendH264Frame(const uint8_t* data, int size) {
        if (rtpStream && data && size > 0) {
            // 模拟丢包情况
            if (rand() % 10 == 0) {
                fprintf(stderr, "Simulating packet loss\n");
                return;
            }

            AVPacket packet;
            av_init_packet(&packet);
            packet.data = const_cast<uint8_t*>(data);
            packet.size = size;
            // 发送H.264帧
            av_interleaved_write_frame(formatContext, &packet);

            // 模拟发送速率控制，每发送一帧休眠一段时间
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
	// 处理RTCP反馈信息
    void ProcessRtcpFeedback() {
        // 模拟处理RTCP反馈信息，可以根据具体需求调整发送速率等
        fprintf(stderr, "Processing RTCP feedback\n");
        // 在实际应用中，根据RTCP反馈信息进行发送速率调整等操作
    }

private:
    const char* udpAddress; // UDP地址
    int udpPort;            // UDP端口
    AVFormatContext* formatContext; // 输出格式上下文
    AVStream* rtpStream;    // RTP流
    AVStream* rtcpStream;   // RTCP流

    // 初始化RTP流
    void InitializeRtpStream() {
        rtpStream = avformat_new_stream(formatContext, nullptr);
        if (!rtpStream) {
            fprintf(stderr, "Error creating RTP stream\n");
            exit(EXIT_FAILURE);
        }

        rtpStream->id = 0;
        rtpStream->time_base = { 1, 90000 };

        AVCodecParameters* codecParameters = avcodec_parameters_alloc();
        codecParameters->codec_type = AVMEDIA_TYPE_VIDEO;
        codecParameters->codec_id = AV_CODEC_ID_H264;
        avcodec_parameters_copy(rtpStream->codecpar, codecParameters);
    }

    // 初始化RTCP流
    void InitializeRtcpStream() {
        rtcpStream = avformat_new_stream(formatContext, nullptr);
        if (!rtcpStream) {
            fprintf(stderr, "Error creating RTCP stream\n");
            exit(EXIT_FAILURE);
        }

        rtcpStream->id = 1;
        rtcpStream->time_base = { 1, 90000 };

        AVCodecParameters* codecParameters = avcodec_parameters_alloc();
        codecParameters->codec_type = AVMEDIA_TYPE_VIDEO;
        codecParameters->codec_id = AV_CODEC_ID_H264;
        avcodec_parameters_copy(rtcpStream->codecpar, codecParameters);
    }
};

// 客户端类，用于接收H.264帧
class RtpUdpClient {
public:
    // 构造函数，初始化RTP/RTCP和UDP输入流
    RtpUdpClient(const char* udpAddress, int udpPort) : udpAddress(udpAddress), udpPort(udpPort) {
        // 注册FFmpeg库的所有组件
        av_register_all();
        // 初始化网络模块
        avformat_network_init();
        // 创建输入格式上下文并设置为RTP/RTCP和UDP
        avformat_alloc_input_context2(&formatContext, nullptr, "rtp", udpAddress);
        // 初始化RTP流
        InitializeRtpStream();
        // 初始化RTCP流
        InitializeRtcpStream();
        // 打开输入流
        avformat_open_input(&formatContext, udpAddress, nullptr, nullptr);
        // 查找流信息
        avformat_find_stream_info(formatContext, nullptr);
    }

    // 析构函数，释放资源
    ~RtpUdpClient() {
        // 关闭输入流
        avformat_close_input(&formatContext);
    }

    // 读取并返回H.264帧
    int ReceiveH264Frame(uint8_t* buffer, int bufferSize) {
        if (rtpStream) {
            AVPacket packet;
            av_init_packet(&packet);
            if (av_read_frame(formatContext, &packet) >= 0) {
                if (packet.stream_index == rtpStream->index && packet.size <= bufferSize) {
                    memcpy(buffer, packet.data, packet.size);
                    av_packet_unref(&packet);
                    return packet.size;
                }
                av_packet_unref(&packet);
            }
        }
        return -1;
    }

private:
    const char* udpAddress; // UDP地址
    int udpPort;            // UDP端口
    AVFormatContext* formatContext; // 输入格式上下文
    AVStream* rtpStream;    // RTP流
    AVStream* rtcpStream;   // RTCP流

    // 初始化RTP流
    void InitializeRtpStream() {
        rtpStream = avformat_new_stream(formatContext, nullptr);
        if (!rtpStream) {
            fprintf(stderr, "Error creating RTP stream\n");
            exit(EXIT_FAILURE);
        }

        rtpStream->id = 0;
        rtpStream->time_base = { 1, 90000 };

        AVCodecParameters* codecParameters = avcodec_parameters_alloc();
        codecParameters->codec_type = AVMEDIA_TYPE_VIDEO;
        codecParameters->codec_id = AV_CODEC_ID_H264;
        avcodec_parameters_copy(rtpStream->codecpar, codecParameters);
    }

    // 初始化RTCP流
    void InitializeRtcpStream() {
        rtcpStream = avformat_new_stream(formatContext, nullptr);
        if (!rtcpStream) {
            fprintf(stderr, "Error creating RTCP stream\n");
            exit(EXIT_FAILURE);
        }

        rtcpStream->id = 1;
        rtcpStream->time_base = { 1, 90000 };

        AVCodecParameters* codecParameters = avcodec_parameters_alloc();
        codecParameters->codec_type = AVMEDIA_TYPE_VIDEO;
        codecParameters->codec_id = AV_CODEC_ID_H264;
        avcodec_parameters_copy(rtcpStream->codecpar, codecParameters);
    }
};

// 服务器主函数
int main_server() {
   RtpUdpServer server("udp://127.0.0.1:1234", 1234);

    // 模拟不断发送H.264帧并处理RTCP反馈信息
    while (true) {
        // 模拟获取H.264帧数据
        uint8_t h264Frame[] = { /* Your H.264 frame data */ };
        int frameSize = sizeof(h264Frame);

        // 发送H.264帧
        server.SendH264Frame(h264Frame, frameSize);

        // 模拟定期处理RTCP反馈信息
        server.ProcessRtcpFeedback();
    }

    return 0;
}

// 客户端主函数
int main_client() {
    RtpUdpClient client("udp://127.0.0.1:1234", 1234);

    // 模拟接收H.264帧
    uint8_t receivedBuffer[1024];
    int receivedSize = client.ReceiveH264Frame(receivedBuffer, sizeof(receivedBuffer));
    if (receivedSize > 0) {
        // 处理接收到的H.264帧数据
    }

    return 0;
}

