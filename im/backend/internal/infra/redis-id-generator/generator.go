package redisidgenerator

import (
	"context"
	"errors"
	"math/rand"
	"strconv"
	"sync"
	"time"

	"github.com/redis/go-redis/v9"
)

var (
	luaScript = `
		local seq_value = tonumber(redis.call('GET', KEYS[1]))
		local start_value = tonumber(ARGV[1])
		if seq_value and seq_value < start_value then
			local step = tonumber(ARGV[2])
			local end_value = start_value + step
			redis.call('SET', KEYS[1], end_value)
			return end_value
		else
			return redis.call('INCRBY', KEYS[1], ARGV[2])
		end
	`
	UidGenerator *RedisIdGenerator
)

func Init() error {
	var err error
	UidGenerator, err = NewRedisIdGenerator("generator:uid", 1000, 10)
	if err != nil {
		return err
	}
	return nil
}

type RedisIdGenerator struct {
	// 号池范围 [start, end)
	start     int64
	end       int64
	nextId    int64
	mu        sync.Mutex
	rdb       *redis.Client
	key       string
	applyStep int64
	grandStep int

	scriptSha string
}

func NewRedisIdGenerator(key string, applyStep int64, grandStep int) (*RedisIdGenerator, error) {

	if int64(grandStep) >= applyStep {
		return nil, errors.New("grandStep >= applyStep")
	}

	ctx := context.Background()
	rdb := redis.NewClient(&redis.Options{
		Addr:        "127.0.0.1:6379",
		Password:    "im-redis",
		DB:          0,
		ReadTimeout: 2 * time.Second,
		DialTimeout: 2 * time.Second,
	})

	if err := rdb.Ping(ctx).Err(); err != nil {
		return nil, err
	}

	script := redis.NewScript(luaScript)
	sha, err := script.Load(ctx, rdb).Result()
	if err != nil {
		return nil, err
	}

	return &RedisIdGenerator{
		start:     0,
		end:       0,
		nextId:    0,
		mu:        sync.Mutex{},
		rdb:       rdb,
		key:       key,
		applyStep: applyStep,
		grandStep: grandStep,
		scriptSha: sha,
	}, nil
}

func (r *RedisIdGenerator) NextId(ctx context.Context) (int64, error) {
	r.mu.Lock()
	defer r.mu.Unlock()

	if r.nextId >= r.end {
		// 获取新一批号段

		result, err := r.rdb.EvalSha(ctx, r.scriptSha, []string{r.key},
			strconv.FormatInt(r.end, 10),
			strconv.FormatInt(r.applyStep, 10)).Result()

		if err != nil {
			return 0, err
		}

		val, ok := result.(int64)
		if !ok {
			return 0, errors.New("reset error")
		}

		r.end = val
		r.start = val - r.applyStep
		r.nextId = r.start + int64(rand.Intn(r.grandStep)) + 1
	}

	id := r.nextId
	r.nextId = r.nextId + int64(rand.Intn(r.grandStep)) + 1

	return id, nil
}
