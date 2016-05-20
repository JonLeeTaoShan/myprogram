/*
 Navicat MySQL Data Transfer

 Source Server         : 119.57.124.211 数据库
 Source Server Type    : MySQL
 Source Server Version : 50173
 Source Host           : 119.57.124.211
 Source Database       : upload_db

 Target Server Type    : MySQL
 Target Server Version : 50173
 File Encoding         : utf-8

 Date: 05/20/2016 15:41:07 PM
*/

SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
--  Table structure for `dev_upload_tb`
-- ----------------------------
DROP TABLE IF EXISTS `dev_upload_offline_tb`;
CREATE TABLE `dev_upload_offline_tb` (
  `id` int(1) unsigned NOT NULL AUTO_INCREMENT,
  `nowtime` datetime DEFAULT NULL,
  `time` int(1) unsigned DEFAULT NULL,
  `mac` bigint(1) unsigned DEFAULT NULL,
  `devtype` char(4) DEFAULT NULL,
  `propid` text,
  `wandev` text,
  `landev` char(128) DEFAULT NULL,
  `mem` char(20) DEFAULT NULL,
  `cdevlist` text CHARACTER SET utf8,
  `softversion` char(20) DEFAULT NULL,
  `others` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE (nowtime,mac)
) ENGINE=MyISAM AUTO_INCREMENT=1433 DEFAULT CHARSET=latin1;

SET FOREIGN_KEY_CHECKS = 1;
