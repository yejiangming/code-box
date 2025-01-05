### UID 发号器设计
采用 redis 发号器 + 内存号池两层设计  
内存号池  
```go
type RedisIdGenerator struct {
	// 号池范围 [start, end)
	start  int64
	end    int64
	nextId int64
	mu     sync.Mutex
	rdb    *redis.Client
	key    string
	step   int64
}
```

当号池资源耗尽时(nextId >= end), 从 redis 中获取新一批号, 获取逻辑使用 LUA 脚本实现  
申请 `[start, start+step)` 号段  
如果 redis 发号器中的值 val >= start: 执行 incrby 进行发号, val = val+step, 发放 `[val, val+step)`  
如果 redis 发号器中的值 val < start; 说明可能发生了 redis 主从切换/重启导致回拨, 将 val 设置为 start+step, 发放 `[start, start+step)`, 尽可能快速的进行恢复  
当实例使用发号器生成 uid 重复时, 可以设置 end=nextId 表明号池耗尽, 并将发号器的 end/nextId 设置的尽可能大, 这样在向 redis 申请号池时, 可以快速将 redis 中的值设置为一个较大的值从而避免重复  
可以取 `((maxUid / step)+1) * step`, 这个值大于所有实例的内存发号器  


```go
if r.nextId >= r.end {
    // 获取新一批号段

    luaScript := `
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
    result, err := r.rdb.Eval(ctx, luaScript, []string{r.key},
        strconv.FormatInt(r.end, 10),
        strconv.FormatInt(r.step, 10)).Result()
	}
```

多个协程使用一个 RedisIdGenerator 取号由锁进行保护  
测试程序: 10个协程,每个协程使用自己的 RedisIdGenerator 进行取号操作, 设置不同的 step, 可以保证取出不重复的号  

LUA 脚本预加载  

为了被防止预测实际的 uid 数量, uid 可以设计为跳跃递增, 在 RedisIdGenerator 中引入一个 grandStep, 每次发号时随机增加 rand.Intn(r.grandStep) + 1  
还有一些其他的手段, 比如设置一个定时器, 每月一次, 对 redis 中的发号器加一个比较大的值  

