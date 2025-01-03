DROP TABLE IF EXISTS t_user;
CREATE TABLE `t_user` (
    `id` bigint NOT NULL PRIMARY AUTO_INCREMENT,
    `username` varchar(32) NOT NULL,
    `password` varchar(32) NOT NULL,
    `phone` varchar(32) DEFAULT NULL,
    `create_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    `update_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    UNIQUE KEY `uniq_phone` (`phone`)
)