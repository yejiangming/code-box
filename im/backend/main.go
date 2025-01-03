package main

import (
	"im/internal/handler"
	"time"

	"github.com/gin-gonic/gin"
)

func main() {
	router := gin.New()

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
