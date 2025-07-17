package main

import (
	"im/internal/handler"
	redisidgenerator "im/internal/infra/redis-id-generator"
	"time"

	"github.com/gin-gonic/gin"
)

func main() {

	err := redisidgenerator.Init()
	if err != nil {
		panic(err)
	}

	router := gin.New()

	router.Static("/html", "./static/html")
	router.Static("/css", "./static/css")
	router.Static("/js", "./static/js")

	userGroup := router.Group("/user")
	userGroup.POST("/register", handler.UserRegister)

	router.GET("/ping", func(c *gin.Context) {
		time.Sleep(time.Millisecond * 300)
		c.JSON(200, gin.H{
			"message": "ping-pong",
		})
	})

	router.Run(":8081")
}
