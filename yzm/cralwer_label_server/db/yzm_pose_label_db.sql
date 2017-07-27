/*
Navicat MySQL Data Transfer

Source Server         : 120.24.249.45
Source Server Version : 50629
Source Host           : 120.24.249.45:3306
Source Database       : yzm_version_test_db

Target Server Type    : MYSQL
Target Server Version : 50629
File Encoding         : 65001

Date: 2017-07-12 09:42:23
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_picture
-- ----------------------------
DROP TABLE IF EXISTS `t_picture`;
CREATE TABLE `t_picture` (
  `Fid` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '主键',
  `Fpic_url`    varchar(256) NOT NULL DEFAULT '',
  `Fpic_md5`     varchar(32)  NOT NULL  DEFAULT '',
  `Fpic_status`  tinyint(4)     NOT NULL  DEFAULT '0',
  `Fkey_word` varchar(256)  NOT NULL  DEFAULT '',
  `Ftag_word` varchar(256)  NOT NULL  DEFAULT '',
  `Fpose_type` varchar(64)  NOT NULL  DEFAULT '',
  `Fpre2DPoseInfo` text  NOT NULL  ,
  `Fpre3DPoseInfo` text  NOT NULL  ,
  `Flabel_count`   tinyint(4)     NOT NULL  DEFAULT '5',
  `Fcreate_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `Fupdate_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`Fid`),
  KEY `pic_name_index` (`Fpic_md5`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `t_label`;
CREATE TABLE `t_label` (
  `Fid` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '主键',
  `Fpic_id`  int(10) NOT NULL DEFAULT '0',
  `Fuser_name`  varchar(64)  NOT NULL  DEFAULT '',
  `Fpose_data` text  NOT NULL  ,
  `Fcreate_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `Fupdate_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`Fid`),
  KEY `user_name_index` (`Fuser_name`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user` (
  `Fid` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '主键',
  `Fuser_id`  int(10) NOT NULL DEFAULT '0',
  `Fuser_name`  varchar(64)  NOT NULL  DEFAULT '',
  `Fuser_phone` text  NOT NULL  ,
  `Frole_id` int(10)  NOT NULL DEFAULT '0' ,
  `Fcreate_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `Fupdate_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`Fid`),
  KEY `user_name_index` (`Fuser_name`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



