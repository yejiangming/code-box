package redisidgenerator

import (
	"context"
	"fmt"
	"math/rand"
	"sync"
	"testing"
	"time"
)

var (
	mu     sync.Mutex
	uidMap = make(map[int64]bool)
	dupCnt = 0
)

func getIdWorker(workerId int64, applyStep int64, grandStep int) error {
	ctx := context.Background()
	generator, err := NewRedisIdGenerator("test:generator:uid", applyStep, grandStep)
	if err != nil {
		return err
	}

	for i := 0; i < 100; i++ {
		time.Sleep(time.Duration(rand.Intn(1000)) * time.Millisecond)
		id, err := generator.NextId(ctx)
		if err != nil {
			return err
		}

		mu.Lock()
		if _, ok := uidMap[id]; ok {
			fmt.Printf("workerId: %d, id: %d, duplicate\n", workerId, id)
			generator.nextId = generator.end
			dupCnt++
		} else {
			uidMap[id] = true
			fmt.Printf("workerId: %d, id: %d\n", workerId, id)
		}
		mu.Unlock()
	}

	return nil
}

func TestSequential(t *testing.T) {
	getIdWorker(1, 10, 9)
}

func TestConcurrent(t *testing.T) {
	var wg sync.WaitGroup
	for i := 0; i < 10; i++ {
		wg.Add(1)
		go func(i int64, wg *sync.WaitGroup) {
			getIdWorker(i, i+5, 1)
			wg.Done()

		}(int64(i), &wg)
	}
	wg.Wait()
	fmt.Printf("dup cnt:%v\n", dupCnt)
}
