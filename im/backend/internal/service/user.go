package service

import (
	"context"
	redisidgenerator "im/internal/infra/redis-id-generator"
	"im/internal/model"
	"im/internal/store"
	"time"
)

type UserRegisterParam struct {
	Username string `json:"username" binding:"required"`
	Password string `json:"password" binding:"required"`
	Phone    string `json:"phone" binding:"required"`
}

func UserRegister(ctx context.Context, param UserRegisterParam) error {

	t := time.Now()
	userModel := model.UserModel{
		Username:   param.Username,
		Password:   param.Password,
		Phone:      param.Phone,
		CreateTime: t,
		UpdateTime: t,
	}

	uid, err := redisidgenerator.UidGenerator.NextId(ctx)
	if err != nil {
		return err
	}
	userModel.Uid = uid
	userModel.Uid = 51

	err = store.ImDB.Table(model.UserTableName()).Create(&userModel).Error
	if err != nil {
		// TODO uid 重复时, 设置重新申请号段发号
		return err
	}

	return nil
}
