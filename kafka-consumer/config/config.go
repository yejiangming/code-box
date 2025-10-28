package config

import (
	"fmt"
	"os"

	"gopkg.in/yaml.v3"
)

// Config 代表 YAML 配置文件的结构
type Config struct {
	Kafka KafkaConfig `yaml:"kafka"`
}

// KafkaConfig 代表 Kafka 相关配置
type KafkaConfig struct {
	Brokers []string `yaml:"brokers"`
	Topic   string   `yaml:"topic"`
	GroupID string   `yaml:"group_id"`
}

// LoadConfig 从 YAML 文件加载配置
func LoadConfig(filename string) (*Config, error) {
	data, err := os.ReadFile(filename)
	if err != nil {
		return nil, fmt.Errorf("读取配置文件失败: %v", err)
	}

	var config Config
	err = yaml.Unmarshal(data, &config)
	if err != nil {
		return nil, fmt.Errorf("解析 YAML 配置文件失败: %v", err)
	}

	// 验证必需的配置项
	if len(config.Kafka.Brokers) == 0 {
		return nil, fmt.Errorf("Kafka brokers 配置不能为空")
	}
	if config.Kafka.Topic == "" {
		return nil, fmt.Errorf("Kafka topic 配置不能为空")
	}
	if config.Kafka.GroupID == "" {
		return nil, fmt.Errorf("Kafka group_id 配置不能为空")
	}

	return &config, nil
}
