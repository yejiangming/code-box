package store

import (
	"log"

	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

var (
	ImDB *gorm.DB
)

func init() {
	dsn := "root:im-mysql-password@tcp(127.0.0.1:3306)/im?charset=utf8mb4&parseTime=True&loc=Local"
	var err error
	ImDB, err = gorm.Open(mysql.New(mysql.Config{
		DSN: dsn,
	}), &gorm.Config{})

	if err != nil {
		log.Fatal(err)
	}
}
