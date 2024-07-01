/**
 * 10.5.22-MariaDB
 */

DELIMITER ;

CREATE DATABASE `sensor_DB` /*!40100 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci */;

USE `sensor_DB`;

SET GLOBAL event_scheduler = ON;


-- sensor_DB.Sensor definition

CREATE TABLE `Sensor` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `dev_addr` VARCHAR(16) NOT NULL,
  `last_refresh` TIMESTAMP NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `gps_longitude` DECIMAL(11, 8),
  `gps_latitude` DECIMAL(10, 8),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;


-- sensor_DB.SensorData definition

CREATE TABLE `SensorData` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `sensor_id` INT NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `temperature` FLOAT DEFAULT NULL,
  `humidity` FLOAT DEFAULT NULL,
  `pressure` FLOAT DEFAULT NULL,
  `gas_resistance` FLOAT DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `SensorData_FK` (`sensor_id`),
  CONSTRAINT `SensorData_FK` FOREIGN KEY (`sensor_id`) REFERENCES `Sensor` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;


-- sensor_DB.`User` definition

CREATE TABLE `User` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(100) NOT NULL,
  `password` VARCHAR(128) NOT NULL,
  `user_permissions` INT UNSIGNED DEFAULT 0,
  `name` VARCHAR(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;


-- sensor_DB_db.`Session` definition

CREATE TABLE `Session` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `session_id` VARCHAR(64) NOT NULL,
  `user_id` INT NOT NULL,
  `device` VARCHAR(300) DEFAULT NULL,
  `last_refresh` TIMESTAMP NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `Session_User_FK` (`user_id`),
  CONSTRAINT `Session_User_FK` FOREIGN KEY (`user_id`) REFERENCES `User` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

-- Auto delete expored sessions, every 10 minutes check, not used for over 6 hours

CREATE EVENT delete_expired_sessions
ON SCHEDULE EVERY 10 MINUTE
DO
  DELETE FROM Session WHERE TIMESTAMPDIFF(HOUR, last_refresh, NOW()) > 6;


DELIMITER ;



