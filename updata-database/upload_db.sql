/*
 Navicat Premium Data Transfer

 Source Server         : 119.57.124.211 数据库
 Source Server Type    : MySQL
 Source Server Version : 50173
 Source Host           : 119.57.124.211
 Source Database       : upload_db

 Target Server Type    : MySQL
 Target Server Version : 50173
 File Encoding         : utf-8

 Date: 05/27/2016 16:56:59 PM
*/

SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
--  Table structure for `dev_upload_offline_tb`
-- ----------------------------
DROP TABLE IF EXISTS `dev_upload_offline_tb`;
CREATE TABLE `dev_upload_offline_tb` (
  `id` int(1) unsigned DEFAULT NULL,
  `devtype` char(4) DEFAULT NULL,
  `mac` bigint(1) unsigned DEFAULT NULL,
  `nowtime` datetime DEFAULT NULL,
  `time` int(1) unsigned DEFAULT NULL,
  `mem` char(20) DEFAULT NULL,
  `propid` text,
  `wandev` text,
  `landev` char(128) DEFAULT NULL,
  `cdevlist` text CHARACTER SET utf8,
  `bandwidth` text,
  `softversion` char(20) DEFAULT NULL,
  `others` varchar(128) DEFAULT NULL,
  `erruptime` datetime DEFAULT NULL,
  `errtype` char(10) DEFAULT NULL,
  UNIQUE KEY `nowtime` (`nowtime`,`mac`,`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
--  Table structure for `dev_upload_tb`
-- ----------------------------
DROP TABLE IF EXISTS `dev_upload_tb`;
CREATE TABLE `dev_upload_tb` (
  `id` int(1) unsigned NOT NULL AUTO_INCREMENT,
  `devtype` char(4) DEFAULT NULL,
  `mac` bigint(1) unsigned DEFAULT NULL,
  `nowtime` datetime DEFAULT NULL,
  `time` int(1) unsigned DEFAULT NULL,
  `mem` char(20) DEFAULT NULL,
  `propid` text,
  `wandev` text,
  `landev` char(128) DEFAULT NULL,
  `cdevlist` text CHARACTER SET utf8,
  `bandwidth` text,
  `softversion` char(20) DEFAULT NULL,
  `others` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5340 DEFAULT CHARSET=latin1;

-- ----------------------------
--  Table structure for `shop_by_channelID`
-- ----------------------------
DROP TABLE IF EXISTS `shop_by_channelID`;
CREATE TABLE `shop_by_channelID` (
  `id` int(1) unsigned NOT NULL AUTO_INCREMENT,
  `name` char(100) DEFAULT NULL,
  `table_name` text,
  `channelID` varchar(48) CHARACTER SET latin1 DEFAULT NULL,
  `nameuser` text CHARACTER SET latin1,
  `passwd` text,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `shop_xibei`
-- ----------------------------
DROP TABLE IF EXISTS `shop_xibei`;
CREATE TABLE `shop_xibei` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` char(100) DEFAULT NULL,
  `shopID` varchar(48) CHARACTER SET latin1 DEFAULT NULL,
  `macList` text CHARACTER SET latin1,
  `adminPhone` varchar(20) CHARACTER SET latin1 DEFAULT NULL,
  `adminEmail` text CHARACTER SET latin1,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

SET FOREIGN_KEY_CHECKS = 1;
