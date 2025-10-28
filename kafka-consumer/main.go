package main

import (
	"context"
	"flag"
	"fmt"
	"kafka-consumer/config"
	"log"
	"os"
	"os/signal"
	"sync"
	"syscall"

	"github.com/IBM/sarama"
)

// KafkaConsumer 结构体用于管理 Kafka 消费者
type KafkaConsumer struct {
	brokers       []string
	topic         string
	consumerGroup string
	client        sarama.ConsumerGroup
	ready         chan bool
}

// NewKafkaConsumer 创建新的 Kafka 消费者实例
func NewKafkaConsumer(brokers []string, topic, consumerGroup string) (*KafkaConsumer, error) {
	config := sarama.NewConfig()
	config.Version = sarama.V2_6_0_0

	// 消费者配置
	config.Consumer.Group.Rebalance.Strategy = sarama.BalanceStrategyRoundRobin
	config.Consumer.Offsets.Initial = sarama.OffsetNewest
	config.Consumer.Group.Session.Timeout = 10 * 1000 * 1000 * 1000   // 10 seconds
	config.Consumer.Group.Heartbeat.Interval = 3 * 1000 * 1000 * 1000 // 3 seconds

	client, err := sarama.NewConsumerGroup(brokers, consumerGroup, config)
	if err != nil {
		return nil, fmt.Errorf("创建消费者组失败: %v", err)
	}

	return &KafkaConsumer{
		brokers:       brokers,
		topic:         topic,
		consumerGroup: consumerGroup,
		client:        client,
		ready:         make(chan bool),
	}, nil
}

// Consumer 实现 sarama.ConsumerGroupHandler 接口
type Consumer struct {
	ready chan bool
}

// Setup 在会话开始前调用
func (consumer *Consumer) Setup(sarama.ConsumerGroupSession) error {
	close(consumer.ready)
	return nil
}

// Cleanup 在会话结束后调用
func (consumer *Consumer) Cleanup(sarama.ConsumerGroupSession) error {
	return nil
}

// ConsumeClaim 处理消息
func (consumer *Consumer) ConsumeClaim(session sarama.ConsumerGroupSession, claim sarama.ConsumerGroupClaim) error {
	for {
		select {
		case message := <-claim.Messages():
			if message == nil {
				return nil
			}

			// 处理消息
			err := consumer.processMessage(message)
			if err != nil {
				log.Printf("处理消息失败: %v", err)
				// 根据业务需求决定是否继续消费或重试
				continue
			}

			// 标记消息已处理
			session.MarkMessage(message, "")

		case <-session.Context().Done():
			return nil
		}
	}
}

// processMessage 处理单个消息的业务逻辑
func (consumer *Consumer) processMessage(message *sarama.ConsumerMessage) error {
	log.Printf("收到消息 - Topic: %s, Partition: %d, Offset: %d",
		message.Topic, message.Partition, message.Offset)
	log.Printf("消息键: %s", string(message.Key))
	log.Printf("消息值: %s", string(message.Value))
	log.Printf("消息时间戳: %v", message.Timestamp)

	// 在这里添加您的业务逻辑
	// 例如：解析 JSON、存储到数据库、调用 API 等

	return nil
}

// Start 启动消费者
func (kc *KafkaConsumer) Start(ctx context.Context) error {
	wg := &sync.WaitGroup{}
	wg.Add(1)

	go func() {
		defer wg.Done()
		consumer := &Consumer{
			ready: kc.ready,
		}

		for {
			if err := kc.client.Consume(ctx, []string{kc.topic}, consumer); err != nil {
				log.Printf("消费者错误: %v", err)
				return
			}

			// 检查上下文是否被取消
			if ctx.Err() != nil {
				return
			}
		}
	}()

	<-kc.ready
	log.Printf("Kafka 消费者已启动，消费组: %s，主题: %s", kc.consumerGroup, kc.topic)
	wg.Wait()

	return nil
}

// Close 关闭消费者
func (kc *KafkaConsumer) Close() error {
	return kc.client.Close()
}

func main() {
	// 解析命令行参数
	configFile := flag.String("config", "conf.yaml", "配置文件路径")
	flag.Parse()

	// 加载配置文件
	config, err := config.LoadConfig(*configFile)
	if err != nil {
		log.Fatalf("加载配置文件失败: %v", err)
	}

	// 从配置文件获取 Kafka 参数
	brokers := config.Kafka.Brokers
	topic := config.Kafka.Topic
	consumerGroup := config.Kafka.GroupID

	log.Printf("开始连接 Kafka - Brokers: %v, Topic: %s, Group: %s",
		brokers, topic, consumerGroup)

	// 创建 Kafka 消费者
	consumer, err := NewKafkaConsumer(brokers, topic, consumerGroup)
	if err != nil {
		log.Fatalf("创建 Kafka 消费者失败: %v", err)
	}

	// 创建上下文用于优雅关闭
	ctx, cancel := context.WithCancel(context.Background())

	// 处理系统信号
	go func() {
		sigterm := make(chan os.Signal, 1)
		signal.Notify(sigterm, syscall.SIGINT, syscall.SIGTERM)
		<-sigterm
		log.Println("收到终止信号，正在关闭消费者...")
		cancel()
	}()

	// 启动消费者
	err = consumer.Start(ctx)
	if err != nil {
		log.Printf("启动消费者时发生错误: %v", err)
	}

	// 关闭消费者
	if err := consumer.Close(); err != nil {
		log.Printf("关闭消费者时发生错误: %v", err)
	}

	log.Println("Kafka 消费者已关闭")
}
