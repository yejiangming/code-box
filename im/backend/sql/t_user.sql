DROP TABLE IF EXISTS t_user;
CREATE TABLE `t_user` (
    `id` bigint NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `uid` bigint NOT NULL,
    `username` varchar(32) NOT NULL,
    `password` varchar(32) NOT NULL,
    `phone` varchar(32) DEFAULT NULL,
    `create_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    `update_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    UNIQUE KEY `uniq_uid` (`uid`),
    UNIQUE KEY `uniq_phone` (`phone`)
)