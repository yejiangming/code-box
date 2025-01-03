package service

import (
	"im/internal/model"
	"im/internal/store"
	"time"
)

type UserRegisterParam struct {
	Username string `json:"username" binding:"required"`
	Password string `json:"password" binding:"required"`
	Phone    string `json:"phone" binding:"required"`
}

func UserRegister(param UserRegisterParam) error {

	t := time.Now()
	userModel := model.UserModel{
		Username:   param.Username,
		Password:   param.Password,
		Phone:      param.Phone,
		CreateTime: t,
		UpdateTime: t,
	}
	err := store.ImDB.Table(model.UserTableName()).Create(&userModel).Error
	if err != nil {
		return err
	}

	return nil
}
