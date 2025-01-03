#include <stdio.h>
#include <signal.h>
#include <cstring> 
#include <iostream>
#include <atomic>
#ifdef _WIN32
#include <Windows.h>
#define MySleepSec(x) Sleep(x*1000)
#else
#include <unistd.h>
#define MySleepSec(x) usleep((x)*1000*1000)
#endif
#include <librdkafka/rdkafka.h>
using namespace std;


std::atomic_bool global_is_run{true};
static void stop(int sig) 
{
    global_is_run.store(false);
}

static void dr_msg_cb(rd_kafka_t *rk, const rd_kafka_message_t *rkmessage, void *opaque) 
{
    if (rkmessage->err) {
        std::cerr << "ERROR: Message delivery failed: " << rd_kafka_err2str(rkmessage->err) << endl;
    }
    else {
        std::cout << "Success Message delivered (" << rkmessage->len << "bytes, partition " << rkmessage->partition << ")" << endl;
    }
}


static void KafKaProduce(rd_kafka_t *rk, rd_kafka_topic_t *rkt, const std::string &dataPackage) 
{

retry:
    /* Send or Producer Message.
    这是一个异步调用，在成功的情况下，只会将消息排入内部producer队列，对
    broker的实际传递尝试由后台线程处理，之前注册的传递回调函数dr_msg_cb用
    于在消息传递成功或失败时向应用程式发回信号
    */
    if (rd_kafka_produce(rkt, RD_KAFKA_PARTITION_UA, RD_KAFKA_MSG_F_COPY, (void*)dataPackage.c_str(), dataPackage.length(), NULL, 0, NULL) == -1) {
        std::cerr << "%% Failed to produce to topic " << rd_kafka_topic_name(rkt)
            << ": " << rd_kafka_err2str(rd_kafka_last_error()) << endl;

        if (rd_kafka_last_error() == RD_KAFKA_RESP_ERR__QUEUE_FULL) {
            /* 如果内部队列满，等待消息传输完成并retry, 内部队列表示要发送的消息和已
            发送或失败的消息，内部队列受限于queue, buffering.max.messages配置项
            */
            rd_kafka_poll(rk, 1000);
            goto retry;
        }
    }
    else {
        std::cout << "produce message (" << dataPackage.length() << "bytes) for topic " << rd_kafka_topic_name(rkt) << endl;
    }

    /* Producer应用程序应不断地通过以频繁的间隔调用rd_kafka_poll()来为传送报告队列提供服务。在没有
    生成消息以确认先前生成的消息已发送了其发送报告回调函数（和其它注册过的回调函数）期间，要确
    保rd_kafka_poll()仍然被调用
    */
    rd_kafka_poll(rk, 0);
}

int main(int argc, char **argv)
{
    bool is_open_sasl2 = true;
    rd_kafka_t *rk = NULL;                /* Producer instance handle */
    rd_kafka_topic_t *rkt = NULL;        /* Topic对象 */
    rd_kafka_conf_t *conf = NULL;        /* Config Object */
    
    char errStr[512] = {0};
    //const char *brokers = "192.168.0.223:9092,192.168.0.224:9092,192.168.0.225:9092";
    const char *brokers = "master:9092,slave01:9092,slave02:9092";
    const char *topic = "sunquan";
    const char *protocol = "SASL_PLAINTEXT";
    const char *servicename = "kafka";
    const char *keytab = "/home/xunce/xunce.keytab";
    const char *principal = "kafka/hu53@XUNCE.COM";
    const char *mechanism = "GSSAPI";

    /* 创建Kafka配置对象 */
    conf = rd_kafka_conf_new();

    char builtin_features[120] = {0};
    size_t len_size = sizeof(builtin_features);
    rd_kafka_conf_get(conf, "builtin.features", builtin_features, &len_size);
    printf("builtin.features: <%s>.\n", builtin_features);

   
    /* 创建broker集群 */
    if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers, errStr, sizeof(errStr)) != RD_KAFKA_CONF_OK)
    {
        std::cerr << "0001: " << errStr << endl;
        return -1;
    }
    if (is_open_sasl2) {
          std::cout<< " is_open_sasl2 option true !!" <<std::endl;
        if (rd_kafka_conf_set(conf, "security.protocol", protocol, errStr, sizeof(errStr)) != RD_KAFKA_CONF_OK ||
            rd_kafka_conf_set(conf, "sasl.kerberos.service.name", servicename, errStr, sizeof(errStr)) != RD_KAFKA_CONF_OK ||
            rd_kafka_conf_set(conf, "sasl.kerberos.keytab", keytab, errStr, sizeof(errStr)) != RD_KAFKA_CONF_OK ||
            rd_kafka_conf_set(conf, "sasl.kerberos.principal", principal, errStr, sizeof(errStr)) != RD_KAFKA_CONF_OK ||
            rd_kafka_conf_set(conf, "sasl.mechanism", mechanism, errStr, sizeof(errStr)) != RD_KAFKA_CONF_OK)
        {
            std::cerr << "0002: " << errStr << endl;
            return -1;
        }
    }
    

    std::cout << "initial OK!" << endl;
    /* 设置发送报告回调函数，rd_kafka_produce() 接收的每条消息都会调用一次该回调函数，
        应用程序需要定期调用rd_kafka_poll() 来服务排队的发送报告回调函数 
    */
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    /* 创建Producer 实例*/
    rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errStr, sizeof(errStr));
    if (!rk) {
        std::cerr << "%% Failed to create new producer: " << errStr << endl;
        return -1;
    }

    /* 实例化一个或多个topics(`rd_kafka_topic_t`)来提供生产或消费，topic对象保存topic
        特定的配置，并在内部填充所有可用分区和leader brokers
    */
    rkt = rd_kafka_topic_new(rk, topic, NULL);
    if (!rkt) {
        std::cerr << "%% Failed to create topic object: " << rd_kafka_err2str(rd_kafka_last_error()) << endl;
        rd_kafka_destroy(rk);
        return -1;
    }

    /* 用于中断的信号 */
    signal(SIGINT, stop);
 
    std::string fileDataPackage ="this is an test_msg!";
    int count = 0;
    while (global_is_run) {
        count++;
        KafKaProduce(rk, rkt, fileDataPackage);
        printf("id:%d, topic: %s, publish msg len %ld.\n", count, topic, fileDataPackage.size());
         
        MySleepSec(3);
    }

    std::cerr << "%% Flushing final message ..." << endl;
    /* rd_kafka_flush是rd_kafka_poll()的抽象化，等待所有未完成的produce请求完成，通常在销毁producer实例前
        完成，以确保所有排列中和正在传输的produce请求在销毁前完成
    */
    rd_kafka_flush(rk, 10 * 1000);

    /* 销毁topic 实例 */
    rd_kafka_topic_destroy(rkt);

    /* 销毁producer 实例 */
    rd_kafka_destroy(rk);

    getchar();
    return 0;
}
